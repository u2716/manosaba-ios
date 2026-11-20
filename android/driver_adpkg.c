// android/driver_adpkg.c: Custom GPU driver package (*.adpkg.zip) loader for Android.
// Scans the external data folder (/storage/emulated/0/Android/data/<package>/files)
// where game files and fonts live, extracts custom Vulkan drivers (e.g. Mesa Turnip)
// into internal app storage, and configures the environment for adrenotools/Vulkan loader.

#include "driver_adpkg.h"
#include <android/asset_manager.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <jni.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <zlib.h>
#include "../host/dm.h"

#pragma pack(push, 1)
typedef struct {
    uint32_t sig;            // 0x06054b50
    uint16_t disk_no;
    uint16_t cd_disk;
    uint16_t disk_entries;
    uint16_t total_entries;
    uint32_t cd_size;
    uint32_t cd_offset;
    uint16_t comment_len;
} zip_eocd_t;

typedef struct {
    uint32_t sig;            // 0x02014b50
    uint16_t version_made;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t method;         // 0: store, 8: deflate
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t comp_size;
    uint32_t uncomp_size;
    uint16_t name_len;
    uint16_t extra_len;
    uint16_t comment_len;
    uint16_t disk_start;
    uint16_t int_attr;
    uint32_t ext_attr;
    uint32_t local_offset;
} zip_cd_entry_t;

typedef struct {
    uint32_t sig;            // 0x04034b50
    uint16_t version_needed;
    uint16_t flags;
    uint16_t method;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t comp_size;
    uint32_t uncomp_size;
    uint16_t name_len;
    uint16_t extra_len;
} zip_local_header_t;
#pragma pack(pop)

static void mkdirs(const char *p) {
    char b[1024]; snprintf(b, sizeof b, "%s", p);
    for (char *s = b + 1; *s; s++) if (*s == '/') { *s = 0; mkdir(b, 0755); *s = '/'; }
    mkdir(b, 0755);
}

static int ends_with(const char *str, const char *suffix) {
    size_t slen = strlen(str), sflen = strlen(suffix);
    return slen >= sflen && !strcasecmp(str + slen - sflen, suffix);
}

static int is_driver_package(const char *name) {
    if (ends_with(name, ".adpkg.zip") || ends_with(name, ".adpkg")) return 1;
    if (ends_with(name, ".zip")) {
        if (strcasestr(name, "turnip") || strcasestr(name, "adreno") || strcasestr(name, "driver") || strcasestr(name, "mesa"))
            return 1;
    }
    return 0;
}

