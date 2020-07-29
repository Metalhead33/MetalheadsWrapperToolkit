#ifndef TEXTURE3D_HPP
#define TEXTURE3D_HPP
#include "Texture.hpp"
namespace Abstract {
namespace GFX {

class Texture3D : public Texture {
public:
	virtual ~Texture3D() = default;
	virtual unsigned getWidth() const = 0;
	virtual unsigned getHeight() const = 0;
	virtual unsigned getDepth() const = 0;
	virtual unsigned getStride() const = 0;
};

}
}
#endif // TEXTURE3D_HPP
