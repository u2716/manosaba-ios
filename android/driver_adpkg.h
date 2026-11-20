#pragma once
#include <android/native_activity.h>

#ifdef __cplusplus
extern "C" {
#endif

// Scans ext_dir (/storage/emulated/0/Android/data/<package>/files) for *.adpkg.zip.
// If found, extracts it to int_dir/driver (if not already extracted / changed),
// reads meta.json for libraryName (or locates .so), and sets DM_VK_DRIVER_DIR,
// DM_VK_DRIVER_NAME, and DM_VK_HOOK_DIR in the environment.
// Returns 1 if a custom driver package was found and configured, 0 otherwise.
int dm_android_driver_setup(ANativeActivity *a, const char *ext_dir, const char *int_dir);

#ifdef __cplusplus
}
#endif
