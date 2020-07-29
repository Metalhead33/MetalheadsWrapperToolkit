#ifndef TEXTURE1D_HPP
#define TEXTURE1D_HPP
#include "Texture.hpp"
namespace Abstract {
namespace GFX {

class Texture1D : public Texture {
public:
	virtual ~Texture1D() = default;
	virtual unsigned getWidth() const = 0;
};

}
}
#endif // TEXTURE1D_HPP
