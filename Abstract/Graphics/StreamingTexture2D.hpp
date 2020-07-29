#ifndef STREAMINGTEXTURE2D_HPP
#define STREAMINGTEXTURE2D_HPP
#include "Texture2D.hpp"
#include "Streamable.hpp"
namespace Abstract {
namespace GFX {

class StreamingTexture2D : public Texture2D, public Streamable {
public:
	virtual ~StreamingTexture2D() = default;
	virtual void* map() = 0;
	virtual void unmap() = 0;
};

}
}
#endif // STREAMINGTEXTURE2D_HPP
