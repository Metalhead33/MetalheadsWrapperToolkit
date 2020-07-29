#ifndef STREAMINGTEXTURE1D_HPP
#define STREAMINGTEXTURE1D_HPP
#include "Texture1D.hpp"
#include "Streamable.hpp"
namespace Abstract {
namespace GFX {

class StreamingTexture1D : public Texture1D, public Streamable {
public:
	virtual ~StreamingTexture1D() = default;
	virtual void* map() = 0;
	virtual void unmap() = 0;
};

}
}
#endif // STREAMINGTEXTURE1D_HPP
