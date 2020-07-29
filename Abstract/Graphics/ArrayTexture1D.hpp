#ifndef ARRAYTEXTURE1D_HPP
#define ARRAYTEXTURE1D_HPP
#include "Texture.hpp"
namespace Abstract {
namespace GFX {

class ArrayTexture1D : public Texture {
public:
	virtual ~ArrayTexture1D() = default;
	virtual unsigned getWidth() const = 0;
	virtual unsigned getImageCount() const = 0;
};

}
}
#endif // ARRAYTEXTURE1D_HPP
