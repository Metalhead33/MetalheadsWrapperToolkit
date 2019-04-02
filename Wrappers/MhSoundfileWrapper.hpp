#ifndef MHSOUNDFILEWRAPPER_HPP
#define MHSOUNDFILEWRAPPER_HPP
#include "../Io/FIO.hpp"

namespace Mh {

enum class SoundFormat : int {
	AIFF_ALAW = (0x020000 | 0x0011),
	AIFF_DOUBLE = (0x020000 | 0x0007),
	AIFF_DWVW_12 = (0x020000 | 0x0040),
	AIFF_DWVW_16 = (0x020000 | 0x0041),
	AIFF_DWVW_24 = (0x020000 | 0x0042),
	AIFF_FLOAT = (0x020000 | 0x0006),
	AIFF_GSM610 = (0x020000 | 0x0020),
	AIFF_PCM_16 = (0x020000 | 0x0002),
	AIFF_PCM_24 = (0x020000 | 0x0003),
	AIFF_PCM_32 = (0x020000 | 0x0004),
	AIFF_PCM_S8 = (0x020000 | 0x0001),
	AIFF_PCM_U8 = (0x020000 | 0x0005),
	AIFF_ULAW = (0x020000 | 0x0010),
	AU_ALAW = (0x030000 | 0x0011),
	AU_DOUBLE = (0x030000 | 0x0007),
	AU_FLOAT = (0x030000 | 0x0006),
	AU_G721_32 = (0x030000 | 0x0030),
	AU_G723_24 = (0x030000 | 0x0031),
	AU_G723_40 = (0x030000 | 0x0032),
	AU_PCM_16 = (0x030000 | 0x0002),
	AU_PCM_24 = (0x030000 | 0x0003),
	AU_PCM_32 = (0x030000 | 0x0004),
	AU_PCM_S8 = (0x030000 | 0x0001),
	AU_ULAW = (0x030000 | 0x0010),
	CAF_ALAW = (0x180000 | 0x0011),
	CAF_DOUBLE = (0x180000 | 0x0007),
	CAF_FLOAT = (0x180000 | 0x0006),
	CAF_PCM_16 = (0x180000 | 0x0002),
	CAF_PCM_24 = (0x180000 | 0x0003),
	CAF_PCM_32 = (0x180000 | 0x0004),
	CAF_PCM_S8 = (0x180000 | 0x0001),
	CAF_ULAW = (0x180000 | 0x0010),
	FLAC_PCM_16 = (0x170000 | 0x0002),
	FLAC_PCM_24 = (0x170000 | 0x0003),
	FLAC_PCM_S8 = (0x170000 | 0x0001),
	HTK_PCM_16 = (0x100000 | 0x0002),
	IRCAM_ALAW = (0x0A0000 | 0x0011),
	IRCAM_FLOAT = (0x0A0000 | 0x0006),
	IRCAM_PCM_16 = (0x0A0000 | 0x0002),
	IRCAM_PCM_24 = (0x0A0000 | 0x0003),
	IRCAM_PCM_32 = (0x0A0000 | 0x0004),
	IRCAM_ULAW = (0x0A0000 | 0x0010),
	MAT4_DOUBLE = (0x0C0000 | 0x0007),
	MAT4_FLOAT = (0x0C0000 | 0x0006),
	MAT4_PCM_16 = (0x0C0000 | 0x0002),
	MAT4_PCM_32 = (0x0C0000 | 0x0004),
	MAT5_DOUBLE = (0x0D0000 | 0x0007),
	MAT5_FLOAT = (0x0D0000 | 0x0006),
	MAT5_PCM_16 = (0x0D0000 | 0x0002),
	MAT5_PCM_32 = (0x0D0000 | 0x0004),
	MAT5_PCM_U8 = (0x0D0000 | 0x0005),
	NIST_ALAW = (0x070000 | 0x0011),
	NIST_PCM_16 = (0x070000 | 0x0002),
	NIST_PCM_24 = (0x070000 | 0x0003),
	NIST_PCM_32 = (0x070000 | 0x0004),
	NIST_PCM_S8 = (0x070000 | 0x0001),
	NIST_ULAW = (0x070000 | 0x0010),
	OGG_VORBIS = (0x200000 | 0x0060),
	PAF_PCM_16 = (0x050000 | 0x0002),
	PAF_PCM_24 = (0x050000 | 0x0003),
	PAF_PCM_S8 = (0x050000 | 0x0001),
	PVF_PCM_16 = (0x0E0000 | 0x0002),
	PVF_PCM_32 = (0x0E0000 | 0x0004),
	PVF_PCM_S8 = (0x0E0000 | 0x0001),
	RAW_ALAW = (0x040000 | 0x0011),
	RAW_DOUBLE = (0x040000 | 0x0007),
	RAW_DWVW_12 = (0x040000 | 0x0040),
	RAW_DWVW_16 = (0x040000 | 0x0041),
	RAW_DWVW_24 = (0x040000 | 0x0042),
	RAW_FLOAT = (0x040000 | 0x0006),
	RAW_GSM610 = (0x040000 | 0x0020),
	RAW_PCM_16 = (0x040000 | 0x0002),
	RAW_PCM_24 = (0x040000 | 0x0003),
	RAW_PCM_32 = (0x040000 | 0x0004),
	RAW_PCM_S8 = (0x040000 | 0x0001),
	RAW_PCM_U8 = (0x040000 | 0x0005),
	RAW_ULAW = (0x040000 | 0x0010),
	RAW_VOX_ADPCM = (0x040000 | 0x0021),
	SD2_PCM_16 = (0x160000 | 0x0002),
	SD2_PCM_24 = (0x160000 | 0x0003),
	SD2_PCM_S8 = (0x160000 | 0x0001),
	SVX_PCM_16 = (0x060000 | 0x0002),
	SVX_PCM_S8 = (0x060000 | 0x0001),
	VOC_ALAW = (0x080000 | 0x0011),
	VOC_PCM_16 = (0x080000 | 0x0002),
	VOC_PCM_U8 = (0x080000 | 0x0005),
	VOC_ULAW = (0x080000 | 0x0010),
	W64_ALAW = (0x0B0000 | 0x0011),
	W64_DOUBLE = (0x0B0000 | 0x0007),
	W64_FLOAT = (0x0B0000 | 0x0006),
	W64_GSM610 = (0x0B0000 | 0x0020),
	W64_IMA_ADPCM = (0x0B0000 | 0x0012),
	W64_MS_ADPCM = (0x0B0000 | 0x0013),
	W64_PCM_16 = (0x0B0000 | 0x0002),
	W64_PCM_24 = (0x0B0000 | 0x0003),
	W64_PCM_32 = (0x0B0000 | 0x0004),
	W64_PCM_U8 = (0x0B0000 | 0x0005),
	W64_ULAW = (0x0B0000 | 0x0010),
	WAV_ALAW = (0x010000 | 0x0011),
	WAV_DOUBLE = (0x010000 | 0x0007),
	WAV_FLOAT = (0x010000 | 0x0006),
	WAV_G721_32 = (0x010000 | 0x0030),
	WAV_GSM610 = (0x010000 | 0x0020),
	WAV_IMA_ADPCM = (0x010000 | 0x0012),
	WAV_MS_ADPCM = (0x010000 | 0x0013),
	WAV_PCM_16 = (0x010000 | 0x0002),
	WAV_PCM_24 = (0x010000 | 0x0003),
	WAV_PCM_32 = (0x010000 | 0x0004),
	WAV_PCM_U8 = (0x010000 | 0x0005),
	WAV_ULAW = (0x010000 | 0x0010),
	XI_DPCM_16 = (0x0F0000 | 0x0051),
	XI_DPCM_8 = (0x0F0000 | 0x0050)
};

class SoundfileWrapper
{
private:
	DEFINE_STRUCT(SoundfileWrapper_imp)
	sSoundfileWrapper_imp pimpl;
public:
	~SoundfileWrapper() = default;
	SoundfileWrapper();
	SoundfileWrapper(SoundfileWrapper&& mov); // Move constructor
	void operator=(SoundfileWrapper&& mov); // Move assignment operator
	SoundfileWrapper(const SoundfileWrapper& cpy) = delete; // Disallow copying
	void operator=(const SoundfileWrapper& cpy) = delete; // Disallow copying
	SoundfileWrapper(const char *path, bool readOnly=true);
	SoundfileWrapper(const char *path, SoundFormat format, int samplerate, int channelCount, int mode);
	SoundfileWrapper(Abstract::sFIO fio, bool readOnly=true);
	SoundfileWrapper(Abstract::sFIO fio, SoundFormat format, int samplerate, int channelCount, int mode);
	// IO
	long seekSet(long frames) const;
	long seekCur(long frames) const;
	long seekEnd(long frames) const;
	long read(short *ptr,long items) const;
	long read(int *ptr,long items) const;
	long read(float *ptr,long items) const;
	long read(double *ptr,long items) const;
	long readf(short *ptr,long frames) const;
	long readf(int *ptr,long frames) const;
	long readf(float *ptr,long frames) const;
	long readf(double *ptr,long frames) const;
	long write(short *ptr,long items) const;
	long write(int *ptr,long items) const;
	long write(float *ptr,long items) const;
	long write(double *ptr,long items) const;
	long writef(short *ptr,long frames) const;
	long writef(int *ptr,long frames) const;
	long writef(float *ptr,long frames) const;
	long writef(double *ptr,long frames) const;
	long read_raw(void *ptr, long bytes) const;
	long write_raw(void *ptr, long bytes) const;
	void write_sync() const;

