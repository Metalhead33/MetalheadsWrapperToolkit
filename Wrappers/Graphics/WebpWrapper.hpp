#ifndef MHWEBPWRAPPER_HPP
#define MHWEBPWRAPPER_HPP
#include "../../Io/FIO.hpp"
#include <cstdint>
#include <vector>
namespace MH33 {
namespace GFX {

enum class WEBP_IMGFORMAT : uint8_t {
	RGBA,
	ARGB,
	BGRA,
	RGB,
	BGR,
	// ANIMATION ONLY
	RGB_565,
	RGBA_4444
};

struct DecodedWebp {
	struct Frame {
		std::vector<uint8_t> pixels;
		int timestamp;
	};
	unsigned width,height;
	std::vector<Frame> frames;
	WEBP_IMGFORMAT format;
};

class WebpWrapper {
public:
	DEFINE_STRUCT(WebpWrapper_imp)
private:
	uWebpWrapper_imp pimpl;
	WebpWrapper(const WebpWrapper& cpy) = delete;
	void operator=(const WebpWrapper& cpy) = delete;
public:
	WebpWrapper(const Abstract::sFIO& nio);
	WebpWrapper(Abstract::sFIO&& nio);
	WebpWrapper(WebpWrapper&& mov);
	void operator=(WebpWrapper&& mov);
	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned getImageCount() const;
};


}
}
#endif // MHWEBPWRAPPER_HPP
