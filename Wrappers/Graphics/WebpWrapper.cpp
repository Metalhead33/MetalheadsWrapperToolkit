#include "WebpWrapper.hpp"
#include <cstring>
#include <webp/decode.h>
#include <webp/demux.h>

namespace MH33 {
namespace GFX {

struct WebpWrapper::WebpWrapper_imp {
	virtual ~WebpWrapper_imp();
	virtual unsigned getWidth() const = 0;
	virtual unsigned getHeight() const = 0;
	virtual size_t getImageCount() const = 0;
	virtual bool isAnimated() const = 0;
	virtual void decodeTo(DecodedWebp& target) const = 0;
};
WebpWrapper::WebpWrapper_imp::~WebpWrapper_imp() = default;

struct WEBP_DECODE : public WebpWrapper::WebpWrapper_imp {
	std::vector<uint8_t> imageData;
	int width, height;
	WEBP_DECODE(const Abstract::sFIO& reader) {
		imageData.resize(reader->size());
		reader->read(imageData.data(),imageData.size());
		if(!WebPGetInfo(imageData.data(),imageData.size(),&width,&height)) return;
	}
	WEBP_DECODE(const std::vector<uint8_t>& cpy)
		: imageData(cpy)
	{
		if(!WebPGetInfo(imageData.data(),imageData.size(),&width,&height)) return;
	}
	WEBP_DECODE(std::vector<uint8_t>&& mov)
		: imageData(std::move(mov))
	{
		if(!WebPGetInfo(imageData.data(),imageData.size(),&width,&height)) return;
	}
	WEBP_DECODE(const WEBP_DECODE& cpy)
		: imageData(cpy.imageData), width(cpy.width), height(cpy.height)
	{

	}
	WEBP_DECODE(WEBP_DECODE&& mov)
		: imageData(std::move(mov.imageData)), width(mov.width), height(mov.height)
	{

	}
	void operator=(const std::vector<uint8_t>& cpy)
	{
		this->imageData = cpy;
		if(!WebPGetInfo(imageData.data(),imageData.size(),&width,&height)) return;
	}
	void operator=(std::vector<uint8_t>&& mov)
	{
		this->imageData = std::move(mov);
		if(!WebPGetInfo(imageData.data(),imageData.size(),&width,&height)) return;
	}
	void operator=(const WEBP_DECODE& cpy)
	{
		this->imageData = cpy.imageData;
		this->width = cpy.width;
		this->height = cpy.height;
	}
	void operator=(WEBP_DECODE&& mov)
	{
		this->imageData = std::move(mov.imageData);
		this->width = mov.width;
		this->height = mov.height;
	}

	bool decodeRGBA(uint8_t* output_buffer, size_t output_buffer_size, int output_stride) const
	{
		return WebPDecodeRGBAInto(imageData.data(),imageData.size(),output_buffer,output_buffer_size,output_stride) != nullptr;
	}
	bool decodeARGB(uint8_t* output_buffer, size_t output_buffer_size, int output_stride) const
	{
		return WebPDecodeARGBInto(imageData.data(),imageData.size(),output_buffer,output_buffer_size,output_stride) != nullptr;
	}
	bool decodeBGRA(uint8_t* output_buffer, size_t output_buffer_size, int output_stride) const
	{
		return WebPDecodeBGRAInto(imageData.data(),imageData.size(),output_buffer,output_buffer_size,output_stride) != nullptr;
	}
	bool decodeRGB(uint8_t* output_buffer, size_t output_buffer_size, int output_stride) const
	{
		return WebPDecodeRGBInto(imageData.data(),imageData.size(),output_buffer,output_buffer_size,output_stride) != nullptr;
	}
	bool decodeBGR(uint8_t* output_buffer, size_t output_buffer_size, int output_stride) const
	{
		return WebPDecodeBGRInto(imageData.data(),imageData.size(),output_buffer,output_buffer_size,output_stride) != nullptr;
	}
	unsigned getWidth() const;
	unsigned getHeight() const;
	size_t getImageCount() const;
	bool isAnimated() const;
	void decodeTo(DecodedWebp& target) const;
};


unsigned WEBP_DECODE::getWidth() const { return width; }
unsigned WEBP_DECODE::getHeight() const { return height; }
size_t WEBP_DECODE::getImageCount() const { return 1; }
bool WEBP_DECODE::isAnimated() const { return false; }
void WEBP_DECODE::decodeTo(DecodedWebp& target) const
{
	target.width = width;
	target.height = height;
	target.frames.resize(1);
	switch(target.format)
	{
	case WEBP_IMGFORMAT::RGBA:
	{
		target.frames[0].pixels.resize(4*width*height);
		decodeRGBA(target.frames[0].pixels.data(),target.frames[0].pixels.size(),width*4);
		break;
	}
	case WEBP_IMGFORMAT::ARGB:
	{
		target.frames[0].pixels.resize(4*width*height);
		decodeRGBA(target.frames[0].pixels.data(),target.frames[0].pixels.size(),width*4);
		break;
	}
	case WEBP_IMGFORMAT::BGRA:
	{
		target.frames[0].pixels.resize(4*width*height);
		decodeRGBA(target.frames[0].pixels.data(),target.frames[0].pixels.size(),width*4);
		break;
	}
	case WEBP_IMGFORMAT::RGB:
	{
		target.frames[0].pixels.resize(4*width*height);
		decodeRGBA(target.frames[0].pixels.data(),target.frames[0].pixels.size(),width*3);
		break;
	}
	case WEBP_IMGFORMAT::BGR:
	{
		target.frames[0].pixels.resize(4*width*height);
		decodeRGBA(target.frames[0].pixels.data(),target.frames[0].pixels.size(),width*3);
		break;
	}
	default: break;
	}
}

struct WEBP_DEMUX : public WebpWrapper::WebpWrapper_imp {
	typedef std::unique_ptr<WebPDemuxer,decltype (&WebPDemuxDelete)> Configuration;
	std::vector<uint8_t> imageData;
	WebPData riff;
	Configuration config;
	uint32_t width, height, flags, frameCount;