static int unzip_archive(const char *zip_path, const char *dest_dir) {
    FILE *f = fopen(zip_path, "rb");
    if (!f) return -1;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz < (long)sizeof(zip_eocd_t)) { fclose(f); return -1; }

    long scan_start = sz - 65536 - (long)sizeof(zip_eocd_t);
    if (scan_start < 0) scan_start = 0;
    long eocd_pos = -1;
    for (long pos = sz - sizeof(zip_eocd_t); pos >= scan_start; pos--) {
        fseek(f, pos, SEEK_SET);
        uint32_t sig = 0;
        if (fread(&sig, 4, 1, f) == 1 && sig == 0x06054b50) {
            eocd_pos = pos;
            break;
        }
    }
    if (eocd_pos < 0) { fclose(f); return -2; }

    fseek(f, eocd_pos, SEEK_SET);
    zip_eocd_t eocd;
    if (fread(&eocd, sizeof eocd, 1, f) != 1) { fclose(f); return -3; }

    mkdirs(dest_dir);
    fseek(f, eocd.cd_offset, SEEK_SET);
    int extracted = 0;
    for (uint16_t i = 0; i < eocd.total_entries; i++) {
        zip_cd_entry_t cd;
        if (fread(&cd, sizeof cd, 1, f) != 1 || cd.sig != 0x02014b50) break;
        char name[512];
        int nlen = cd.name_len < 511 ? cd.name_len : 511;
        if (fread(name, 1, nlen, f) != (size_t)nlen) break;
        name[nlen] = 0;
        if (cd.name_len > nlen) fseek(f, cd.name_len - nlen, SEEK_CUR);
        if (cd.extra_len) fseek(f, cd.extra_len, SEEK_CUR);
        if (cd.comment_len) fseek(f, cd.comment_len, SEEK_CUR);

        long next_cd = ftell(f);

        // Sanitize path (strip leading slashes or relative components)
        const char *clean_name = name;
        while (*clean_name == '/' || *clean_name == '\\') clean_name++;
        if (strstr(clean_name, "../")) { fseek(f, next_cd, SEEK_SET); continue; }

        char out_path[1024];
        snprintf(out_path, sizeof out_path, "%s/%s", dest_dir, clean_name);
        if (clean_name[strlen(clean_name) - 1] == '/') {
            mkdirs(out_path);
            fseek(f, next_cd, SEEK_SET);
            continue;
        }

        char *slash = strrchr(out_path, '/');
        if (slash) {
            *slash = 0;
            mkdirs(out_path);
            *slash = '/';
        }

        fseek(f, cd.local_offset, SEEK_SET);
        zip_local_header_t loc;
        if (fread(&loc, sizeof loc, 1, f) != 1 || loc.sig != 0x04034b50) {
            fseek(f, next_cd, SEEK_SET);
            continue;
        }
        fseek(f, loc.name_len + loc.extra_len, SEEK_CUR);

        uint8_t *comp_buf = malloc(cd.comp_size ? cd.comp_size : 1);
        if (!comp_buf || fread(comp_buf, 1, cd.comp_size, f) != cd.comp_size) {
            free(comp_buf);
            fseek(f, next_cd, SEEK_SET);
            continue;
        }

        uint8_t *uncomp_buf = NULL;
        if (cd.method == 0) {
            uncomp_buf = comp_buf;
        } else if (cd.method == 8) {
            uncomp_buf = malloc(cd.uncomp_size ? cd.uncomp_size : 1);
            if (uncomp_buf) {
                z_stream strm;
                memset(&strm, 0, sizeof strm);
                strm.next_in = comp_buf;
                strm.avail_in = cd.comp_size;
                strm.next_out = uncomp_buf;
                strm.avail_out = cd.uncomp_size;
                if (inflateInit2(&strm, -MAX_WBITS) == Z_OK) {
                    inflate(&strm, Z_FINISH);
                    inflateEnd(&strm);
                }
            }
            free(comp_buf);
        } else {
            free(comp_buf);
            fseek(f, next_cd, SEEK_SET);
            continue;
        }

        if (uncomp_buf) {
            FILE *out = fopen(out_path, "wb");
            if (out) {
                fwrite(uncomp_buf, 1, cd.uncomp_size, out);
                fclose(out);
                if (ends_with(out_path, ".so")) chmod(out_path, 0755);
                else chmod(out_path, 0644);
                extracted++;
            }
            free(uncomp_buf);
        }

        fseek(f, next_cd, SEEK_SET);
    }

    fclose(f);
    return extracted;
}

static int json_extract_string(const char *json, const char *key, char *out, size_t out_sz) {
    char needle[128];
    snprintf(needle, sizeof needle, "\"%s\"", key);
    char *p = strstr(json, needle);
    if (!p) return 0;
    p = strchr(p + strlen(needle), ':');
    if (!p) return 0;
    p = strchr(p, '"');
    if (!p) return 0;
    p++;
    char *end = strchr(p, '"');
    if (!end) return 0;
    size_t len = end - p;
    if (len >= out_sz) len = out_sz - 1;
    memcpy(out, p, len);
    out[len] = 0;
    return 1;
}

