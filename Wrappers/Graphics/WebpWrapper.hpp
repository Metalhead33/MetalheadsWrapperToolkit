#ifndef MHWEBPWRAPPER_HPP
#define MHWEBPWRAPPER_HPP
#include "../../Io/FIO.hpp"
namespace MH33 {
namespace GFX {

class WebpWrapper {
private:
	DEFINE_STRUCT(WebpWrapper_imp)
	uWebpWrapper_imp pimpl;
	WebpWrapper(const WebpWrapper& cpy) = delete;
	void operator=(const WebpWrapper& cpy) = delete;
public:
	WebpWrapper(const Abstract::sFIO& nio);
	WebpWrapper(Abstract::sFIO&& nio);
	WebpWrapper(WebpWrapper&& mov);
	void operator=(WebpWrapper&& mov);
	unsigned getWidth() const;
	unsigned getHeight() const;
	unsigned getImageCount() const;
};


}
}
#endif // MHWEBPWRAPPER_HPP
