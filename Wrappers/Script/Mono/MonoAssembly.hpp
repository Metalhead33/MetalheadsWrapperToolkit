#ifndef MONOASSEMBLY_HPP
#define MONOASSEMBLY_HPP
#include "MonoContext.hpp"
extern "C" {
#include <mono/metadata/assembly.h>
}

namespace MH33 {
namespace Script {
namespace Mono {

DEFINE_CLASS(Assembly)
class Assembly
{
private:
	sContext context;
	std::unique_ptr<MonoAssembly,decltype (&mono_assembly_close)> assembly;
public:
	// Some public static stuff
	static void parseConfig();
	static void parseConfig(const char* path);
	static void parseConfig(const std::string& path);
	// Wrapping around mono_domain_assembly_open
	Assembly(const sContext& ncontext, const char* executablePath);
	Assembly(const sContext& ncontext, const std::string& executablePath);
	Assembly(sContext&& ncontext, const char* executablePath);
	Assembly(sContext&& ncontext, const std::string& executablePath);
	// Kaboom
	int execute(int argc, char** argv);
};

}
}
}
#endif // MONOASSEMBLY_HPP
