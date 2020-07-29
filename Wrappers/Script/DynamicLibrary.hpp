#ifndef DYNAMICLIBRARY_HPP
#define DYNAMICLIBRARY_HPP
#include <memory>
#include <string>
extern "C" {
#ifdef _WIN32
#include <windows.h>
#define OPEN_LIB(X) LoadLibrary(TEXT(X))
#define CLOSE_LIB FreeLibrary
#define GET_LIB_FUNC GetProcAddress
#elif __unix__
#include <dlfcn.h>
#define OPEN_LIB(X) dlopen(X,RTLD_NOW|RTLD_GLOBAL)
#define CLOSE_LIB dlclose
#define GET_LIB_FUNC dlsym
#else
#error "OS not supported!"
#endif
#if defined(_MSC_VER)
//  Microsoft
	#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
//  GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
//  do nothing and hope for the best?
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif
}
namespace MH33 {
namespace Script {

class DynamicLibrary
{
public:
	typedef std::unique_ptr<void,decltype(&CLOSE_LIB)> Library;
private:
	Library libPriv;
	DynamicLibrary(const DynamicLibrary& cpy) = delete; // Copy constructor
	void operator=(const DynamicLibrary& cpy) = delete; // No copy assignment
public:
	DynamicLibrary();
	DynamicLibrary(const char* path);
	DynamicLibrary(const std::string& path);
	DynamicLibrary(DynamicLibrary&& mov); // Move constructor
	void operator=(DynamicLibrary&& mov); // Move assignment
	void* getFunction(const char* funcName) const;
	void* getFunction(const std::string& path) const;
	bool isValid() const;
};

}
}
#endif // DYNAMICLIBRARY_HPP
