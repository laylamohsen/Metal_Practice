//
//  renderer.hpp
//  Metal-Window
//
//  Created by layla on 27/06/2024.
//

#include "Metal/Metal.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"

class Renderer {
public:
    Renderer(MTL::Device * const pDevice);
  ~Renderer();
//  void draw() const;
    void drawFrame(const CA::MetalDrawable * const pDrawable);

private:
//  CA::MetalDrawable * _pDrawable;
  MTL::Device * const _pDevice;
  MTL::CommandQueue * const _pCommandQueue;
  std::unique_ptr<MTL::RenderPipelineState, void(*)(MTL::RenderPipelineState *)> _pRenderPipelineState;
//  float _timer;
  void buildShaders();

};
