#include "vk.h"
#include "vk_loader.h"
#include "vk_dispatch.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__ANDROID__)
#include <adrenotools/driver.h>
#endif

static void *g_vk_handle = NULL;
static int g_is_custom = 0;

void *vk_loader_get_handle(void) {
    return g_vk_handle;
}

int vk_loader_is_custom(void) {
    return g_is_custom;
}

void vk_loader_init(void) {
    if (g_vk_handle) return;

    const char *driver_dir = getenv("DM_VK_DRIVER_DIR");
    const char *driver_name = getenv("DM_VK_DRIVER_NAME");
    const char *hook_dir = getenv("DM_VK_HOOK_DIR");
    const char *driver_pkg = getenv("DM_VK_DRIVER_PKG");

    if (driver_dir && driver_name && driver_name[0]) {
        VLOG("custom Vulkan driver specified: %s/%s (package: %s)",
             driver_dir, driver_name, driver_pkg ? driver_pkg : "unknown");

#if defined(__ANDROID__)
        // Attempt 1: adrenotools
        char dir_with_slash[1024];
        snprintf(dir_with_slash, sizeof dir_with_slash, "%s/", driver_dir);
        size_t len = strlen(dir_with_slash);
        while (len > 1 && dir_with_slash[len - 1] == '/' && dir_with_slash[len - 2] == '/') {
            dir_with_slash[--len] = '\0';
        }

        Dl_info dlinfo;
        char native_lib_dir[1024] = "";
        if (dladdr((void *)&vk_loader_init, &dlinfo) && dlinfo.dli_fname) {
            snprintf(native_lib_dir, sizeof native_lib_dir, "%s", dlinfo.dli_fname);
            char *slash = strrchr(native_lib_dir, '/');
            if (slash) *slash = '\0';
        }

        const char *effective_hook_dir = hook_dir;
        if (native_lib_dir[0]) {
            char check[1024];
            snprintf(check, sizeof check, "%s/libmain_hook.so", native_lib_dir);
            struct stat st;
            if (stat(check, &st) == 0) {
                effective_hook_dir = native_lib_dir;
            }
        }
        if (!effective_hook_dir || !effective_hook_dir[0]) {
            effective_hook_dir = native_lib_dir[0] ? native_lib_dir : NULL;
        }

        VLOG("loading custom Vulkan driver via adrenotools (hook_dir=%s)...", effective_hook_dir ? effective_hook_dir : "default");
        g_vk_handle = adrenotools_open_libvulkan(
            RTLD_NOW | RTLD_LOCAL,
            ADRENOTOOLS_DRIVER_CUSTOM,
            NULL,
            effective_hook_dir,
            dir_with_slash,
            driver_name,
            NULL,
            NULL
        );

        if (g_vk_handle) {
            PFN_vkGetInstanceProcAddr gipa = (PFN_vkGetInstanceProcAddr)dlsym(g_vk_handle, "vkGetInstanceProcAddr");
            if (gipa) {
                VLOG("custom Vulkan driver successfully loaded via adrenotools (%s)", driver_name);
                g_is_custom = 1;
                vk_dispatch_init_global(gipa);
                return;
            }
            VLOG("adrenotools returned handle but dlsym(vkGetInstanceProcAddr) failed");
            dlclose(g_vk_handle);
            g_vk_handle = NULL;
        } else {
            VLOG("adrenotools_open_libvulkan failed");
        }
#endif

        // Attempt 2: direct dlopen of driver_dir/driver_name
        char full_drv_path[1024];
        snprintf(full_drv_path, sizeof full_drv_path, "%s/%s", driver_dir, driver_name);
        VLOG("attempting direct dlopen of %s...", full_drv_path);
        g_vk_handle = dlopen(full_drv_path, RTLD_NOW | RTLD_GLOBAL);
        if (g_vk_handle) {
            PFN_vkGetInstanceProcAddr gipa = (PFN_vkGetInstanceProcAddr)dlsym(g_vk_handle, "vkGetInstanceProcAddr");
            if (gipa) {
                VLOG("custom Vulkan driver loaded directly from %s", full_drv_path);
                g_is_custom = 1;
                vk_dispatch_init_global(gipa);
                return;
            }
            VLOG("direct dlopen succeeded but dlsym(vkGetInstanceProcAddr) failed");
            dlclose(g_vk_handle);
            g_vk_handle = NULL;
        }
    }

    // Attempt 3: system Vulkan loader (libvulkan.so)
    VLOG("loading system Vulkan driver (libvulkan.so)...");
    g_vk_handle = dlopen("libvulkan.so", RTLD_NOW | RTLD_GLOBAL);
    if (!g_vk_handle) {
        g_vk_handle = dlopen("/system/lib64/libvulkan.so", RTLD_NOW | RTLD_GLOBAL);
    }
    if (!g_vk_handle) {
        VDIE("could not dlopen libvulkan.so: %s", dlerror());
    }
    PFN_vkGetInstanceProcAddr gipa = (PFN_vkGetInstanceProcAddr)dlsym(g_vk_handle, "vkGetInstanceProcAddr");
    if (!gipa) {
        VDIE("could not find vkGetInstanceProcAddr in system libvulkan.so");
    }
    VLOG("system Vulkan driver loaded");
    vk_dispatch_init_global(gipa);
}
