#pragma once
#include <cstdint>
#include "Global.hpp"
#include <vector>

namespace Abstract {

typedef std::vector<uint8_t> byteBuffer;
typedef std::vector<std::string> stringBuffer;
DEFINE_CLASS(FIO)
class FIO {
public:
	virtual ~FIO() = default;
	virtual int64_t read(void* data, int64_t size) = 0;
	virtual int64_t seek(int64_t position) = 0;
	virtual int64_t tell() = 0;
	virtual int64_t size() = 0;
	virtual int64_t write(const void* data, int64_t size) = 0;
	virtual char getc() = 0;
	virtual byteBuffer loadIntoBuffer() = 0;
	virtual std::string stringize() = 0;
private:

};

}
