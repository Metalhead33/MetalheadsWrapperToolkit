#include "DynamicLibrary.hpp"
namespace MH33 {
namespace Script {


DynamicLibrary::DynamicLibrary()
	: libPriv(nullptr,nullptr)
{

}

DynamicLibrary::DynamicLibrary(const char *path)
	: libPriv(OPEN_LIB(path),CLOSE_LIB)
{

}

DynamicLibrary::DynamicLibrary(const std::string &path)
	: libPriv(OPEN_LIB(path.c_str()),CLOSE_LIB)
{

}

DynamicLibrary::DynamicLibrary(DynamicLibrary &&mov)
	: libPriv(std::move(mov.libPriv))
{

}

void DynamicLibrary::operator=(DynamicLibrary &&mov)
{
	libPriv = std::move(mov.libPriv);
}

void *DynamicLibrary::getFunction(const char *funcName) const
{
	return GET_LIB_FUNC(libPriv.get(),funcName);
}

void *DynamicLibrary::getFunction(const std::string &path) const
{
	return getFunction(path.c_str());
}

bool DynamicLibrary::isValid() const
{
	return libPriv != nullptr;
}
}
}
