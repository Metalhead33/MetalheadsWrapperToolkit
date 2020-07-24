#include "FontWrapper.hpp"
extern "C" {
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_SYSTEM_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_BITMAP_H
#include FT_STROKER_H
}
#include <stdexcept>
#include "../../Io/span.hpp"
#include "Vector.hpp"

unsigned long FontRead( FT_Stream stream, unsigned long offset,
								unsigned char*  buffer, unsigned long count)
{
	if(!stream) return 0;
	if(!stream->descriptor.pointer) return 0;
	Abstract::pFIO file = reinterpret_cast<Abstract::pFIO>(stream->descriptor.pointer);
	file->seek(offset);
	if(buffer && count)
	{
		return file->read(buffer,count);
	}
	else return 0;
}
void FontClose(FT_Stream)
{
	;
}

namespace MH33 {
namespace GFX {

typedef std::shared_ptr<FT_LibraryRec_> Library;
typedef std::shared_ptr<FT_FaceRec_> Face;
typedef std::shared_ptr<FT_StrokerRec_> Stroker;
typedef std::shared_ptr<FT_StreamRec> Stream;
typedef std::shared_ptr<FT_GlyphRec_> Glyph;
typedef std::shared_ptr<FT_BitmapGlyphRec_> Bitmap;

Library initializeLibrary()
{
	FT_LibraryRec_* tmp;
	FT_Error err = FT_Init_FreeType(&tmp);
	if(err)
	{
		return nullptr;
	}
	return Library(tmp,FT_Done_FreeType);
}
Stroker createStroker(const Library& lib)
{
	if(!lib) return nullptr;
	FT_StrokerRec_* tmp;
	FT_Error err = FT_Stroker_New(lib.get(),&tmp);
	if(err)
	{
		return nullptr;
	}
	return Stroker(tmp,FT_Stroker_Done);
}
Stream createStream(Abstract::pFIO fio)
{
	if(!fio) return nullptr;
	Stream tmp = Stream(new FT_StreamRec);
	memset(tmp.get(),0,sizeof(FT_StreamRec));
	tmp->read = FontRead;
	tmp->close = FontClose;
	tmp->descriptor.pointer = fio;
	tmp->pos = 0;
	tmp->base = nullptr;
	tmp->size = fio->size();
	return tmp;
}
Face openFace(const Library& lib, const Stream& streamer)
{
	if(!lib) return nullptr;
	if(!streamer) return nullptr;
	FT_Open_Args args;
	memset(&args,0,sizeof(FT_Open_Args));
	args.flags = FT_OPEN_STREAM;
	args.stream = streamer.get();
	args.driver = nullptr;
	FT_FaceRec* tmp;
	FT_Error err = FT_Open_Face(lib.get(),&args,0,&tmp);
	if(err) return nullptr;
	else return Face(tmp,FT_Done_Face);
}
Face openFace(const Library& lib, const char* path)
{
	if(!lib) return nullptr;
	FT_FaceRec* tmp;
	FT_Error err = FT_New_Face(lib.get(),path,0,&tmp);
	if(err) return nullptr;
	else return Face(tmp,FT_Done_Face);
}
Glyph getGlyph(const Face& face)
{
	if(!face) return nullptr;
	FT_GlyphRec_* tmp;
	FT_Error err = FT_Get_Glyph(face->glyph,&tmp);
	if(err) return nullptr;
	else return Glyph(tmp,FT_Done_Glyph);
}
Glyph copyGlyph(const Glyph& glyph)
{
	if(!glyph) return nullptr;
	FT_GlyphRec_* tmp;
	FT_Error err = FT_Glyph_Copy(glyph.get(),&tmp);
	if(err) return nullptr;
	else return Glyph(tmp,FT_Done_Glyph);
}

struct GlyphWrapper::GlyphWrapper_imp {
	ImageWrapper bitmap;
	Vec2<unsigned int> size;
	Vec2<int> bearing;
	Vec2<long> advance;
	~GlyphWrapper_imp() = default;
	GlyphWrapper_imp()
		: bitmap()
	{
		memset(&size,0,sizeof(size));
		memset(&bearing,0,sizeof(bearing));
		memset(&advance,0,sizeof(advance));
	}
	GlyphWrapper_imp(FT_GlyphSlotRec& base)
		: bitmap(base.bitmap.buffer,base.bitmap.width,base.bitmap.rows,
				 std::abs(base.bitmap.pitch),8,0xFF,0xFF,0xFF,false)
	{
		size.x = base.bitmap.width;
		size.y = base.bitmap.rows;
		bearing.x = base.bitmap_left;
		bearing.y = base.bitmap_top;
		advance.x = base.advance.x;
		advance.y = base.advance.y;
	}
	GlyphWrapper_imp(FT_GlyphRec_& base)
		: bitmap()
	{
		/*size.x = base.bitmap.width;
		size.y = base.bitmap.rows;
		bearing.x = base.bitmap_left;
		bearing.y = base.bitmap_top;
		advance.x = base.advance.x;
		advance.y = base.advance.y;*/
	}
	GlyphWrapper_imp(const GlyphWrapper_imp& cpy)
		: bitmap(cpy.bitmap)
	{
		memcpy(&size,&cpy.size,sizeof(size));
		memcpy(&bearing,&cpy.bearing,sizeof(bearing));
		memcpy(&advance,&cpy.advance,sizeof(advance));
	}
	GlyphWrapper_imp(GlyphWrapper_imp&& mov)
		: bitmap(std::move(mov.bitmap))
	{
		memcpy(&size,&mov.size,sizeof(size));
		memset(&mov.size,0,sizeof(size));
		memcpy(&bearing,&mov.bearing,sizeof(bearing));
		memset(&mov.bearing,0,sizeof(bearing));
		memcpy(&advance,&mov.advance,sizeof(advance));
		memset(&mov.advance,0,sizeof(advance));
	}
	void operator=(const GlyphWrapper_imp& cpy)
	{
		this->bitmap = cpy.bitmap;
		memcpy(&this->size,&cpy.size,sizeof(size));
		memcpy(&this->bearing,&cpy.bearing,sizeof(bearing));
		memcpy(&this->advance,&cpy.advance,sizeof(advance));
	}
	void operator=(GlyphWrapper_imp&& mov)
	{
		this->bitmap = std::move(mov.bitmap);
		memcpy(&this->size,&mov.size,sizeof(size));
		memset(&mov.size,0,sizeof(size));
		memcpy(&this->bearing,&mov.bearing,sizeof(bearing));
		memset(&mov.bearing,0,sizeof(bearing));
		memcpy(&this->advance,&mov.advance,sizeof(advance));
		memset(&mov.advance,0,sizeof(advance));
	}
};

struct FontWrapper::FontWrapper_imp {
	Abstract::sFIO fio;
	Library lib;
	Stroker stroker;
	Face face;
	~FontWrapper_imp() = default;
	FontWrapper_imp()
		: fio(nullptr),lib(nullptr), stroker(nullptr), face(nullptr)
	{
		;
	}
	FontWrapper_imp(const char* path)
		: fio(nullptr),lib(initializeLibrary()), stroker(createStroker(lib)), face(openFace(lib,path))
	{
		;
	}
	FontWrapper_imp(const Abstract::sFIO& fio)
		: fio(fio),lib(initializeLibrary()), stroker(createStroker(lib)), face(openFace(lib,createStream(fio.get())))
	{
		;
	}
	FontWrapper_imp(FontWrapper_imp&& mov)
		: fio(mov.fio), lib(mov.lib), stroker(mov.stroker), face(mov.face)
	{
		mov.fio = nullptr;
		mov.lib = nullptr;
		mov.stroker = nullptr;
		mov.face = nullptr;
	}
	void operator=(FontWrapper_imp&& mov)
	{
		this->fio = mov.fio;
		this->lib = mov.lib;
		this->stroker = mov.stroker;
		this->face = mov.face;
		mov.fio = nullptr;
		mov.lib = nullptr;
		mov.stroker = nullptr;
		mov.face = nullptr;
	}
	FontWrapper_imp(const FontWrapper_imp& cpy) = delete; // No copy-constructor
	void operator=(const FontWrapper_imp& cpy) = delete; // No copy-constructor
	long getGlyphCount() const
	{
		if(face) return face->num_glyphs;
		else return 0;
	}
	const char* getFamilyName() const
	{
		if(face) return face->family_name;
		else return nullptr;
	}
	const char* getStyleName() const
	{
		if(face) return face->style_name;
		else return nullptr;
	}
	tcb::span<FT_Bitmap_Size> getBitmapSizes() const
	{
		if(face) return tcb::span<FT_Bitmap_Size>(face->available_sizes,face->num_fixed_sizes);
		else return tcb::span<FT_Bitmap_Size>();
	}
	tcb::span<FT_CharMap> getCharmaps() const
	{
		if(face) return tcb::span<FT_CharMap>(face->charmaps,face->num_charmaps);
		else return tcb::span<FT_CharMap>();
	}
	void setCharSize(signed long char_width, signed long char_height,
					 unsigned int horz_resolution, unsigned int vert_resolution) const
	{
		if(face) FT_Set_Char_Size(face.get(),char_width, char_height, horz_resolution, vert_resolution);
	}
	unsigned int getGlyphIndex(unsigned long charcode) const
	{
		if(face) return FT_Get_Char_Index(face.get(),charcode);
		else return 0;
	}
	void loadGlyph(unsigned int glyph_index) const
	{
		if(face) FT_Load_Glyph(face.get(), glyph_index, FT_LOAD_RENDER);
	}
	void loadChar(unsigned long charcode) const
	{
		if(face) FT_Load_Char(face.get(), charcode, FT_LOAD_RENDER);
	}
	FT_GlyphSlot getGlyph() const
	{
		if(face) return face->glyph;
		else return nullptr;
	}
};

FontWrapper::FontWrapper()
{

}

}
}
