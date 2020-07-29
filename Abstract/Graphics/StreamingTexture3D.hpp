#ifndef STREAMINGTEXTURE3D_HPP
#define STREAMINGTEXTURE3D_HPP
#include "Texture3D.hpp"
#include "Streamable.hpp"
namespace Abstract {
namespace GFX {

class StreamingTexture3D : public Texture3D, public Streamable {
public:
	virtual ~StreamingTexture3D() = default;
	virtual void* map() = 0;
	virtual void unmap() = 0;
};

}
}
#endif // STREAMINGTEXTURE3D_HPP