	void getBasicInformation()
	{
		width = WebPDemuxGetI(config.get(), WEBP_FF_CANVAS_WIDTH);
		height = WebPDemuxGetI(config.get(), WEBP_FF_CANVAS_HEIGHT);
		flags = WebPDemuxGetI(config.get(), WEBP_FF_FORMAT_FLAGS);
		frameCount = WebPDemuxGetI(config.get(), WEBP_FF_FRAME_COUNT);
	}
	WEBP_DEMUX(const Abstract::sFIO& reader)
		: imageData(0), config(nullptr,WebPDemuxDelete)
	{
		imageData.resize(reader->size());
		reader->read(imageData.data(),imageData.size());
		riff.bytes = imageData.data();
		riff.size = imageData.size();
		config = Configuration(WebPDemux(&riff),WebPDemuxDelete);
		getBasicInformation();
	}
	WEBP_DEMUX(const std::vector<uint8_t>& buff)
		: imageData(buff), config(nullptr,WebPDemuxDelete)
	{
		riff.bytes = imageData.data();
		riff.size = imageData.size();
		config = Configuration(WebPDemux(&riff),WebPDemuxDelete);
		getBasicInformation();
	}
	WEBP_DEMUX(std::vector<uint8_t>&& buff)
		: imageData(std::move(buff)), config(nullptr,WebPDemuxDelete)
	{
		riff.bytes = imageData.data();
		riff.size = imageData.size();
		config = Configuration(WebPDemux(&riff),WebPDemuxDelete);
		getBasicInformation();
	}
	unsigned getWidth() const;
	unsigned getHeight() const;
	size_t getImageCount() const;
	bool isAnimated() const;
	void decodeTo(DecodedWebp& target) const;
};
unsigned WEBP_DEMUX::getWidth() const { return width; }
unsigned WEBP_DEMUX::getHeight() const { return height; }
size_t WEBP_DEMUX::getImageCount() const { return frameCount; }
bool WEBP_DEMUX::isAnimated() const { return true; }
void WEBP_DEMUX::decodeTo(DecodedWebp& target) const
{
	target.width = width;
	target.height = height;
	int bytesPerPixel = 0;
	WebPAnimDecoderOptions dec_options;
	WebPAnimDecoderOptionsInit(&dec_options);
	switch(target.format)
	{
	case WEBP_IMGFORMAT::RGBA:
	{
		dec_options.color_mode = MODE_RGBA;
		bytesPerPixel = 4;
		break;
	}
	case WEBP_IMGFORMAT::ARGB:
	{
		dec_options.color_mode = MODE_ARGB;
		bytesPerPixel = 4;
		break;
	}
	case WEBP_IMGFORMAT::BGRA:
	{
		dec_options.color_mode = MODE_BGRA;
		bytesPerPixel = 4;
		break;
	}
	case WEBP_IMGFORMAT::RGB:
	{
		dec_options.color_mode = MODE_RGB;
		bytesPerPixel = 4;
		break;
	}
	case WEBP_IMGFORMAT::BGR:
	{
		dec_options.color_mode = MODE_BGR;
		bytesPerPixel = 4;
		break;
	}
	case WEBP_IMGFORMAT::RGB_565:
	{
		dec_options.color_mode = MODE_RGB_565;
		bytesPerPixel = 2;
		break;
	}
	case WEBP_IMGFORMAT::RGBA_4444:
	{
		dec_options.color_mode = MODE_RGBA_4444;
		bytesPerPixel = 2;
		break;
	}
	}
	int framesSoFar = 0;
	target.frames.resize(frameCount);
	std::unique_ptr<WebPAnimDecoder,decltype (&WebPAnimDecoderDelete)> decoder(WebPAnimDecoderNew(&riff,&dec_options),WebPAnimDecoderDelete);
	WebPAnimInfo anim_info;
	WebPAnimDecoderGetInfo(decoder.get(), &anim_info);
	for (uint32_t i = 0; i < anim_info.loop_count; ++i) {
		while (WebPAnimDecoderHasMoreFrames(decoder.get())) {
			uint8_t* buf;
			int timestamp;
			WebPAnimDecoderGetNext(decoder.get(), &buf, &timestamp);
			target.frames[framesSoFar].timestamp = timestamp;
			target.frames[framesSoFar].pixels.resize(width*height*bytesPerPixel);
			memcpy(target.frames[framesSoFar].pixels.data(),buf,width*height*bytesPerPixel);
			++framesSoFar;
		}
	}
}

}
}
