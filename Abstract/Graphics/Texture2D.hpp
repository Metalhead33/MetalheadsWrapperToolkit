#ifndef TEXTURE2D_HPP
#define TEXTURE2D_HPP
#include "Texture.hpp"
namespace Abstract {
namespace GFX {

class Texture2D : public Texture {
public:
	virtual ~Texture2D() = default;
	virtual unsigned getWidth() const = 0;
	virtual unsigned getHeight() const = 0;
	virtual unsigned getStride() const = 0;
};

}
}
#endif // TEXTURE2D_HPP
