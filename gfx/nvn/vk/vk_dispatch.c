#include "vk_dispatch.h"
#include <string.h>

// Entry point
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;

// Global commands
PFN_vkCreateInstance vkCreateInstance = NULL;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = NULL;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = NULL;

// Instance commands
PFN_vkDestroyInstance vkDestroyInstance = NULL;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = NULL;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = NULL;
PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2 = NULL;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = NULL;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = NULL;
PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2 = NULL;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = NULL;
PFN_vkCreateDevice vkCreateDevice = NULL;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = NULL;
PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = NULL;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = NULL;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = NULL;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = NULL;

// Device commands
PFN_vkGetDeviceQueue vkGetDeviceQueue = NULL;
PFN_vkQueueSubmit2 vkQueueSubmit2 = NULL;
PFN_vkQueueWaitIdle vkQueueWaitIdle = NULL;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle = NULL;
PFN_vkAllocateMemory vkAllocateMemory = NULL;
PFN_vkFreeMemory vkFreeMemory = NULL;
PFN_vkMapMemory vkMapMemory = NULL;
PFN_vkCreateBuffer vkCreateBuffer = NULL;
PFN_vkDestroyBuffer vkDestroyBuffer = NULL;
PFN_vkBindBufferMemory vkBindBufferMemory = NULL;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = NULL;
PFN_vkCreateImage vkCreateImage = NULL;
PFN_vkDestroyImage vkDestroyImage = NULL;
PFN_vkBindImageMemory vkBindImageMemory = NULL;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = NULL;
PFN_vkCreateImageView vkCreateImageView = NULL;
PFN_vkDestroyImageView vkDestroyImageView = NULL;
PFN_vkCreateSampler vkCreateSampler = NULL;
PFN_vkCreateShaderModule vkCreateShaderModule = NULL;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout = NULL;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = NULL;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool = NULL;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = NULL;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = NULL;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = NULL;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = NULL;
PFN_vkCreateComputePipelines vkCreateComputePipelines = NULL;
PFN_vkCreatePipelineCache vkCreatePipelineCache = NULL;
PFN_vkGetPipelineCacheData vkGetPipelineCacheData = NULL;
PFN_vkCreateCommandPool vkCreateCommandPool = NULL;
PFN_vkResetCommandPool vkResetCommandPool = NULL;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = NULL;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers = NULL;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer = NULL;
PFN_vkEndCommandBuffer vkEndCommandBuffer = NULL;
PFN_vkResetCommandBuffer vkResetCommandBuffer = NULL;
PFN_vkCmdBeginRendering vkCmdBeginRendering = NULL;
PFN_vkCmdEndRendering vkCmdEndRendering = NULL;
PFN_vkCmdBindPipeline vkCmdBindPipeline = NULL;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = NULL;
PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2 = NULL;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = NULL;
PFN_vkCmdSetViewport vkCmdSetViewport = NULL;
PFN_vkCmdSetScissor vkCmdSetScissor = NULL;
PFN_vkCmdSetLineWidth vkCmdSetLineWidth = NULL;
PFN_vkCmdSetDepthBias vkCmdSetDepthBias = NULL;
PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable = NULL;
PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants = NULL;
PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable = NULL;
PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable = NULL;
PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp = NULL;
PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable = NULL;
PFN_vkCmdSetStencilOp vkCmdSetStencilOp = NULL;
PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask = NULL;
PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask = NULL;
PFN_vkCmdSetStencilReference vkCmdSetStencilReference = NULL;
PFN_vkCmdSetCullMode vkCmdSetCullMode = NULL;
PFN_vkCmdSetFrontFace vkCmdSetFrontFace = NULL;
PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology = NULL;
PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable = NULL;
PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable = NULL;
PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT = NULL;
PFN_vkCmdDraw vkCmdDraw = NULL;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed = NULL;
PFN_vkCmdDispatch vkCmdDispatch = NULL;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer = NULL;
PFN_vkCmdCopyImage vkCmdCopyImage = NULL;
PFN_vkCmdBlitImage vkCmdBlitImage = NULL;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage = NULL;
PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer = NULL;
PFN_vkCmdClearColorImage vkCmdClearColorImage = NULL;
PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = NULL;
PFN_vkCmdClearAttachments vkCmdClearAttachments = NULL;
PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2 = NULL;
PFN_vkCreateSemaphore vkCreateSemaphore = NULL;
PFN_vkWaitSemaphores vkWaitSemaphores = NULL;
PFN_vkCreateFence vkCreateFence = NULL;
PFN_vkDestroyFence vkDestroyFence = NULL;
PFN_vkResetFences vkResetFences = NULL;
PFN_vkWaitForFences vkWaitForFences = NULL;
PFN_vkCreateQueryPool vkCreateQueryPool = NULL;
PFN_vkResetQueryPool vkResetQueryPool = NULL;
PFN_vkCmdResetQueryPool vkCmdResetQueryPool = NULL;
PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2 = NULL;
PFN_vkGetQueryPoolResults vkGetQueryPoolResults = NULL;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = NULL;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = NULL;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = NULL;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = NULL;
PFN_vkQueuePresentKHR vkQueuePresentKHR = NULL;
PFN_vkMapMemory2KHR vkMapMemory2KHR = NULL;
PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR = NULL;
PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR = NULL;

