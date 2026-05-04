#include "WebGPURenderer.h"

void WebGPURenderer::prepareDevice() {
  // Access to gpu
  static const auto kTimedWaitAny = wgpu::InstanceFeatureName::TimedWaitAny;
  wgpu::InstanceDescriptor instanceDesc{.requiredFeatureCount = 1,
                                        .requiredFeatures = &kTimedWaitAny};
  instance = wgpu::CreateInstance(&instanceDesc);

  wgpu::Future f1 = instance.RequestAdapter(
      nullptr, wgpu::CallbackMode::WaitAnyOnly,
      [this](wgpu::RequestAdapterStatus status, wgpu::Adapter a,
             wgpu::StringView message) {
        if (status != wgpu::RequestAdapterStatus::Success) {
          // std::cout << "RequestAdapter: " << message << "\n";
          exit(0);
        }
        adapter = std::move(a);
      });
  instance.WaitAny(f1, UINT64_MAX);

  wgpu::DeviceDescriptor desc{};
  desc.SetUncapturedErrorCallback([](const wgpu::Device &,
                                     wgpu::ErrorType errorType,
                                     wgpu::StringView message) {
    // std::cout << "Error: " << errorType << " - message: " << message << "\n";
  });

  wgpu::Future f2 = adapter.RequestDevice(
      &desc, wgpu::CallbackMode::WaitAnyOnly,
      [this](wgpu::RequestDeviceStatus status, wgpu::Device d,
             wgpu::StringView message) {
        if (status != wgpu::RequestDeviceStatus::Success) {
          // std::cout << "RequestDevice: " << message << "\n";
          exit(0);
        }
        device = std::move(d);
      });
  instance.WaitAny(f2, UINT64_MAX);
}

void WebGPURenderer::prepareSurface() {
  wgpu::SurfaceCapabilities capabilities;
  surface.GetCapabilities(adapter, &capabilities);
  format = capabilities.formats[0];

  wgpu::SurfaceConfiguration config{.device = device,
                                    .format = format,
                                    .width = window.getWidth(),
                                    .height = window.getHeight()};
  surface.Configure(&config);
}

void WebGPURenderer::prepareShaders() {
  const char shaderCode[] = R"(
    @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
      @builtin(position) vec4f {
        const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
        return vec4f(pos[i], 0, 1);
    }
    @fragment fn fragmentMain() -> @location(0) vec4f {
        return vec4f(1, 0, 0, 1);
    }
  )";

  wgpu::ShaderSourceWGSL wgsl{{.code = shaderCode}};

  wgpu::ShaderModuleDescriptor shaderModuleDescriptor{.nextInChain = &wgsl};
  wgpu::ShaderModule shaderModule =
      device.CreateShaderModule(&shaderModuleDescriptor);

  wgpu::ColorTargetState colorTargetState{.format = format};

  wgpu::FragmentState fragmentState{
      .module = shaderModule, .targetCount = 1, .targets = &colorTargetState};

  wgpu::RenderPipelineDescriptor descriptor{.vertex = {.module = shaderModule},
                                            .fragment = &fragmentState};
  pipeline = device.CreateRenderPipeline(&descriptor);
}

#if defined(PLATFORM_MACOS)
std::unique_ptr<wgpu::ChainedStruct, void (*)(wgpu::ChainedStruct *)>
SetupWindowAndGetSurfaceDescriptorCocoa(GLFWwindow *window);
#endif

void WebGPURenderer::init() {
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  wgpu::SurfaceDescriptor desc{};

#if defined(PLATFORM_WINDOWS)
  wgpu::SurfaceSourceWindowsHWND win{};
  win.hwnd = glfwGetWin32Window(window);
  win.hinstance = GetModuleHandle(nullptr);
  desc.nextInChain = &win;

#elif defined(PLATFORM_MACOS)
  macChain = SetupWindowAndGetSurfaceDescriptorCocoa(window.getNativeWindow());
  desc.nextInChain = macChain.get();

#elif defined(USE_WAYLAND)
  wgpu::SurfaceSourceWaylandSurface wl{};
  wl.chain.sType = wgpu::SType::SurfaceSourceWaylandSurface;
  wl.display = glfwGetWaylandDisplay();
  wl.surface = glfwGetWaylandWindow(window);
  desc.nextInChain = &wl;

#elif defined(USE_X11)
  wgpu::SurfaceSourceXlibWindow x11{};
  x11.chain.sType = wgpu::SType::SurfaceSourceXlibWindow;
  x11.display = glfwGetX11Display();
  x11.window = glfwGetX11Window(window);
  desc.nextInChain = &x11;
#endif

  surface = instance.CreateSurface(&desc);

  prepareDevice();
  prepareSurface();
  prepareShaders();
}

void WebGPURenderer::render() {
  wgpu::SurfaceTexture surfaceTexture;
  surface.GetCurrentTexture(&surfaceTexture);

  wgpu::RenderPassColorAttachment attachment{
      .view = surfaceTexture.texture.CreateView(),
      .loadOp = wgpu::LoadOp::Clear,
      .storeOp = wgpu::StoreOp::Store};

  wgpu::RenderPassDescriptor renderpass{.colorAttachmentCount = 1,
                                        .colorAttachments = &attachment};

  wgpu::CommandEncoder encoder = device.CreateCommandEncoder();
  wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&renderpass);
  pass.SetPipeline(pipeline);
  pass.Draw(3);
  pass.End();
  wgpu::CommandBuffer commands = encoder.Finish();
  device.GetQueue().Submit(1, &commands);

  surface.Present();
  instance.ProcessEvents();
}
