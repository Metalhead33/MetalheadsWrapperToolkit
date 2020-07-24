#ifndef MHGIFWRAPPER_HPP
#define MHGIFWRAPPER_HPP
#include "../../Io/FIO.hpp"
#include <vector>
#include <cstdint>
namespace MH33 {
namespace GFX {

struct DecodedAnimation {
	struct PixelRgb {
		uint8_t r,g,b;
	};
	struct PixelRgba {
		uint8_t r,g,b,a;
	};
	union GifData {
		std::vector<std::vector<PixelRgb>> variant24bit;
		std::vector<std::vector<PixelRgba>> variant32bit;
		~GifData() {}
	} data;
	unsigned width,height;
	float delayTime;
	bool hasTransparency;
};

class GifWrapper
{
private:
	DEFINE_STRUCT(GifWrapper_imp)
	uGifWrapper_imp pimpl;
	GifWrapper(const GifWrapper& cpy) = delete;
	void operator=(const GifWrapper& cpy) = delete;
public:
	GifWrapper(const Abstract::sFIO& nio);
	GifWrapper(Abstract::sFIO&& nio);
	GifWrapper(GifWrapper&& mov);
	void operator=(GifWrapper&& mov);
	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned getImageCount() const;
	int getPaletteColourCount() const;
	void decode(DecodedAnimation& target);
	bool hasTransparency() const;
	float getDelayTime() const;
};

}
}
#endif // MHGIFWRAPPER_HPP
