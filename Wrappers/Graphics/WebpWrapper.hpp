#ifndef MHWEBPWRAPPER_HPP
#define MHWEBPWRAPPER_HPP
#include "../../Abstract/Io/FIO.hpp"
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
	WebpWrapper(const Abstract::sFIO& nio, bool demux = true);
	WebpWrapper(const std::vector<uint8_t>& nio, bool demux = true);
	WebpWrapper(std::vector<uint8_t>&& nio, bool demux = true);
	WebpWrapper(WebpWrapper&& mov);
	void operator=(WebpWrapper&& mov);
	unsigned getWidth() const;
	unsigned getHeight() const;
	size_t getImageCount() const;
	bool isAnimated() const;
	void decodeTo(DecodedWebp& target) const;
};


}
}
#endif // MHWEBPWRAPPER_HPP
