#ifndef MONOIMAGE_HPP
#define MONOIMAGE_HPP
#include <memory>
#include <string>
extern "C" {
#include <mono/metadata/image.h>
}

namespace MH33 {
namespace Script {
namespace Mono {

class Image {
private:
	std::unique_ptr<MonoImage,decltype (&mono_image_close)> image;
	MonoImageOpenStatus state;
	void setRegStuff();
	std::string name,filename,guid;
	// No copying
	Image(const Image& cpy) = delete;
	void operator=(const Image& cpy) = delete;
public:
	// Move semantics
	Image(Image&& mov);
	void operator=(Image&& mov);
	// Wrappers around mono_image_open
	Image(const char *fname);
	Image(const std::string& fname);
	// Wrappers around mono_image_open_full
	Image(const char *fname, bool refonly);
	Image(const std::string& fname, bool refonly);
	// Wrappers around mono_pe_file_open
	Image(bool pe, const char *fname);
	Image(bool pe, const std::string& fname);
	// Wrappers around mono_image_open_from_data
	Image(void *data, uint32_t data_len, bool need_copy);
	// Wrappers around mono_image_open_from_data_full
	Image(void *data, uint32_t data_len, bool need_copy, bool refonly);
	// Wrappers around mono_image_open_from_data_with_name
	Image(void *data, uint32_t data_len, bool need_copy, bool refonly, const char *name);
	Image(void *data, uint32_t data_len, bool need_copy, bool refonly, const std::string& name);
	// Other stuff
	void init(); // mono_image_init
	void addref(); // mono_image_addref
	int ensureSection(const char *section); // mono_image_ensure_section
	int ensureSection(const std::string& section); // mono_image_ensure_section
	int ensureSection(int idx); // mono_image_ensure_section_idx
	uint32_t getEntryPoint(); // mono_image_get_entry_point

	const std::string& getName() const;
	const std::string& getFilename() const;
	const std::string& getGuid() const;
	MonoImage* getRawImg() const;
};

}
}
}

#endif // MONOIMAGE_HPP
