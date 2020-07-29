#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "UniversalHandle.hpp"
namespace Abstract {
namespace GFX {

enum class PixelFormat : uint8_t {
	// Layout
	INVALID = 0,
	// Standard formats
	R_U8,
	R_S8,
	R_U16,
	R_S16,
	R_U32,
	R_S32,
	R_HF,
	R_F,
	RG_U8,
	RG_S8,
	RG_U16,
	RG_S16,
	RG_U32,
	RG_S32,
	RG_HF,
	RG_F,
	RGB_U8,
	RGB_S8,
	BGR_U8,
	BGR_S8,
	RGB_U16,
	RGB_S16,
	RGB_U32,
	RGB_S32,
	RGB_HF,
	RGB_F,
	RGBA_U8,
	RGBA_S8,
	BGRA_U8,
	BGRA_S8,
	RGBA_U16,
	RGBA_S16,
	RGBA_U32,
	RGBA_S32,
	RGBA_HF,
	RGBA_F,
	// More unusual formats
	RGB_U444, // 4 bits for RGB
	RGB_U565, // 5 bits for R and B, 6 for G
	RGBA_U5551, // 5 bits for RGB, 1 for A
	RGBA_U4444, // 4 bits for RGBA
	// Depth formats
	DEPTH24_STENCIL8, // 24 bits for depth, 8 for stencil
	DEPTH32_STENCIL8, // 32 bits for depth, 8 for stencil, 24 bits unused
	DEPTH_F32, // 32-bit float depth.
	// While stencil alone is supported by OpenGL and Vulkan, it is not supported by D3D
	// Since we're aiming for the lowest common denominator, we won't support it
	// DDX compressed formats
	S3TC_DXT1_RGB,
	S3TC_DXT1_RGBA,
	S3TC_DXT3_RGBA,
	S3TC_DXT5_RGBA
};

class Texture {
public:
	virtual ~Texture() = default;
	virtual unsigned getDimensionCount() const = 0;
	virtual unsigned getLinearSize() const = 0;
	virtual unsigned getSizeInBytes() const = 0;
	virtual PixelFormat getPixelFormat() const = 0;
	virtual GAPI_HANDLE& getHandle() = 0;
	virtual const GAPI_HANDLE& getHandle() const = 0;
};

}
}
#endif // TEXTURE_HPP
