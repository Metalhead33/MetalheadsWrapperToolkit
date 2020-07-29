#ifndef UNIVERSALHANDLE_HPP
#define UNIVERSALHANDLE_HPP
#include <cstdint>
namespace Abstract {
namespace GFX {

union GAPI_HANDLE {
	// OpenGL
	uint32_t OpenGL_GLuint;
	int32_t OpenGL_GLint;
	uint32_t OpenGL_GLenum;
	// Vulkan
	void* Vulkan_Dispatchable;
#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__) ) || defined(_M_X64) || defined(__ia64) || defined (_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
	void* Vulkan_NonDispatchable;
#else
	uint64_t Vulkan_NonDispatchable;
#endif
	// Direct3D
	void* Direct3D_resource;
	// Other
	uint64_t misc_Index;
	void* misc_Ptr;
};

}
}
#endif // UNIVERSALHANDLE_HPP