static int parse_driver_meta(const char *driver_dir, char *lib_name, size_t lib_sz, char *drv_title, size_t title_sz) {
    char meta_path[1024];
    snprintf(meta_path, sizeof meta_path, "%s/meta.json", driver_dir);
    FILE *f = fopen(meta_path, "r");
    if (f) {
        char buf[8192];
        size_t n = fread(buf, 1, sizeof(buf) - 1, f);
        fclose(f);
        buf[n] = 0;

        char name[128] = "", ver[64] = "", vendor[64] = "";
        json_extract_string(buf, "name", name, sizeof name);
        json_extract_string(buf, "driverVersion", ver, sizeof ver);
        json_extract_string(buf, "vendor", vendor, sizeof vendor);
        if (drv_title && title_sz > 0) {
            snprintf(drv_title, title_sz, "%s%s%s%s%s",
                     name[0] ? name : "Custom Driver",
                     ver[0] ? " " : "", ver,
                     vendor[0] ? " (" : "", vendor[0] ? vendor : "");
            if (vendor[0]) strncat(drv_title, ")", title_sz - strlen(drv_title) - 1);
        }

        if (json_extract_string(buf, "libraryName", lib_name, lib_sz)) {
            char full[1024];
            snprintf(full, sizeof full, "%s/%s", driver_dir, lib_name);
            struct stat st;
            if (stat(full, &st) == 0 && S_ISREG(st.st_mode)) return 1;
        }
    }

    // Fallback: look for standard known names or any .so in driver_dir
    const char *candidates[] = {"vulkan.adreno.so", "libvulkan_freedreno.so", "vulkan.turnip.so"};
    for (size_t i = 0; i < sizeof(candidates)/sizeof(candidates[0]); i++) {
        char full[1024];
        snprintf(full, sizeof full, "%s/%s", driver_dir, candidates[i]);
        struct stat st;
        if (stat(full, &st) == 0 && S_ISREG(st.st_mode)) {
            snprintf(lib_name, lib_sz, "%s", candidates[i]);
            if (drv_title && !drv_title[0]) snprintf(drv_title, title_sz, "Turnip (%s)", candidates[i]);
            return 1;
        }
    }

    DIR *d = opendir(driver_dir);
    if (d) {
        struct dirent *de;
        while ((de = readdir(d))) {
            if (ends_with(de->d_name, ".so")) {
                snprintf(lib_name, lib_sz, "%s", de->d_name);
                if (drv_title && !drv_title[0]) snprintf(drv_title, title_sz, "Custom Driver (%s)", de->d_name);
                closedir(d);
                return 1;
            }
        }
        closedir(d);
    }

    return 0;
}

static void copy_hook_from_assets(AAssetManager *am, const char *asset_name, const char *dst_path) {
    struct stat st;
    if (stat(dst_path, &st) == 0 && st.st_size > 0) return;
    AAsset *a = AAssetManager_open(am, asset_name, AASSET_MODE_BUFFER);
    if (!a) return;
    const void *buf = AAsset_getBuffer(a);
    off_t len = AAsset_getLength(a);
    if (buf && len > 0) {
        FILE *f = fopen(dst_path, "wb");
        if (f) {
            fwrite(buf, 1, len, f);
            fclose(f);
            chmod(dst_path, 0755);
        }
    }
    AAsset_close(a);
}

