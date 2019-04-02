#ifndef MHFONTWRAPPER_HPP
#define MHFONTWRAPPER_HPP
#include "../Io/FIO.hpp"
#include "MhImageWrapper.hpp"
namespace Mh {

class GlyphWrapper
{
private:
	DEFINE_STRUCT(GlyphWrapper_imp)
	sGlyphWrapper_imp pimpl;
public:
	GlyphWrapper();
};

class FontWrapper
{
private:
	DEFINE_STRUCT(FontWrapper_imp)
	sFontWrapper_imp pimpl;
public:
	FontWrapper();
};

}
#endif // MHFONTWRAPPER_HPP
