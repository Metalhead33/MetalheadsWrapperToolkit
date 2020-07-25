#include "MonoImage.hpp"
namespace MH33 {
namespace Script {
namespace Mono {

// Wrappers around mono_image_open
void Image::setRegStuff()
{
	if(image)
	{
		if(mono_image_get_name(image.get())) name = mono_image_get_name(image.get());
		if(mono_image_get_filename(image.get())) filename = mono_image_get_filename(image.get());
		if(mono_image_get_guid(image.get())) guid = mono_image_get_guid(image.get());
	}
}

Image::Image(Image &&mov)
	: image(std::move(mov.image)), name(std::move(mov.name)),
	  filename(std::move(mov.filename)), guid(std::move(mov.guid))

{

}

void Image::operator=(Image &&mov)
{
	this->image = std::move(mov.image);
	this->name = std::move(mov.name);
	this->filename = std::move(mov.filename);
	this->guid = std::move(mov.guid);
}

const std::string &Image::getGuid() const
{
	return guid;
}

MonoImage *Image::getRawImg() const
{
	return image.get();
}

const std::string &Image::getFilename() const
{
	return filename;
}

const std::string &Image::getName() const
{
	return name;
}

Image::Image(const char *fname)
	: image(mono_image_open(fname,&state),mono_image_close)
{
	setRegStuff();
}
Image::Image(const std::string& fname)
	: image(mono_image_open(fname.c_str(),&state),mono_image_close)
{
	setRegStuff();
}
// Wrappers around mono_image_open_full
Image::Image(const char *fname, bool refonly)
	: image(mono_image_open_full(fname,&state,refonly),mono_image_close)
{
	setRegStuff();
}
Image::Image(const std::string& fname, bool refonly)
	: image(mono_image_open_full(fname.c_str(),&state,refonly),mono_image_close)
{
	setRegStuff();
}
// Wrappers around mono_pe_file_open
Image::Image(bool pe, const char *fname)
	: image(mono_pe_file_open(fname,&state),mono_image_close)
{
	(void)pe;
	setRegStuff();
}
Image::Image(bool pe, const std::string& fname)
	: image(mono_pe_file_open(fname.c_str(),&state),mono_image_close)
{
	(void)pe;
	setRegStuff();
}
// Wrappers around mono_image_open_from_data
Image::Image(void *data, uint32_t data_len, bool need_copy)
	: image(mono_image_open_from_data(reinterpret_cast<char*>(data),data_len,need_copy,&state),mono_image_close)
{
	setRegStuff();
}
// Wrappers around mono_image_open_from_data_full
Image::Image(void *data, uint32_t data_len, bool need_copy, bool refonly)
	: image(mono_image_open_from_data_full(reinterpret_cast<char*>(data),data_len,need_copy,&state,refonly),mono_image_close)
{
	setRegStuff();
}
// Wrappers around mono_image_open_from_data_with_name
Image::Image(void *data, uint32_t data_len, bool need_copy, bool refonly, const char *name)
	: image(mono_image_open_from_data_with_name(reinterpret_cast<char*>(data),data_len,need_copy,&state,refonly,name),mono_image_close)
{
	setRegStuff();
}
Image::Image(void *data, uint32_t data_len, bool need_copy, bool refonly, const std::string& name)
	: image(mono_image_open_from_data_with_name(reinterpret_cast<char*>(data),data_len,need_copy,&state,refonly,name.c_str()),mono_image_close)
{
	setRegStuff();
}

}
}
}