int dm_android_driver_setup(ANativeActivity *a, const char *ext_dir, const char *int_dir) {
    if (!ext_dir || !int_dir) return 0;

    DIR *d = opendir(ext_dir);
    if (!d) return 0;

    char chosen_pkg[1024] = "";
    char chosen_name[512] = "";
    time_t latest_mtime = 0;
    struct dirent *de;

    while ((de = readdir(d))) {
        if (de->d_name[0] == '.') continue;
        if (is_driver_package(de->d_name)) {
            char path[1024];
            snprintf(path, sizeof path, "%s/%s", ext_dir, de->d_name);
            struct stat st;
            if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
                if (st.st_mtime >= latest_mtime) {
                    latest_mtime = st.st_mtime;
                    snprintf(chosen_pkg, sizeof chosen_pkg, "%s", path);
                    snprintf(chosen_name, sizeof chosen_name, "%s", de->d_name);
                }
            }
        }
    }
    closedir(d);

    if (!chosen_pkg[0]) {
        LOG("driver: no custom Vulkan driver (*.adpkg.zip) in %s, using system Vulkan", ext_dir);
        return 0;
    }

    struct stat st;
    if (stat(chosen_pkg, &st) != 0) return 0;

    char driver_dir[1024], stamp_path[1024], hooks_dir[1024];
    snprintf(driver_dir, sizeof driver_dir, "%s/driver", int_dir);
    snprintf(stamp_path, sizeof stamp_path, "%s/.driver_stamp", driver_dir);
    snprintf(hooks_dir, sizeof hooks_dir, "%s/hooks", int_dir);
    mkdirs(driver_dir);
    mkdirs(hooks_dir);

    // Check stamp
    int need_extract = 1;
    FILE *sf = fopen(stamp_path, "r");
    if (sf) {
        char s_name[512] = "";
        unsigned long long s_size = 0, s_mtime = 0;
        if (fscanf(sf, "%511s %llu %llu", s_name, &s_size, &s_mtime) == 3) {
            if (!strcmp(s_name, chosen_name) && s_size == (unsigned long long)st.st_size && s_mtime == (unsigned long long)st.st_mtime) {
                need_extract = 0;
            }
        }
        fclose(sf);
    }

    if (need_extract) {
        LOG("driver: extracting custom Vulkan driver package %s (%.1f MB) to %s...",
            chosen_name, st.st_size / 1e6, driver_dir);
        int count = unzip_archive(chosen_pkg, driver_dir);
        if (count <= 0) {
            LOG("[red] driver: failed to unpack %s (rc=%d)", chosen_name, count);
            return 0;
        }
        sf = fopen(stamp_path, "w");
        if (sf) {
            fprintf(sf, "%s %llu %llu\n", chosen_name, (unsigned long long)st.st_size, (unsigned long long)st.st_mtime);
            fclose(sf);
        }
        LOG("driver: unpacked %d files from %s", count, chosen_name);
    } else {
        LOG("driver: using already unpacked driver %s in %s", chosen_name, driver_dir);
    }

    char lib_name[256] = "", title[256] = "";
    if (!parse_driver_meta(driver_dir, lib_name, sizeof lib_name, title, sizeof title)) {
        LOG("[red] driver: could not identify driver shared library in %s", driver_dir);
        return 0;
    }

    // Setup hook libraries if bundled as assets
    if (a && a->assetManager) {
        char h1[1024], h2[1024];
        snprintf(h1, sizeof h1, "%s/libmain_hook.so", hooks_dir);
        snprintf(h2, sizeof h2, "%s/libhook_impl.so", hooks_dir);
        copy_hook_from_assets(a->assetManager, "hooks/libmain_hook.so", h1);
        copy_hook_from_assets(a->assetManager, "hooks/libhook_impl.so", h2);
    }

    // Check if applicationInfo has nativeLibraryDir with hooks
    char hook_dir_use[1024];
    snprintf(hook_dir_use, sizeof hook_dir_use, "%s", hooks_dir);
    if (a && a->env && a->clazz) {
        JNIEnv *E = a->env;
        jclass C = (*E)->GetObjectClass(E, a->clazz);
        jmethodID m_ai = (*E)->GetMethodID(E, C, "getApplicationInfo", "()Landroid/content/pm/ApplicationInfo;");
        if (m_ai) {
            jobject ai = (*E)->CallObjectMethod(E, a->clazz, m_ai);
            if (ai) {
                jclass AI = (*E)->GetObjectClass(E, ai);
                jfieldID f_nld = (*E)->GetFieldID(E, AI, "nativeLibraryDir", "Ljava/lang/String;");
                if (f_nld) {
                    jstring s = (*E)->GetObjectField(E, ai, f_nld);
                    if (s) {
                        const char *nld = (*E)->GetStringUTFChars(E, s, NULL);
                        if (nld) {
                            char check[1024];
                            snprintf(check, sizeof check, "%s/libmain_hook.so", nld);
                            struct stat st_h;
                            if (stat(check, &st_h) == 0) {
                                snprintf(hook_dir_use, sizeof hook_dir_use, "%s", nld);
                            }
                            (*E)->ReleaseStringUTFChars(E, s, nld);
                        }
                        (*E)->DeleteLocalRef(E, s);
                    }
                }
                (*E)->DeleteLocalRef(E, AI);
                (*E)->DeleteLocalRef(E, ai);
            }
        }
        (*E)->DeleteLocalRef(E, C);
    }

    LOG("driver: custom GPU driver configured: %s [%s] (library: %s)",
        title[0] ? title : chosen_name, chosen_name, lib_name);

    setenv("DM_VK_DRIVER_DIR", driver_dir, 1);
    setenv("DM_VK_DRIVER_NAME", lib_name, 1);
    setenv("DM_VK_HOOK_DIR", hook_dir_use, 1);
    setenv("DM_VK_DRIVER_PKG", chosen_pkg, 1);

    return 1;
}
