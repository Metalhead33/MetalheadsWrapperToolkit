#ifndef S3WRAPPER_HPP
#define S3WRAPPER_HPP
#include "../../Abstract/Io/FIO.hpp"
#include <cstdint>
namespace MH33 {
namespace GFX {

enum class S3Type : uint8_t {
	INVALID = 0,
	DDX1,
	DDX3,
	DDX5
};

class S3Wrapper {
private:
	DEFINE_STRUCT(S3Wrapper_imp)
	uS3Wrapper_imp pimpl;
	S3Wrapper(const S3Wrapper& cpy) = delete;
	void operator=(const S3Wrapper& cpy) = delete;
public:
	S3Wrapper(const Abstract::sFIO& nio);
	S3Wrapper(S3Wrapper&& mov);
	void operator=(S3Wrapper&& mov);
	unsigned getWidth(unsigned level=0) const;
	unsigned getHeight(unsigned level=0) const;
	size_t getMipmapCount() const;
	void* getData(unsigned level);
	const void* getData(unsigned level) const;
	S3Type getS3Type() const;
};

}
}
#endif // S3WRAPPER_HPP
