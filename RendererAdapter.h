//
//  RendererAdapter.h
//  Metal-Window
//
//  Created by layla on 27/06/2024.
//

#import "QuartzCore/CAMetalLayer.h"
#import "Metal/MTLDevice.h"
#import <MetalKit/MTKView.h>
#import "renderer.hpp"

@interface RendererAdapter : NSObject<MTKViewDelegate>
{
  Renderer * _pRenderer;
}
-(nonnull instancetype) initWithMTKView: (MTKView *)pView;
@end
