#include "MonoAssembly.hpp"
extern "C" {
#include <mono/metadata/mono-config.h>
}

namespace MH33 {
namespace Script {
namespace Mono {

// Wrapping around mono_domain_assembly_open
void Assembly::parseConfig()
{
	mono_config_parse(nullptr);
}

void Assembly::parseConfig(const char *path)
{
	mono_config_parse(path);
}

void Assembly::parseConfig(const std::string &path)
{
	mono_config_parse(path.c_str());
}

Assembly::Assembly(const sContext& ncontext, const char* executablePath)
	: context(ncontext), assembly(mono_domain_assembly_open(context->getDomain(),executablePath),mono_assembly_close)
{

}
Assembly::Assembly(const sContext& ncontext, const std::string& executablePath)
	: context(ncontext), assembly(mono_domain_assembly_open(context->getDomain(),executablePath.c_str()),mono_assembly_close)
{

}
Assembly::Assembly(sContext&& ncontext, const char* executablePath)
	: context(std::move(ncontext)), assembly(mono_domain_assembly_open(context->getDomain(),executablePath),mono_assembly_close)
{

}
Assembly::Assembly(sContext&& ncontext, const std::string& executablePath)
	: context(std::move(ncontext)), assembly(mono_domain_assembly_open(context->getDomain(),executablePath.c_str()),mono_assembly_close)
{

}
int Assembly::execute(int argc, char **argv)
{
	return mono_jit_exec(context->getDomain(),assembly.get(),argc,argv);
}

}
}
}
