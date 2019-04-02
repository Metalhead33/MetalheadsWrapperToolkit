#include "MhResampler.hpp"
#include <cstring>
extern "C" {
#include <samplerate.h>
}
namespace Mh {

struct SimpleResampler::SimpleResamplerImpl {
	SRC_DATA data;
	~SimpleResamplerImpl() = default;
	SimpleResamplerImpl() {
		std::memset(&data,0,sizeof(SRC_DATA));
	}
	SimpleResamplerImpl(const SimpleResamplerImpl& cpy) {
		std::memcpy(&this->data,&cpy.data,sizeof(SRC_DATA));
	}
	void operator=(const SimpleResamplerImpl& cpy) {
		std::memcpy(&this->data,&cpy.data,sizeof(SRC_DATA));
	}
	SimpleResamplerImpl(float* dataIn, long input_frames, float* dataOut, long outputFrames, double src_ratio)
		: data{dataIn, dataOut, input_frames, 0, outputFrames, 0, 0, src_ratio}
	{
		;
	}
	int resample(ResampleType resamplerType, int channels) {
		return src_simple(&data,int(resamplerType),channels);
	}
	const float* getDataIn() const { return data.data_in; }
	const float* getDataOut() const { return data.data_out; }
	float* getDataOut() { return data.data_out; }
	long getInputFrames() const { return data.input_frames; }
	long getOutputFrames() const { return data.output_frames; }
	double getRatio() const { return data.src_ratio; }
	bool getEndOfInput() const { return bool(data.end_of_input); }
	void setDataIn(const float* set_to) { data.data_in = set_to; }
	void setDataOut(float* set_to) { data.data_out = set_to; }
	void setInputFrames(long set_to) { data.input_frames = set_to; }
	void setOutputFrames(long set_to) { data.output_frames = set_to; }
	void setRatio(double set_to) { data.src_ratio = set_to; }
	void setEndOfInput(bool set_to) { data.end_of_input = int(set_to); }
	void set(const float* data_in, float* data_out, long input_frames, long output_frames, double ratio)
	{
		std::memset(&data,0,sizeof(SRC_DATA));
		data.data_in = data_in;
		data.data_out = data_out;
		data.input_frames = input_frames;
		data.output_frames = output_frames;
		data.src_ratio = ratio;
	}

};
struct FullResampler::FullResamplerImpl {
	SRC_DATA data;
	int channels;
	ResampleType converterType;
	typedef std::shared_ptr<SRC_STATE> HandleType;
	HandleType hndl;
	~FullResamplerImpl() = default;
	FullResamplerImpl(int channels = 1, ResampleType converterType = ResampleType::SINC_FASTEST)
		: channels(channels), converterType(converterType),
		  hndl(HandleType(src_new(int(converterType),channels,&data.end_of_input),src_delete))
	{
		std::memset(&data,0,sizeof(SRC_DATA));
	}
	FullResamplerImpl(const FullResamplerImpl& cpy)
		: channels(cpy.channels), converterType(cpy.converterType),
		  hndl(HandleType(src_new(int(cpy.converterType),cpy.channels,&data.end_of_input),src_delete))
	{
		std::memset(&data,0,sizeof(SRC_DATA));
	}
	FullResamplerImpl(FullResamplerImpl&& mov)
		: hndl(mov.hndl)
	{
		std::memcpy(&this->data,&mov.data,sizeof(SRC_DATA));
		mov.hndl = nullptr;
	}
	void operator=(const FullResamplerImpl& cpy)
	{
		this->channels = cpy.channels;
		this->converterType = cpy.converterType;
		this->hndl = HandleType(src_new(int(cpy.converterType),cpy.channels,&data.end_of_input),src_delete);
		std::memset(&data,0,sizeof(SRC_DATA));
	}
	void operator=(FullResamplerImpl&& mov)
	{
		this->channels = mov.channels;
		this->converterType = mov.converterType;
		this->hndl = mov.hndl;
		std::memcpy(&this->data,&mov.data,sizeof(SRC_DATA));
		mov.hndl = nullptr;
	}
	int process()
	{
		if(hndl) return src_process(hndl.get(),&data);
		else return 0;
	}
	int reset() const
	{
		if(hndl) return src_reset(hndl.get());
		else return 0;
	}
	const float* getDataIn() const { return data.data_in; }
	const float* getDataOut() const { return data.data_out; }
	float* getDataOut() { return data.data_out; }
	long getInputFrames() const { return data.input_frames; }
	long getOutputFrames() const { return data.output_frames; }
	double getRatio() const { return data.src_ratio; }
	bool getEndOfInput() const { return bool(data.end_of_input); }
	void setDataIn(const float* set_to) { data.data_in = set_to; }
	void setDataOut(float* set_to) { data.data_out = set_to; }
	void setInputFrames(long set_to) { data.input_frames = set_to; }
	void setOutputFrames(long set_to) { data.output_frames = set_to; }
	void setRatio(double set_to) { data.src_ratio = set_to; }
	void setEndOfInput(bool set_to) { data.end_of_input = int(set_to); }
	void set(const float* data_in, float* data_out, long input_frames, long output_frames, double ratio)
	{
		reset();
		std::memset(&data,0,sizeof(SRC_DATA));
		data.data_in = data_in;
		data.data_out = data_out;
		data.input_frames = input_frames;
		data.output_frames = output_frames;
		data.src_ratio = ratio;
	}
};

long DefaultCallback(void *self, float **data)
{
	if(self)
	{
		auto sampler = reinterpret_cast<CallbackInterfaceBase*>(self);
		auto def = sampler->callBack();
		*data = def.second;
		return def.first;
	} else return 0;
}

struct CallbackResampler::CallbackResamplerImpl {
	int error;
	int channels;
	ResampleType converterType;
	typedef std::shared_ptr<SRC_STATE> HandleType;
	HandleType hndl;
	~CallbackResamplerImpl() = default;
	CallbackResamplerImpl(CallbackResamplerImpl&& mov)
		: error(0), channels(mov.channels), converterType(mov.converterType), hndl(mov.hndl)
	{
		mov.hndl = nullptr;
	}
	void operator=(CallbackResamplerImpl&& mov)
	{
		this->hndl = mov.hndl;
		this->channels = mov.channels;
		this->converterType = mov.converterType;
		mov.hndl = nullptr;
	}
	CallbackResamplerImpl(const CallbackResamplerImpl& cpy) = delete; // No copy-constructor
	void operator=(const CallbackResamplerImpl& cpy) = delete; // No copy-assignment
	CallbackResamplerImpl(src_callback_t func, ResampleType converter_type, int channels, void* userdata)
		: error(0), channels(channels), converterType(converter_type),
		  hndl(HandleType(src_callback_new(func,int(converter_type),channels,&error,userdata),src_delete))
	{
		;
	}
	CallbackResamplerImpl(CallbackInterfaceBase* interface, ResampleType converter_type, int channels)
		: error(0), channels(channels), converterType(converter_type),
		  hndl(HandleType(src_callback_new(DefaultCallback,int(converter_type),channels,&error,interface),src_delete))
	{
		;
	}
	long read(double src_ratio, long frames, float *data) const
	{
		if(hndl) return src_callback_read(hndl.get(),src_ratio,frames,data);
		else return 0;
	}
	int reset() const
	{
		if(hndl) return src_reset(hndl.get());
		else return 0;
	}
};

SimpleResampler::SimpleResampler()
	: pimpl(nullptr)
{
	;
}
SimpleResampler::SimpleResampler(float* dataIn, long input_frames, float* dataOut, long outputFrames, double src_ratio)
	: pimpl(new SimpleResamplerImpl(dataIn,input_frames,dataOut,outputFrames,src_ratio))
{
	;
}
SimpleResampler::SimpleResampler(const SimpleResampler& cpy)
	: pimpl((cpy.pimpl) ? new SimpleResamplerImpl(*cpy.pimpl) : nullptr)
{
	;
}
SimpleResampler::SimpleResampler(SimpleResampler&& mov)
	: pimpl(mov.pimpl)
{
	mov.pimpl = nullptr;
}
void SimpleResampler::operator=(const SimpleResampler& cpy)
{
	pimpl = sSimpleResamplerImpl((cpy.pimpl) ? new SimpleResamplerImpl(*cpy.pimpl) : nullptr);
}
void SimpleResampler::operator=(SimpleResampler&& mov)
{
	this->pimpl = mov.pimpl;
	mov.pimpl = nullptr;
}
FullResampler::FullResampler()
	: pimpl(nullptr)
{
	;
}
FullResampler::FullResampler(int channels, ResampleType converterType)
	: pimpl(new FullResamplerImpl(channels,converterType))
{
	;
}
FullResampler::FullResampler(const FullResampler& cpy)
	: pimpl((cpy.pimpl) ? new FullResamplerImpl(*cpy.pimpl) : nullptr)
{
	;
}
FullResampler::FullResampler(FullResampler&& mov)
	: pimpl(mov.pimpl)
{
	mov.pimpl = nullptr;
}
void FullResampler::operator=(const FullResampler& cpy)
{
	pimpl = sFullResamplerImpl((cpy.pimpl) ? new FullResamplerImpl(*cpy.pimpl) : nullptr);
}
void FullResampler::operator=(FullResampler&& mov)
{
	this->pimpl = mov.pimpl;
	mov.pimpl = nullptr;
}
CallbackResampler::CallbackResampler()
	: pimpl(nullptr)
{
	;
}
CallbackResampler::CallbackResampler(CallbackResampler&& mov)
	: pimpl(mov.pimpl)
{
	mov.pimpl = nullptr;
}
void CallbackResampler::operator=(CallbackResampler&& mov)
{
	this->pimpl = mov.pimpl;
	mov.pimpl = nullptr;
}
CallbackResampler::CallbackResampler(samplerate_callback func, ResampleType converter_type, int channels, void* userdata)
	: pimpl(new CallbackResamplerImpl(func,converter_type,channels,userdata))
{
	;
}
CallbackResampler::CallbackResampler(CallbackInterfaceBase* interface, ResampleType converter_type, int channels)
	: pimpl(new CallbackResamplerImpl(interface,converter_type,channels))
{
	;
}

int SimpleResampler::resample(ResampleType resamplerType, int channels) const
{
	if(pimpl) return pimpl->resample(resamplerType,channels);
	else return 0;
}
const float* SimpleResampler::getDataIn() const
{
	if(pimpl) return pimpl->getDataIn();
	else return nullptr;
}
float* SimpleResampler::getDataOut() const
{
	if(pimpl) return pimpl->getDataOut();
	else return nullptr;
}
long SimpleResampler::getInputFrames() const
{
	if(pimpl) return pimpl->getInputFrames();
	else return 0;
}
long SimpleResampler::getOutputFrames() const
{
	if(pimpl) return pimpl->getOutputFrames();
	else return 0;
}
double SimpleResampler::getRatio() const
{
	if(pimpl) return pimpl->getOutputFrames();
	else return 0.0;
}
bool SimpleResampler::getEndOfInput() const
{
	if(pimpl) return pimpl->getEndOfInput();
	else return true;
}
void SimpleResampler::setDataIn(const float* set_to) const
{
	if(pimpl) return pimpl->setDataIn(set_to);
}
void SimpleResampler::setDataOut(float* set_to) const
{
	if(pimpl) return pimpl->setDataOut(set_to);
}
void SimpleResampler::setInputFrames(long set_to) const
{
	if(pimpl) return pimpl->setInputFrames(set_to);
}
void SimpleResampler::setOutputFrames(long set_to) const
{
	if(pimpl) return pimpl->setOutputFrames(set_to);
}
void SimpleResampler::setRatio(double set_to) const
{
	if(pimpl) return pimpl->setRatio(set_to);
}
void SimpleResampler::setEndOfInput(bool set_to) const
{
	if(pimpl) return pimpl->setEndOfInput(set_to);
}
void SimpleResampler::set(const float* data_in, float* data_out, long input_frames, long output_frames, double ratio) const
{
	if(pimpl) return pimpl->set(data_in,data_out,input_frames,output_frames,ratio);
}
int FullResampler::reset() const
{
	if(pimpl) return pimpl->reset();
	else return 0;
}
int FullResampler::process() const
{
	if(pimpl) return pimpl->process();
	else return 0;
}
const float* FullResampler::getDataIn() const
{
	if(pimpl) return pimpl->getDataIn();
	else return nullptr;
}
float* FullResampler::getDataOut() const
{
	if(pimpl) return pimpl->getDataOut();
	else return nullptr;
}
long FullResampler::getInputFrames() const
{
	if(pimpl) return pimpl->getInputFrames();
	else return 0;
}
long FullResampler::getOutputFrames() const
{
	if(pimpl) return pimpl->getOutputFrames();
	else return 0;
}
double FullResampler::getRatio() const
{
	if(pimpl) return pimpl->getOutputFrames();
	else return 0.0;
}
bool FullResampler::getEndOfInput() const
{
	if(pimpl) return pimpl->getEndOfInput();
	else return true;
}
void FullResampler::setDataIn(const float* set_to) const
{
	if(pimpl) return pimpl->setDataIn(set_to);
}
void FullResampler::setDataOut(float* set_to) const
{
	if(pimpl) return pimpl->setDataOut(set_to);
}
void FullResampler::setInputFrames(long set_to) const
{
	if(pimpl) return pimpl->setInputFrames(set_to);
}
void FullResampler::setOutputFrames(long set_to) const
{
	if(pimpl) return pimpl->setOutputFrames(set_to);
}
void FullResampler::setRatio(double set_to) const
{
	if(pimpl) return pimpl->setRatio(set_to);
}
void FullResampler::setEndOfInput(bool set_to) const
{
	if(pimpl) return pimpl->setEndOfInput(set_to);
}
void FullResampler::set(const float* data_in, float* data_out, long input_frames, long output_frames, double ratio) const
{
	if(pimpl) return pimpl->set(data_in,data_out,input_frames,output_frames,ratio);
}
long CallbackResampler::read(double src_ratio, long frames, float *data) const
{
	if(pimpl) return pimpl->read(src_ratio,frames,data);
	else return 0;
}
int CallbackResampler::reset() const
{
	if(pimpl) return pimpl->reset();
	else return 0;
}
int FullResampler::getChannelCount() const
{
	if(pimpl) return pimpl->channels;
	else return 0;
}
ResampleType FullResampler::getResamplerType() const
{
	if(pimpl) return pimpl->converterType;
	else return ResampleType::SINC_BEST_QUALITY;
}
int CallbackResampler::getChannelCount() const
{
	if(pimpl) return pimpl->channels;
	else return 0;
}
ResampleType CallbackResampler::getResamplerType() const
{
	if(pimpl) return pimpl->converterType;
	else return ResampleType::SINC_BEST_QUALITY;
}
CallbackInterface::CallbackInterface(int channels, ResampleType converter_type)
	: CallbackResampler(this,converter_type,channels)
{
	;
}

}
