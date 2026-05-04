#if !defined(DAWN_ENABLE_BACKEND_METAL)
#error "utils_metal.mm requires the Metal backend to be enabled."
#endif  // !defined(DAWN_ENABLE_BACKEND_METAL)

#import <QuartzCore/CAMetalLayer.h>

#include <cstdlib>

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>


std::unique_ptr<wgpu::ChainedStruct, void (*)(wgpu::ChainedStruct*)>
SetupWindowAndGetSurfaceDescriptorCocoa(GLFWwindow* window) {
    @autoreleasepool {
        NSWindow* nsWindow = glfwGetCocoaWindow(window);
        NSView* view = [nsWindow contentView];

        // Create a CAMetalLayer that covers the whole window that will be passed to
        // CreateSurface.
        [view setWantsLayer:YES];
        [view setLayer:[CAMetalLayer layer]];

        // Use retina if the window was created with retina support.
        [[view layer] setContentsScale:[nsWindow backingScaleFactor]];

        wgpu::SurfaceSourceMetalLayer* desc = new wgpu::SurfaceSourceMetalLayer();
        desc->layer = [view layer];
        return {desc, [](wgpu::ChainedStruct* desc) {
                    delete reinterpret_cast<wgpu::SurfaceSourceMetalLayer*>(desc);
                }};
    }
}
