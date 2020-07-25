#ifndef MONOCONTEXT_HPP
#define MONOCONTEXT_HPP
#include "../../../Io/Global.hpp"
extern "C" {
#include <mono/jit/jit.h>
}
#include <memory>
#include <string>

namespace MH33 {
namespace Script {
namespace Mono {

DEFINE_CLASS(Context)
class Context
{
private:
	std::unique_ptr<MonoDomain, decltype (&mono_jit_cleanup)> domain;
public:
	Context(const char* domain_name);
	Context(const std::string& domain_name);
	Context(const char* domain_name, const char* version_name);
	Context(const std::string& domain_name, const std::string& version_name);
	MonoDomain* getDomain() const;
};

}
}
}
#endif // MONOCONTEXT_HPP
