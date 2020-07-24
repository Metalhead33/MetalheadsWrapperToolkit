#include "GifWrapper.hpp"
#include <gif_lib.h>

namespace MH33 {
namespace GFX {

struct GifWrapper::GifWrapper_imp {
	static int LATEST_ERROR;
	// Typedefs and static functions
	static void CLOSE_GIF(GifFileType* ptr) { DGifCloseFile(ptr,&LATEST_ERROR); }
	static int GIF_INPUT(GifFileType* container, GifByteType* buffer, int size)
	{
		if(!container) return -1;
		Abstract::pFIO filehandle = reinterpret_cast<Abstract::pFIO>(container->UserData);
		if(!filehandle) return -1;
		return int(filehandle->read(buffer,size));
	}
	typedef std::unique_ptr<GifFileType, decltype (&CLOSE_GIF)> HandleType;
	// Fields
	Abstract::sFIO fileIO;
	HandleType handle;
	int transparency;
	float delayTime;
	// Constructor helpers
	void checkForTransparency()
	{
		int tmpFramerate = 0;
		for(int i = 0; i < handle->ExtensionBlockCount; ++i)
		{
			GraphicsControlBlock block;
			if (DGifExtensionToGCB(handle->ExtensionBlocks[i].ByteCount,handle->ExtensionBlocks[i].Bytes,
								   &block)) {
				if(block.TransparentColor > transparency) transparency = block.TransparentColor;
				if(block.DelayTime > tmpFramerate) tmpFramerate = block.DelayTime;
			}
		}
		delayTime = float(tmpFramerate) * 0.01f;
	}
	// Constructors
	GifWrapper_imp()
		: fileIO(nullptr), handle(nullptr,CLOSE_GIF), transparency(-1), delayTime(0)
	{

	}
	GifWrapper_imp(const Abstract::sFIO& nio)
		: fileIO(nio), handle(DGifOpen(fileIO.get(),&GIF_INPUT,&LATEST_ERROR),CLOSE_GIF), transparency(-1), delayTime(0)
	{
		DGifSlurp(handle.get());
		checkForTransparency();
	}
	GifWrapper_imp(Abstract::sFIO&& nio)
		: fileIO(std::move(nio)), handle(DGifOpen(fileIO.get(),&GIF_INPUT,&LATEST_ERROR),CLOSE_GIF), transparency(-1), delayTime(0)
	{
		DGifSlurp(handle.get());
		checkForTransparency();
	}
	GifWrapper_imp(const GifWrapper_imp& cpy) = delete;
	void operator=(const GifWrapper_imp& cpy) = delete;
	GifWrapper_imp(GifWrapper_imp&& mov)
		: fileIO(std::move(mov.fileIO)), handle(std::move(mov.handle)), transparency(mov.transparency), delayTime(mov.delayTime)
	{

	}
	void operator=(GifWrapper_imp&& mov)
	{
		this->handle = std::move(mov.handle);
		this->fileIO = std::move(mov.fileIO);
		this->transparency = mov.transparency;
		this->delayTime = mov.delayTime;
	}
	GifWord getWidth() const
	{
		return handle->SWidth;
	}
	GifWord getHeight() const
	{
		return handle->SHeight;
	}
	int getImageCount() const
	{
		return handle->ImageCount;
	}
	int getPaletteColourCount() const
	{
		return handle->SColorMap->ColorCount;
	}
	int getPaletteBPS() const
	{
		return handle->SColorMap->BitsPerPixel;
	}
	GifColorType* getPaletteColours()
	{
		return handle->SColorMap->Colors;
	}
	const GifColorType* getPaletteColours() const
	{
		return handle->SColorMap->Colors;
	}
	SavedImage* getImages()
	{
		return handle->SavedImages;
	}
	const SavedImage* getImages() const
	{
		return handle->SavedImages;
	}
	void decode(DecodedAnimation& target)
	{
		ColorMapObject *gifcolormap = handle->SColorMap;
		if(!gifcolormap) return;
		GifColorType *gifpalette = gifcolormap->Colors;
		if(!gifpalette) return;
		target.width = handle->SWidth;
		target.height = handle->SHeight;
		target.delayTime = delayTime;
		if(transparency >= 0)
		{
			target.data.variant32bit.resize(handle->ImageCount);
			for(int i = 0; i < handle->ImageCount; ++i) {
				const SavedImage* img = &(handle->SavedImages[i]);
				const int newSize = img->ImageDesc.Height * img->ImageDesc.Width;
				auto& targetX = target.data.variant32bit[i];
				targetX.resize(newSize);
				for(int x = 0; x < newSize;++x) {
					auto& targetY = targetX[x];
					if(img->RasterBits[x] == transparency) {
						targetY.a = 0;
					} else {
						targetY.a = 255;
						targetY.r = gifpalette[img->RasterBits[x]].Red;
						targetY.g = gifpalette[img->RasterBits[x]].Green;
						targetY.b = gifpalette[img->RasterBits[x]].Blue;
					}
				}
			}
		} else {
			target.data.variant24bit.resize(handle->ImageCount);
			for(int i = 0; i < handle->ImageCount; ++i) {
				const SavedImage* img = &(handle->SavedImages[i]);
				const int newSize = img->ImageDesc.Height * img->ImageDesc.Width;
				auto& targetX = target.data.variant24bit[i];
				targetX.resize(newSize);
				for(int x = 0; x < newSize;++x) {
					auto& targetY = targetX[x];
					targetY.r = gifpalette[img->RasterBits[x]].Red;
					targetY.g = gifpalette[img->RasterBits[x]].Green;
					targetY.b = gifpalette[img->RasterBits[x]].Blue;
				}
			}
		}
	}
};

GifWrapper::GifWrapper(const Abstract::sFIO &nio)
	: pimpl(new GifWrapper_imp(nio))
{

}
GifWrapper::GifWrapper(Abstract::sFIO &&nio)
	: pimpl(new GifWrapper_imp(std::move(nio)))
{

}

GifWrapper::GifWrapper(GifWrapper &&mov)
	: pimpl(std::move(mov.pimpl))
{

}

void GifWrapper::operator=(GifWrapper &&mov)
{
	this->pimpl = std::move(mov.pimpl);
}
unsigned GifWrapper::getWidth() const
{
	return pimpl->getWidth();
}
unsigned GifWrapper::getHeight() const
{
	return pimpl->getHeight();
}
unsigned GifWrapper::getImageCount() const
{
	return pimpl->getImageCount();
}
int GifWrapper::getPaletteColourCount() const
{
	return pimpl->getPaletteColourCount();
}
void GifWrapper::decode(DecodedAnimation& target)
{
	pimpl->decode(target);
}

bool GifWrapper::hasTransparency() const
{
	return pimpl->transparency >= 0;
}

float GifWrapper::getDelayTime() const
{
	return pimpl->delayTime;
}

}
}
