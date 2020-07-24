#include "ImageWrapper.hpp"
extern "C" {
#include <FreeImage.h>
}

unsigned ImageRead( void *buffer, unsigned size, unsigned count, void *handle );
unsigned ImageWrite( void *buffer, unsigned size, unsigned count,
			 void *handle );
int ImageSeek( void *handle, long offset, int origin );
long ImageTell( void *handle );
static FreeImageIO AbstractFreadImgio = {ImageRead, ImageWrite, ImageSeek,
					 ImageTell};

namespace MH33 {
namespace GFX {

	struct ImageWrapper::ImageWrapper_imp {
		typedef std::unique_ptr< FIBITMAP, decltype (&FreeImage_Unload) > ContainerType;
	ContainerType container;
	ImageFormat format;
	~ImageWrapper_imp( ) = default;
	ImageWrapper_imp( ) : container( nullptr,FreeImage_Unload), format(ImageFormat::UNKNOWN) {  }
	ImageWrapper_imp( ContainerType &&cpy ) : container( std::move(cpy) ) { format = ImageFormat(FreeImage_GetImageType(container.get())); }
	ImageWrapper_imp( FIBITMAP *cpy )
		: container( FreeImage_Clone( cpy ), FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	ImageWrapper_imp( const ImageWrapper_imp &cpy )
		: container( FreeImage_Clone( cpy.container.get( ) ),
					FreeImage_Unload ), format(cpy.format) {

	}
	ImageWrapper_imp( ImageWrapper_imp &&mov )
		: container( std::move(mov.container) ), format(mov.format) {
		mov.container = nullptr;
	}
	void operator=( const ImageWrapper_imp &cpy ) {
		this->container = ContainerType(
		FreeImage_Clone( cpy.container.get( ) ), FreeImage_Unload );
		this->format = cpy.format;
	}
	void operator=( ImageWrapper_imp &&mov ) {
		this->container = std::move(mov.container);
		this->format = mov.format;
	}
	ImageWrapper_imp( int width, int height, int bpp, unsigned red_mask=0, unsigned green_mask=0, unsigned blue_mask=0 )
		: container( FreeImage_Allocate( width, height, bpp, red_mask, green_mask, blue_mask ),
					FreeImage_Unload ), format(ImageFormat::BITMAP) {

	}
	ImageWrapper_imp( ImageFormat typus, int width, int height,
			  int bpp = 8, unsigned red_mask=0, unsigned green_mask=0, unsigned blue_mask=0 )
		: container( FreeImage_AllocateT( FREE_IMAGE_TYPE(typus), width, height, bpp, red_mask, green_mask, blue_mask),
					FreeImage_Unload ), format(typus) {

	}
	ImageWrapper_imp( const char *path )
		: container( FreeImage_Load( FreeImage_GetFileType( path ), path ),
			 FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	ImageWrapper_imp( ImageFileType typus, const char *path )
		: container( FreeImage_Load( FREE_IMAGE_FORMAT(typus), path ), FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	ImageWrapper_imp( Abstract::FIO &handle )
		: container(
		  FreeImage_LoadFromHandle( FreeImage_GetFileTypeFromHandle(
						&AbstractFreadImgio, &handle ),
						&AbstractFreadImgio, &handle ),
		  FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	ImageWrapper_imp( ImageFileType typus, Abstract::FIO &handle )
		: container( FreeImage_LoadFromHandle( FREE_IMAGE_FORMAT(typus), &AbstractFreadImgio,
						   &handle ),
					FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	ImageWrapper_imp(void *bits, int width, int height, int pitch, unsigned bpp = 24,
					 unsigned red_mask = FI_RGBA_RED_MASK,
					 unsigned green_mask = FI_RGBA_GREEN_MASK,
					 unsigned blue_mask = FI_RGBA_BLUE_MASK,
					 bool topdown = false )
		: container( FreeImage_ConvertFromRawBits(static_cast<BYTE*>(bits),width,height,pitch,bpp,
												  red_mask,green_mask,blue_mask,topdown)
					 , FreeImage_Unload ) {
		format = ImageFormat(FreeImage_GetImageType(container.get()));
	}
	bool save( ImageFileType format, const char *path ) const {
		if ( container ) {
		return FreeImage_Save( FREE_IMAGE_FORMAT(format), container.get( ), path );
		} else
		return false;
	}
	bool save( ImageFileType format, Abstract::FIO &handle ) const {
		if ( container ) {
		return FreeImage_SaveToHandle( FREE_IMAGE_FORMAT(format), container.get( ),
						   &AbstractFreadImgio, &handle );
		} else
		return false;
	}
	ImageFormat getImageType( ) const {
		return format;
	}
	unsigned getColorsUsed( ) const {
		if ( container )
		return FreeImage_GetColorsUsed( container.get( ) );
		else
		return 0;
	}
	unsigned getBPP( ) const {
		if ( container )
		return FreeImage_GetBPP( container.get( ) );
		else
		return 0;
	}
	unsigned getWidth( ) const {
		if ( container )
		return FreeImage_GetWidth( container.get( ) );
		else
		return 0;
	}
	unsigned getHeight( ) const {
		if ( container )
		return FreeImage_GetHeight( container.get( ) );
		else
		return 0;
	}
	unsigned getLine( ) const {
		if ( container )
		return FreeImage_GetLine( container.get( ) );
		else
		return 0;
	}
	unsigned getPitch( ) const {
		if ( container )
		return FreeImage_GetPitch( container.get( ) );
		else
		return 0;
	}
	unsigned getDIBSize( ) const {
		if ( container )
		return FreeImage_GetDIBSize( container.get( ) );
		else
		return 0;
	}
	unsigned getMemorySize( ) const {
		if ( container )
		return FreeImage_GetMemorySize( container.get( ) );
		else
		return 0;
	}
	RGBQUAD *getPalette( ) const {
		if ( container )
		return FreeImage_GetPalette( container.get( ) );
		else
		return nullptr;
	}
	unsigned getDotsPerMeterX( ) const {
		if ( container )
		return FreeImage_GetDotsPerMeterX( container.get( ) );
		else
		return 0;
	}
	unsigned getDotsPerMeterY( ) const {
		if ( container )
		return FreeImage_GetDotsPerMeterY( container.get( ) );
		else
		return 0;
	}
	void setDotsPerMeterX( unsigned res ) const {
		if ( container )
		FreeImage_SetDotsPerMeterX( container.get( ), res );
	}
	void setDotsPerMeterY( unsigned res ) const {
		if ( container )
		FreeImage_SetDotsPerMeterY( container.get( ), res );
	}
	BITMAPINFOHEADER *getInfoHeader( ) const {
		if ( container )
		return FreeImage_GetInfoHeader( container.get( ) );
		else
		return nullptr;
	}
	BITMAPINFO *getInfo( ) const {
		if ( container )
		return FreeImage_GetInfo( container.get( ) );
		else
		return nullptr;
	}
	FREE_IMAGE_COLOR_TYPE getColorType( ) const {
		if ( container )
		return FreeImage_GetColorType( container.get( ) );
		else
		return FIC_MINISBLACK;
	}
	unsigned getRedMask( ) const {
		if ( container )
		return FreeImage_GetRedMask( container.get( ) );
		else
		return 0;
	}
	unsigned getGreenMask( ) const {
		if ( container )
		return FreeImage_GetGreenMask( container.get( ) );
		else
		return 0;
	}
	unsigned getBlueMask( ) const {
		if ( container )
		return FreeImage_GetBlueMask( container.get( ) );
		else
		return 0;
	}
	unsigned getTransparencyCount( ) const {
		if ( container )
		return FreeImage_GetTransparencyCount( container.get( ) );
		else
		return 0;
	}
	void *getTransparencyTable( ) const {
		if ( container )
		return static_cast< void * >(
			FreeImage_GetTransparencyTable( container.get( ) ) );
		else
		return nullptr;
	}
	void setTransparencyTable( void *bytes, int size ) const {
		if ( container )
		FreeImage_SetTransparencyTable(
			container.get( ), static_cast< BYTE * >( bytes ), size );
	}
	void setTransparent( bool enabled ) const {
		if ( container )
		FreeImage_SetTransparent( container.get( ), enabled );
	}
	bool isTransparent( ) const {
		if ( container )
		return FreeImage_IsTransparent( container.get( ) );
		else
		return false;
	}
	void setTransparentIndex( int index ) const {
		if ( container )
		FreeImage_SetTransparentIndex( container.get( ), index );
	}
	int getTransparentIndex( ) const {
		if ( container )
		return FreeImage_GetTransparentIndex( container.get( ) );
		else
		return 0;
	}
	bool hasBackgroundColor( ) const {
		if ( container )
		return FreeImage_HasBackgroundColor( container.get( ) );
		else
		return false;
	}
	bool getBackgroundColor( RGBQUAD *bkcolor ) const {
		if ( container )
		return FreeImage_GetBackgroundColor( container.get( ),
							 bkcolor );
		else
		return false;
	}
	bool setBackgroundColor( RGBQUAD *bkcolor ) const {
		if ( container )
		return FreeImage_SetBackgroundColor( container.get( ),
							 bkcolor );
		else
		return false;
	}
	bool hasPixels( ) const {
		if ( container )
		return FreeImage_HasPixels( container.get( ) );
		else
		return false;
	}
	bool hasThumbnail( ) const {
		if ( container )
		return FreeImage_GetThumbnail( container.get( ) ) != nullptr;
		else
		return false;
	}
	ImageWrapper_imp copyThumbnail( ) const {
		if ( container )
		return ImageWrapper_imp(
			FreeImage_GetThumbnail( container.get( ) ) );
		else
		return ImageWrapper_imp( );
	}
	bool setThumbnail( const ImageWrapper_imp &cpy ) {
		if ( container )
		return FreeImage_SetThumbnail( container.get( ),
						   cpy.container.get( ) );
		else
		return false;
	}
	bool removeThumbnail( void ) {
		if ( container )
		return FreeImage_SetThumbnail( container.get( ), nullptr );
		else
		return false;
	}
	void *getBytes( ) const {
		if ( container )
		return static_cast< void * >(
			FreeImage_GetBits( container.get( ) ) );
		else
		return nullptr;
	}
	void *getScanline( int scanline ) const {
		if ( container )
		return static_cast< void * >(
			FreeImage_GetScanLine( container.get( ), scanline ) );
		else
		return nullptr;
	}
	bool getPixelIndex( unsigned x, unsigned y, void *value ) const {
		if ( container )
		return FreeImage_GetPixelIndex(
			container.get( ), x, y, static_cast< BYTE * >( value ) );
		else
		return false;
	}
	bool getPixelColor( unsigned x, unsigned y, RGBQUAD *value ) const {
		if ( container )
		return FreeImage_GetPixelColor( container.get( ), x, y, value );
		else
		return false;
	}
	bool setPixelIndex( unsigned x, unsigned y, void *value ) const {
		if ( container )
		return FreeImage_SetPixelIndex(
			container.get( ), x, y, static_cast< BYTE * >( value ) );
		else
		return false;
	}
	bool setPixelColor( unsigned x, unsigned y, RGBQUAD *value ) const {
		if ( container )
		return FreeImage_SetPixelColor( container.get( ), x, y, value );
		else
		return false;
	}
	ImageWrapper_imp convertTo4Bits( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertTo4Bits( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertTo8Bits( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertTo8Bits( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToGreyscale( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertToGreyscale( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertTo16Bits555( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertTo16Bits555( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertTo16Bits565( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertTo16Bits565( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertTo24Bits( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertTo24Bits( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertTo32Bits( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertTo32Bits( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp
	quantize( FREE_IMAGE_QUANTIZE quantizationType ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ColorQuantize( container.get( ),
								quantizationType ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp quantizeXiaolinWu( ) const {
		return quantize( FIQ_WUQUANT );
	}
	ImageWrapper_imp quantizeNeuQuant( ) const {
		return quantize( FIQ_NNQUANT );
	}
	ImageWrapper_imp quantizeCarlsten( ) const {
		return quantize( FIQ_LFPQUANT );
	}
	ImageWrapper_imp threshold( unsigned char hold ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_Threshold( container.get( ), hold ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp dither( FREE_IMAGE_DITHER algorithm ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_Dither( container.get( ), algorithm ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp ditherFS( ) const { return dither( FID_FS ); }
	ImageWrapper_imp ditherBayer4x4( ) const {
		return dither( FID_BAYER4x4 );
	}
	ImageWrapper_imp ditherBayer8x8( ) const {
		return dither( FID_BAYER8x8 );
	}
	ImageWrapper_imp ditherBayer16x16( ) const {
		return dither( FID_BAYER16x16 );
	}
	ImageWrapper_imp ditherCluster6x6( ) const {
		return dither( FID_CLUSTER6x6 );
	}
	ImageWrapper_imp ditherCluster8x8( ) const {
		return dither( FID_CLUSTER8x8 );
	}
	ImageWrapper_imp ditherCluster16x16( ) const {
		return dither( FID_CLUSTER16x16 );
	}
	ImageWrapper_imp convertToStandardType( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertToStandardType( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToType( FREE_IMAGE_TYPE dst_type ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertToType( container.get( ), dst_type ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToFloat( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertToFloat( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToRGBF( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertToRGBF( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToRGBAF( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertToRGBAF( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToUINT16( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertToUINT16( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToRGB16( ) const {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_ConvertToRGB16( container.get( ) ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp convertToRGBA16( ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ConvertToRGBA16( container.get( ) ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp tonemap( FREE_IMAGE_TMO algorithm, double param1 = 0.0,
				  double param2 = 0.0 ) const {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_ToneMapping( container.get( ), algorithm, param1,
					   param2 ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp tonemapDrago03( double gamma = 2.2,
					 double exposure = 0.0 ) {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_TmoDrago03( container.get( ), gamma, exposure ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp tonemapReinhard05( double intensity = 0.0,
						double contrast = 0.0 ) {
		if ( container )
		return ImageWrapper_imp(
			ContainerType( FreeImage_TmoReinhard05(
					   container.get( ), intensity, contrast ),
				   FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp tonemapReinhard05Ex( double intensity = 0.0,
						  double contrast = 0.0,
						  double adaptation = 1.0,
						  double color_correction = 0.0 ) {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_TmoReinhard05Ex( container.get( ), intensity,
						   contrast, adaptation,
						   color_correction ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	ImageWrapper_imp tonemapFattal02( double color_saturation = 0.5,
					  double attenuation = 0.85 ) {
		if ( container )
		return ImageWrapper_imp( ContainerType(
			FreeImage_TmoFattal02( container.get( ), color_saturation,
					   attenuation ),
			FreeImage_Unload ) );
		else
		return ImageWrapper_imp( );
	}
	bool isValid( ) const { return container != nullptr; }
	};

	ImageWrapper::~ImageWrapper() = default;

	ImageWrapper::ImageWrapper()
		: pimpl(nullptr)
	{

	}

	ImageWrapper::ImageWrapper(ImageWrapper_imp &&mov)
		: pimpl(new ImageWrapper_imp(std::move(mov)))
	{

	}

	ImageWrapper::ImageWrapper(uImageWrapper_imp &&mov)
		: pimpl(std::move(mov))
	{

	}
	ImageWrapper::ImageWrapper( const ImageWrapper &cpy )
		: pimpl((cpy.pimpl) ? new ImageWrapper_imp(*cpy.pimpl) : nullptr)
	{

	}
	ImageWrapper::ImageWrapper( ImageWrapper &&mov )
		: pimpl(std::move(mov.pimpl))
	{
		mov.pimpl = nullptr;
	}
	void ImageWrapper::operator=( ImageWrapper &&mov )
	{
		this->pimpl = std::move(mov.pimpl);
	}
	ImageWrapper::ImageWrapper(void *bits, int width, int height, int pitch, unsigned bpp,
				unsigned red_mask, unsigned green_mask, unsigned blue_mask,
				 bool topdown)
		: pimpl(new ImageWrapper_imp(bits,width,height,pitch,bpp,red_mask,green_mask,blue_mask,topdown))
	{

	}
	ImageWrapper::ImageWrapper(void *bits, int width, int height, int pitch, unsigned bpp, bool topdown)
		: pimpl(new ImageWrapper_imp(bits,width,height,pitch,bpp,
									 FI_RGBA_RED_MASK,FI_RGBA_GREEN_MASK,FI_RGBA_BLUE_MASK,topdown))
	{

	}

	void ImageWrapper::operator=(const ImageWrapper &cpy)
	{
		uImageWrapper_imp tmp((cpy.pimpl) ? new ImageWrapper_imp(*cpy.pimpl) : nullptr);
		pimpl = std::move(tmp);
	}
	ImageWrapper::ImageWrapper(int width, int height, int bpp , unsigned red_mask, unsigned green_mask, unsigned blue_mask)
		: pimpl(new ImageWrapper_imp(width,height,bpp,red_mask,green_mask,blue_mask))
	{

	}
	ImageWrapper::ImageWrapper(ImageFormat typus, int width, int height, int bpp, unsigned red_mask, unsigned green_mask, unsigned blue_mask)
		: pimpl(new ImageWrapper_imp(typus,width,height,bpp,red_mask,green_mask,blue_mask))
	{

	}
	ImageWrapper::ImageWrapper( const char *path )
		: pimpl(new ImageWrapper_imp(path))
	{

	}
	ImageWrapper::ImageWrapper( ImageFileType typus, const char *path )
		: pimpl(new ImageWrapper_imp(typus,path))
	{

	}
	ImageWrapper::ImageWrapper( Abstract::FIO &handle )
		: pimpl(new ImageWrapper_imp(handle))
	{

	}
	ImageWrapper::ImageWrapper( ImageFileType typus, Abstract::FIO &handle )
		: pimpl(new ImageWrapper_imp(typus,handle))
	{

	}

	unsigned ImageWrapper::getColorsUsed( ) const {
	return pimpl->getColorsUsed( );
	}
	unsigned ImageWrapper::getBPP( ) const { return pimpl->getBPP( ); }
	unsigned ImageWrapper::getWidth( ) const { return pimpl->getWidth( ); }
	unsigned ImageWrapper::getHeight( ) const { return pimpl->getHeight( ); }
	unsigned ImageWrapper::getLine( ) const { return pimpl->getLine( ); }
	unsigned ImageWrapper::getPitch( ) const { return pimpl->getPitch( ); }
	unsigned ImageWrapper::getDIBSize( ) const { return pimpl->getDIBSize( ); }
	unsigned ImageWrapper::getMemorySize( ) const {
		return pimpl->getMemorySize( );
	}
	ImageFormat ImageWrapper::getFormat() const
	{
		return pimpl->getImageType();
	}
	ImageWrapper ImageWrapper::convertTo4Bits( ) const {
		return ImageWrapper(pimpl->convertTo4Bits( ));
	}
	ImageWrapper ImageWrapper::convertTo8Bits( ) const {
	return ImageWrapper(pimpl->convertTo8Bits( ));
	}
	ImageWrapper ImageWrapper::convertToGreyscale( ) const {
	return ImageWrapper(pimpl->convertToGreyscale( ));
	}
	ImageWrapper ImageWrapper::convertTo16Bits555( ) const {
		return ImageWrapper(pimpl->convertTo16Bits555( ));
	}
	ImageWrapper ImageWrapper::convertTo16Bits565( ) const {
		return ImageWrapper(pimpl->convertTo16Bits565( ));
	}
	ImageWrapper ImageWrapper::convertTo24Bits( ) const {
		return ImageWrapper(pimpl->convertTo24Bits( ));
	}
	ImageWrapper ImageWrapper::convertTo32Bits( ) const {
		return ImageWrapper(pimpl->convertTo32Bits( ));
	}
	ImageWrapper ImageWrapper::quantizeXiaolinWu( ) const {
		return ImageWrapper(pimpl->quantizeXiaolinWu( ));
	}
	ImageWrapper ImageWrapper::quantizeNeuQuant( ) const {
		return ImageWrapper(pimpl->quantizeNeuQuant( ));
	}
	ImageWrapper ImageWrapper::quantizeCarlsten( ) const {
		return ImageWrapper(pimpl->quantizeCarlsten( ));
	}
	ImageWrapper ImageWrapper::threshold( unsigned char hold ) const {
		return ImageWrapper(pimpl->threshold(hold));
	}
	ImageWrapper ImageWrapper::ditherFS( ) const {
		return ImageWrapper(pimpl->ditherFS( ));
	}
	ImageWrapper ImageWrapper::ditherBayer4x4( ) const {
		return ImageWrapper(pimpl->ditherBayer4x4( ));
	}
	ImageWrapper ImageWrapper::ditherBayer8x8( ) const {
		return ImageWrapper(pimpl->ditherBayer8x8( ));
	}
	ImageWrapper ImageWrapper::ditherBayer16x16( ) const {
		return ImageWrapper(pimpl->ditherBayer16x16( ));
	}
	ImageWrapper ImageWrapper::ditherCluster6x6( ) const {
		return ImageWrapper(pimpl->ditherCluster6x6( ));
	}
	ImageWrapper ImageWrapper::ditherCluster8x8( ) const {
		return ImageWrapper(pimpl->ditherCluster8x8( ));
	}
	ImageWrapper ImageWrapper::ditherCluster16x16( ) const {
		return ImageWrapper(pimpl->ditherCluster16x16( ));
	}
	ImageWrapper ImageWrapper::convertToStandardType( ) const {
		return ImageWrapper(pimpl->convertToStandardType( ));
	}
	ImageWrapper ImageWrapper::convertToFloat( ) const {
		return ImageWrapper(pimpl->convertToFloat( ));
	}
	ImageWrapper ImageWrapper::convertToRGBF( ) const {
		return ImageWrapper(pimpl->convertToRGBF( ));
	}
	ImageWrapper ImageWrapper::convertToRGBAF( ) const {
		return ImageWrapper(pimpl->convertToRGBAF( ));
	}
	ImageWrapper ImageWrapper::convertToUINT16( ) const {
		return ImageWrapper(pimpl->convertToUINT16( ));
	}
	ImageWrapper ImageWrapper::convertToRGB16( ) const {
		return ImageWrapper(pimpl->convertToRGB16( ));
	}
	ImageWrapper ImageWrapper::convertToRGBA16( ) const {
		return ImageWrapper(pimpl->convertToRGBA16( ));
	}
	ImageWrapper ImageWrapper::tonemapDrago03( double gamma, double exposure ) const {
		return ImageWrapper(pimpl->tonemapDrago03(gamma,exposure));
	}
	ImageWrapper ImageWrapper::tonemapReinhard05( double intensity,
					  double contrast ) const {
		return ImageWrapper(pimpl->tonemapReinhard05(intensity,contrast));
	}
	ImageWrapper ImageWrapper::tonemapReinhard05Ex( double intensity, double contrast,
						double adaptation,
						double color_correction ) const {
		return ImageWrapper(pimpl->tonemapReinhard05Ex(intensity,contrast,adaptation,color_correction));
	}
	ImageWrapper ImageWrapper::tonemapFattal02( double color_saturation,
					double attenuation ) const {
		return ImageWrapper(pimpl->tonemapFattal02(color_saturation,attenuation));
	}

	bool ImageWrapper::hasPixels( ) const
	{
		if(pimpl) return pimpl->hasPixels();
		else return false;
	}
	bool ImageWrapper::hasThumbnail( ) const
	{
		if(pimpl) return pimpl->hasThumbnail();
		else return false;
	}
	ImageWrapper ImageWrapper::copyThumbnail( ) const
	{
		if(pimpl)
		{
			uImageWrapper_imp tmp(new ImageWrapper_imp(pimpl->copyThumbnail() ));
			return ImageWrapper(std::move(tmp));
		} else return ImageWrapper();
	}
	bool ImageWrapper::setThumbnail( const ImageWrapper &cpy ) const
	{
		if(pimpl)
		{
			if(cpy.hasPixels()) {
				return pimpl->setThumbnail(*cpy.pimpl);
			}
			else return false;
		}
		else return false;
	}
	bool ImageWrapper::removeThumbnail( void ) const
	{
		if(pimpl)
		{
			return pimpl->removeThumbnail();
		} else return false;
	}
	void* ImageWrapper::getBytes( ) const
	{
		if(pimpl) return pimpl->getBytes();
		else return nullptr;
	}
	void* ImageWrapper::getScanline( int scanline ) const
	{
		if(pimpl) return pimpl->getScanline(scanline);
		else return nullptr;
	}

} // Namespace GFX
} // namespace Mh

unsigned ImageRead( void *buffer, unsigned size, unsigned count,
			void *handle ) {
	return unsigned( reinterpret_cast< Abstract::pFIO >( handle )->read(
	buffer, size * count ) );
}
unsigned ImageWrite( void *buffer, unsigned size, unsigned count,
			 void *handle ) {
	return unsigned( reinterpret_cast< Abstract::pFIO >( handle )->write(
	buffer, size * count ) );
}
int ImageSeek( void *handle, long offset, int origin ) {
	Abstract::pFIO chandle = reinterpret_cast< Abstract::pFIO >( handle );
	switch ( origin ) {
	case SEEK_SET:
	return int( chandle->seek( offset ) );
	break;
	case SEEK_CUR:
	return int( chandle->seek( chandle->tell( ) + offset ) );
	break;
	case SEEK_END:
	return int( chandle->seek( chandle->size( ) - offset ) );
	break;
	default:
	return 0;
	}
}
long ImageTell( void *handle ) {
	return reinterpret_cast< Abstract::pFIO >( handle )->tell( );
}
