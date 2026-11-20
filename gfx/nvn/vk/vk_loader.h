#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void vk_loader_init(void);
void *vk_loader_get_handle(void);
int vk_loader_is_custom(void);

#ifdef __cplusplus
}
#endif
