#include "S3Wrapper.hpp"
#include <vector>
#include <cstring>
#define FOURCC_DXT1 0x31545844
#define FOURCC_DXT3 0x33545844
#define FOURCC_DXT5 0x35545844

namespace MH33 {
namespace GFX {

struct S3Wrapper::S3Wrapper_imp {
	// Fields
	struct Mipmap {
		uint32_t width, height;
		std::vector<uint8_t> bytes;
	};
	uint32_t linear_size;
	S3Type fourCC;
	std::vector<Mipmap> mipmaps;
	// Constructor
	S3Wrapper_imp()
		: linear_size(0), fourCC(S3Type::INVALID), mipmaps(1) // Total null
	{

	}
	S3Wrapper_imp(const S3Wrapper_imp& cpy)
		: linear_size(cpy.linear_size), fourCC(cpy.fourCC), mipmaps(cpy.mipmaps)
	{

	}
	S3Wrapper_imp(S3Wrapper_imp&& mov)
		:linear_size(mov.linear_size), fourCC(mov.fourCC), mipmaps(std::move(mov.mipmaps))
	{

	}
	void operator=(const S3Wrapper_imp& cpy)
	{
		this->linear_size = cpy.linear_size;
		this->fourCC = cpy.fourCC;
		this->mipmaps = cpy.mipmaps;
	}
	void operator=(S3Wrapper_imp&& mov)
	{
		this->linear_size = mov.linear_size;
		this->fourCC = mov.fourCC;
		this->mipmaps = std::move(mov.mipmaps);
	}
	S3Wrapper_imp(const Abstract::sFIO& reader)
	{
		std::vector<uint8_t> header(124);
		std::vector<char> mword(4);
		reader->read(mword.data(),4);
		if (strncmp(mword.data() , "DDS ", 4) != 0)
		{
			return;
		}
		reader->read(header.data(),124);
		uint32_t height = *reinterpret_cast<uint32_t*>(&header[8]);
		uint32_t width = *reinterpret_cast<uint32_t*>(&header[12]);
		this->linear_size = *reinterpret_cast<uint32_t*>(&header[16]);
		const uint32_t mipMapCount = *reinterpret_cast<uint32_t*>(&header[24]);
		uint32_t tmpFourCC = *reinterpret_cast<uint32_t*>(&header[80]);
		switch(tmpFourCC)
		{
		case FOURCC_DXT1:
			this->fourCC = S3Type::DDX1;
			break;
		case FOURCC_DXT3:
			this->fourCC = S3Type::DDX3;
			break;
		case FOURCC_DXT5:
			this->fourCC = S3Type::DDX5;
			break;
		default:
			return;
		}
		// Okay, now we will create the buffer.
		const uint32_t buffsize = mipMapCount > 1 ? linear_size * 2 : linear_size;
		std::vector<uint8_t> buffer(buffsize);
		reader->read(buffer.data(),buffsize);
		const uint32_t blockSize = (tmpFourCC == FOURCC_DXT1) ? 8 : 16;
		uint32_t offset = 0;
		mipmaps.resize(mipMapCount);
		for(Mipmap& x : mipmaps) {
			const unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
			x.width = width;
			x.height = height;
			x.bytes.resize(size);
			memcpy(x.bytes.data(),buffer.data() + offset,size);
			offset += size;
			width  /= 2;
			height /= 2;
		}
	}
	unsigned getWidth(unsigned level) const { return mipmaps[level].width; }
	unsigned getHeight(unsigned level) const { return mipmaps[level].height; }
	size_t getMipmapCount() const { return mipmaps.size(); }
	void* getData(unsigned level) { return mipmaps[level].bytes.data(); }
	const void* getData(unsigned level) const { return mipmaps[level].bytes.data(); }
};

void S3Wrapper::operator=(S3Wrapper &&mov)
{
	this->pimpl = std::move(mov.pimpl);
}

S3Wrapper::S3Wrapper(const Abstract::sFIO &nio)
	: pimpl(new S3Wrapper_imp(nio))
{

}

S3Wrapper::S3Wrapper(S3Wrapper &&mov)
	: pimpl(std::move(mov.pimpl))
{

}

unsigned S3Wrapper::getWidth(unsigned level) const
{
	return pimpl->getWidth(level);
}

unsigned S3Wrapper::getHeight(unsigned level) const
{
	return pimpl->getHeight(level);
}

size_t S3Wrapper::getMipmapCount() const
{
	return pimpl->getMipmapCount();
}

void *S3Wrapper::getData(unsigned level)
{
	return pimpl->getData(level);
}

const void *S3Wrapper::getData(unsigned level) const
{
	return pimpl->getData(level);
}

S3Type S3Wrapper::getS3Type() const
{
	return pimpl->fourCC;
}

}
}
