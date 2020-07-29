#ifndef ARRAYTEXTURE2D_HPP
#define ARRAYTEXTURE2D_HPP
#include "Texture.hpp"
namespace Abstract {
namespace GFX {

class ArrayTexture2D : public Texture {
public:
	virtual ~ArrayTexture2D() = default;
	virtual unsigned getWidth() const = 0;
	virtual unsigned getHeight() const = 0;
	virtual unsigned getImageCount() const = 0;
	virtual unsigned getStride() const = 0;
};

}
}
#endif // ARRAYTEXTURE2D_HPP
