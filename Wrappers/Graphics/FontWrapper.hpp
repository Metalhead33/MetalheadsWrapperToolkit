#ifndef MHFONTWRAPPER_HPP
#define MHFONTWRAPPER_HPP
#include "../../Io/FIO.hpp"
#include "ImageWrapper.hpp"
namespace MH33 {
namespace GFX {
class GlyphWrapper
{
private:
	DEFINE_STRUCT(GlyphWrapper_imp)
	uGlyphWrapper_imp pimpl;
public:
	GlyphWrapper();
};

class FontWrapper
{
private:
	DEFINE_STRUCT(FontWrapper_imp)
	uFontWrapper_imp pimpl;
public:
	FontWrapper();
};

}
}
#endif // MHFONTWRAPPER_HPP
