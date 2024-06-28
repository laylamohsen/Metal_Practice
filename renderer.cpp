//
//  renderer.cpp
//  Metal-Window
//
//  Created by layla on 27/06/2024.
//

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "renderer.hpp"
#include <iostream>
#include <vector>
using namespace std;

Renderer::Renderer(MTL::Device * const pDevice):
//: _pDrawable(pDrawable),
  _pDevice(pDevice),
  _pCommandQueue(_pDevice->newCommandQueue()),
_pRenderPipelineState(nullptr, [](MTL::RenderPipelineState * const p) { p->release(); })
//_timer()

{
  buildShaders();
}

void Renderer::buildShaders() {
  const std::unique_ptr<MTL::Library, void(*)(MTL::Library * const)> pLib(_pDevice->newDefaultLibrary(), [](MTL::Library * const l) { l->release(); });
  const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> pVertexFn(pLib.get()->newFunction(NS::String::string("vertexShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });
  const std::unique_ptr<MTL::Function, void(*)(MTL::Function * const)> pFragmentFn(pLib.get()->newFunction(NS::String::string("fragmentShader", NS::UTF8StringEncoding)), [](MTL::Function * const f) { f->release(); });
  
  const std::unique_ptr<MTL::RenderPipelineDescriptor, void(*)(MTL::RenderPipelineDescriptor * const)> pRpd(MTL::RenderPipelineDescriptor::alloc()->init(), [](MTL::RenderPipelineDescriptor * const d) { d->release(); });
  pRpd.get()->setVertexFunction(pVertexFn.get());
  pRpd.get()->setFragmentFunction(pFragmentFn.get());
  pRpd.get()->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
  
  NS::Error * pErr {nullptr};
  _pRenderPipelineState.reset(_pDevice->newRenderPipelineState(pRpd.get(), &pErr));
  if (!_pRenderPipelineState.get()) {
    __builtin_printf("%s", pErr->localizedDescription()->utf8String());
  }
}

Renderer::~Renderer()
{
  _pCommandQueue->release();
}

void Renderer::drawFrame(const CA::MetalDrawable *const pDrawable) {
  MTL::CommandBuffer * pCmdBuf = _pCommandQueue->commandBuffer();

  MTL::RenderPassDescriptor * pRpd = MTL::RenderPassDescriptor::alloc()->init();
  pRpd->colorAttachments()->object(0)->setTexture(pDrawable->texture());
  pRpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
  pRpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.0, 0.0, 0.0, 0.0));

  const std::vector<float> triangle = {
      -0.5f, 0.5f, .0f,
      0.5f, 0.5f, .0f,
      0.0f, -1.0f, .0f,
      -0.5f, -1.0f, .0f
  };
  const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pVertexBuffer(_pDevice->newBuffer(triangle.data(), sizeof(float) * 12, MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); });
    const std::vector<ushort> indices = {
      // one triangle
      0, 1, 2,
      // another triangle
      0, 2, 3
    };
    const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pIndexBuffer(_pDevice->newBuffer(indices.data(), sizeof(ushort) * 6, MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); });

//  _timer += 0.01;

  MTL::RenderCommandEncoder * pEnc = pCmdBuf->renderCommandEncoder(pRpd);
  pEnc->setRenderPipelineState(_pRenderPipelineState.get());
  pEnc->setVertexBuffer(pVertexBuffer.get(), 0, 5);
    pEnc->setTriangleFillMode(MTL::TriangleFillModeLines);
//  pEnc->setVertexBytes(&_timer, sizeof(float), 7);
//  pEnc->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
    pEnc->drawIndexedPrimitives(MTL::PrimitiveTypeTriangle, indices.size(), MTL::IndexTypeUInt16, pIndexBuffer.get(), 0);

  pEnc->endEncoding();
  pCmdBuf->presentDrawable(pDrawable);
  pCmdBuf->commit();

  pRpd->release();
}
//
//void Renderer::draw() const
//{
//  MTL::CommandBuffer * pCmdBuf = _pCommandQueue->commandBuffer();
//  
//  MTL::RenderPassDescriptor * pRpd = MTL::RenderPassDescriptor::alloc()->init();
//  pRpd->colorAttachments()->object(0)->setTexture(_pDrawable->texture());
//  pRpd->colorAttachments()->object(0)->setLoadAction(MTL::LoadActionClear);
//  pRpd->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(1.0, 1.0, 1.0, 1.0));
//    const vector<float> triangle = {
//        -0.5f, 0.5f, 0.0f,
//        0.5f, 0.5f, 0.0f,
//        0.0f, -1.0f, 0.0f
//    };
//    
////    MTL::Buffer * pVertices = _pDevice->newBuffer(triangle.data(), sizeof(float)*9, MTL::ResourceStorageModeShared);
////    pVertices->release();
////  
//    const std::unique_ptr<MTL::Buffer, void(*)(MTL::Buffer * const)> pVertexBuffer(_pDevice->newBuffer(triangle.data(), sizeof(float) * 9, MTL::ResourceStorageModeShared), [](MTL::Buffer * const b) { b->release(); });
//    
//
//  MTL::RenderCommandEncoder * pEnc = pCmdBuf->renderCommandEncoder(pRpd);
//    
//    /*Number of Buffers*/
//  pEnc->setVertexBuffer(pVertexBuffer.get(), 0, 5);
//  pEnc->setRenderPipelineState(_pRenderPipelineState.get());
//
//  pEnc->drawPrimitives(MTL::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3));
//
//  pEnc->endEncoding();
//  pCmdBuf->presentDrawable(_pDrawable);
//  pCmdBuf->commit();
//  
//  pRpd->release();
//}
