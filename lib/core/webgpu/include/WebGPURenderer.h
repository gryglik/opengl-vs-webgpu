#pragma once
#include "Renderer.h"

#if defined(__APPLE__)
#import <QuartzCore/CAMetalLayer.h>
#endif

#if defined(_WIN32)
#include <windows.h>
#endif

#include <cstdlib>
#include <iostream>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <webgpu/webgpu_cpp.h>

class WebGPURenderer : public Renderer {
public:
  WebGPURenderer(Window &window) : Renderer(window) {}
  void init() override;
  void render() override;

private:
  wgpu::Instance instance;
  wgpu::Device device;
  wgpu::Adapter adapter;
  wgpu::Surface surface;
  wgpu::TextureFormat format;
  wgpu::RenderPipeline pipeline;

  void prepareDevice();
  void prepareSurface();
  void prepareShaders();

#if defined(__APPLE__)
  std::unique_ptr<wgpu::ChainedStruct, void (*)(wgpu::ChainedStruct *)>
      macChain = {nullptr, [](wgpu::ChainedStruct *) {}};
#endif
};