	// Getters of non-text data
	long getFrameNum() const;
	int getSamplerate() const;
	int getChannels() const;
	int getFormat() const;
	int getSections() const;
	int getSeekable() const;

	// Text getters
	const char* getTitle() const;
	const char* getCopyright() const;
	const char* getSoftware() const;
	const char* getArtist() const;
	const char* getComment() const;
	const char* getDate() const;
	const char* getAlbum() const;
	const char* getLicense() const;
	const char* getTrackNumber() const;
	const char* getGenre() const;

	// Text getters - C string
	int setTitle(const char* str) const;
	int setCopyright(const char* str) const;
	int setSoftware(const char* str) const;
	int setArtist(const char* str) const;
	int setComment(const char* str) const;
	int setDate(const char* str) const;
	int getAlbum(const char* str) const;
	int setTrackNumber(const char* str) const;
	int setLicense(const char* str) const;
	int setGenre(const char* str) const;

	// Text getters - C++ string
	int setTitle(const std::string& str) const;
	int setCopyright(const std::string& str) const;
	int setSoftware(const std::string& str) const;
	int setArtist(const std::string& str) const;
	int setComment(const std::string& str) const;
	int setDate(const std::string& str) const;
	int getAlbum(const std::string& str) const;
	int setTrackNumber(const std::string& str) const;
	int setLicense(const std::string& str) const;
	int setGenre(const std::string& str) const;
};

}
#endif // MHSOUNDFILEWRAPPER_HPP
