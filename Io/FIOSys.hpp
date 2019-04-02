#ifndef FIOSYS_HPP
#define FIOSYS_HPP
#include "FIO.hpp"

namespace Abstract {

DEFINE_CLASS(FIOSys)
class FIOSys {
public:
	virtual ~FIOSys() = default;
	virtual sFIO openRead(const std::string& path) const = 0;
	virtual sFIO openWrite(const std::string& path) const = 0;
	virtual bool exists(const std::string& path) const = 0;
	virtual byteBuffer loadFileIntoBuffer(const std::string &path) const = 0;
	virtual std::string stringizeFile(const std::string &path) const = 0;
};

}
#endif // FIOSYS_HPP