// Events
PFN_vkCreateEvent vkCreateEvent = NULL;
PFN_vkSetEvent vkSetEvent = NULL;
PFN_vkResetEvent vkResetEvent = NULL;
PFN_vkCmdWaitEvents vkCmdWaitEvents = NULL;

static VkInstance s_inst = VK_NULL_HANDLE;
static VkDevice s_dev = VK_NULL_HANDLE;

void vk_dispatch_init_global(PFN_vkGetInstanceProcAddr gipa) {
    vkGetInstanceProcAddr = gipa;
    if (!vkGetInstanceProcAddr) return;

#define LOAD_GLOBAL(name) name = (PFN_##name)vkGetInstanceProcAddr(NULL, #name)
    LOAD_GLOBAL(vkCreateInstance);
    LOAD_GLOBAL(vkEnumerateInstanceExtensionProperties);
    LOAD_GLOBAL(vkEnumerateInstanceLayerProperties);
#undef LOAD_GLOBAL
}

void vk_dispatch_init_instance(VkInstance inst) {
    s_inst = inst;
    if (!vkGetInstanceProcAddr || !s_inst) return;

#define LOAD_INST(name) name = (PFN_##name)vkGetInstanceProcAddr(s_inst, #name)
    LOAD_INST(vkDestroyInstance);
    LOAD_INST(vkEnumeratePhysicalDevices);
    LOAD_INST(vkGetPhysicalDeviceProperties);
    LOAD_INST(vkGetPhysicalDeviceProperties2);
    LOAD_INST(vkGetPhysicalDeviceMemoryProperties);
    LOAD_INST(vkGetPhysicalDeviceQueueFamilyProperties);
    LOAD_INST(vkGetPhysicalDeviceFeatures2);
    LOAD_INST(vkGetPhysicalDeviceFormatProperties);
    LOAD_INST(vkCreateDevice);
    LOAD_INST(vkGetDeviceProcAddr);
    LOAD_INST(vkCreateAndroidSurfaceKHR);
    LOAD_INST(vkDestroySurfaceKHR);
    LOAD_INST(vkGetPhysicalDeviceSurfaceSupportKHR);
    LOAD_INST(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
    LOAD_INST(vkGetPhysicalDeviceSurfaceFormatsKHR);
    LOAD_INST(vkEnumerateDeviceExtensionProperties);
#undef LOAD_INST
}

void vk_dispatch_init_device(VkDevice dev) {
    s_dev = dev;
    if (!s_dev) return;

#define LOAD_DEV(name) do { \
    if (vkGetDeviceProcAddr) name = (PFN_##name)vkGetDeviceProcAddr(s_dev, #name); \
    if (!name && vkGetInstanceProcAddr && s_inst) name = (PFN_##name)vkGetInstanceProcAddr(s_inst, #name); \
} while (0)

    LOAD_DEV(vkGetDeviceQueue);
    LOAD_DEV(vkQueueSubmit2);
    LOAD_DEV(vkQueueWaitIdle);
    LOAD_DEV(vkDeviceWaitIdle);
    LOAD_DEV(vkAllocateMemory);
    LOAD_DEV(vkFreeMemory);
    LOAD_DEV(vkMapMemory);
    LOAD_DEV(vkCreateBuffer);
    LOAD_DEV(vkDestroyBuffer);
    LOAD_DEV(vkBindBufferMemory);
    LOAD_DEV(vkGetBufferMemoryRequirements);
    LOAD_DEV(vkCreateImage);
    LOAD_DEV(vkDestroyImage);
    LOAD_DEV(vkBindImageMemory);
    LOAD_DEV(vkGetImageMemoryRequirements);
    LOAD_DEV(vkCreateImageView);
    LOAD_DEV(vkDestroyImageView);
    LOAD_DEV(vkCreateSampler);
    LOAD_DEV(vkCreateShaderModule);
    LOAD_DEV(vkCreatePipelineLayout);
    LOAD_DEV(vkCreateDescriptorSetLayout);
    LOAD_DEV(vkCreateDescriptorPool);
    LOAD_DEV(vkDestroyDescriptorPool);
    LOAD_DEV(vkAllocateDescriptorSets);
    LOAD_DEV(vkUpdateDescriptorSets);
    LOAD_DEV(vkCreateGraphicsPipelines);
    LOAD_DEV(vkCreateComputePipelines);
    LOAD_DEV(vkCreatePipelineCache);
    LOAD_DEV(vkGetPipelineCacheData);
    LOAD_DEV(vkCreateCommandPool);
    LOAD_DEV(vkResetCommandPool);
    LOAD_DEV(vkAllocateCommandBuffers);
    LOAD_DEV(vkFreeCommandBuffers);
    LOAD_DEV(vkBeginCommandBuffer);
    LOAD_DEV(vkEndCommandBuffer);
    LOAD_DEV(vkResetCommandBuffer);
    LOAD_DEV(vkCmdBeginRendering);
    LOAD_DEV(vkCmdEndRendering);
    LOAD_DEV(vkCmdBindPipeline);
    LOAD_DEV(vkCmdBindDescriptorSets);
    LOAD_DEV(vkCmdBindVertexBuffers2);
    LOAD_DEV(vkCmdBindIndexBuffer);
    LOAD_DEV(vkCmdSetViewport);
    LOAD_DEV(vkCmdSetScissor);
    LOAD_DEV(vkCmdSetLineWidth);
    LOAD_DEV(vkCmdSetDepthBias);
    LOAD_DEV(vkCmdSetDepthBiasEnable);
    LOAD_DEV(vkCmdSetBlendConstants);
    LOAD_DEV(vkCmdSetDepthTestEnable);
    LOAD_DEV(vkCmdSetDepthWriteEnable);
    LOAD_DEV(vkCmdSetDepthCompareOp);
    LOAD_DEV(vkCmdSetStencilTestEnable);
    LOAD_DEV(vkCmdSetStencilOp);
    LOAD_DEV(vkCmdSetStencilCompareMask);
    LOAD_DEV(vkCmdSetStencilWriteMask);
    LOAD_DEV(vkCmdSetStencilReference);
    LOAD_DEV(vkCmdSetCullMode);
    LOAD_DEV(vkCmdSetFrontFace);
    LOAD_DEV(vkCmdSetPrimitiveTopology);
    LOAD_DEV(vkCmdSetPrimitiveRestartEnable);
    LOAD_DEV(vkCmdSetRasterizerDiscardEnable);
    LOAD_DEV(vkCmdSetVertexInputEXT);
    LOAD_DEV(vkCmdDraw);
    LOAD_DEV(vkCmdDrawIndexed);
    LOAD_DEV(vkCmdDispatch);
    LOAD_DEV(vkCmdCopyBuffer);
    LOAD_DEV(vkCmdCopyImage);
    LOAD_DEV(vkCmdBlitImage);
    LOAD_DEV(vkCmdCopyBufferToImage);
    LOAD_DEV(vkCmdCopyImageToBuffer);
    LOAD_DEV(vkCmdClearColorImage);
    LOAD_DEV(vkCmdClearDepthStencilImage);
    LOAD_DEV(vkCmdClearAttachments);
    LOAD_DEV(vkCmdPipelineBarrier2);
    LOAD_DEV(vkCreateSemaphore);
    LOAD_DEV(vkWaitSemaphores);
    LOAD_DEV(vkCreateFence);
    LOAD_DEV(vkDestroyFence);
    LOAD_DEV(vkResetFences);
    LOAD_DEV(vkWaitForFences);
    LOAD_DEV(vkCreateQueryPool);
    LOAD_DEV(vkResetQueryPool);
    LOAD_DEV(vkCmdResetQueryPool);
    LOAD_DEV(vkCmdWriteTimestamp2);
    LOAD_DEV(vkGetQueryPoolResults);
    LOAD_DEV(vkCreateSwapchainKHR);
    LOAD_DEV(vkDestroySwapchainKHR);
    LOAD_DEV(vkGetSwapchainImagesKHR);
    LOAD_DEV(vkAcquireNextImageKHR);
    LOAD_DEV(vkQueuePresentKHR);
    LOAD_DEV(vkMapMemory2KHR);
    LOAD_DEV(vkUnmapMemory2KHR);
    LOAD_DEV(vkCmdPushDescriptorSetKHR);

    LOAD_DEV(vkCreateEvent);
    LOAD_DEV(vkSetEvent);
    LOAD_DEV(vkResetEvent);
    LOAD_DEV(vkCmdWaitEvents);
#undef LOAD_DEV
}

void *vk_get_proc_addr(const char *name) {
    if (!name) return NULL;
    if (s_dev && vkGetDeviceProcAddr) {
        void *p = (void *)vkGetDeviceProcAddr(s_dev, name);
        if (p) return p;
    }
    if (s_inst && vkGetInstanceProcAddr) {
        void *p = (void *)vkGetInstanceProcAddr(s_inst, name);
        if (p) return p;
    }
    if (vkGetInstanceProcAddr) {
        void *p = (void *)vkGetInstanceProcAddr(NULL, name);
        if (p) return p;
    }
    return NULL;
}
