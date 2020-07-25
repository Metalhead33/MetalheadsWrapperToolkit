#include "MonoContext.hpp"

namespace MH33 {
namespace Script {
namespace Mono {

Context::Context(const char* domain_name)
	: domain(mono_jit_init(domain_name),mono_jit_cleanup)
{

}
Context::Context(const std::string& domain_name)
	: domain(mono_jit_init(domain_name.c_str()),mono_jit_cleanup)
{

}
Context::Context(const char* domain_name, const char* version_name)
	: domain(mono_jit_init_version(domain_name,version_name),mono_jit_cleanup)
{

}
Context::Context(const std::string& domain_name, const std::string& version_name)
	: domain(mono_jit_init_version(domain_name.c_str(),version_name.c_str()),mono_jit_cleanup)
{

}

MonoDomain *Context::getDomain() const
{
	return domain.get();
}

}
}
}
