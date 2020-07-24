#include "SoundfileWrapper.hpp"
#include <cassert>
#include <cstring>
#include <sndfile.h>

sf_count_t sfGetFilelen( void *user_data );
sf_count_t sfSeek( sf_count_t offset, int whence, void *user_data );
sf_count_t sfRead( void *ptr, sf_count_t count, void *user_data );
sf_count_t sfWrite( const void *ptr, sf_count_t count, void *user_data );
sf_count_t sfTell( void *user_data );
static SF_VIRTUAL_IO sndFileIO = { sfGetFilelen, sfSeek, sfRead, sfWrite,
                                   sfTell };

namespace MH33 {
namespace Audio {

    struct SoundfileWrapper::SoundfileWrapper_imp {
        typedef std::unique_ptr< SNDFILE, decltype( &sf_close ) > HandleType;
        SF_INFO info;
        HandleType sound_handle;
        Abstract::sFIO file_handle;
        ~SoundfileWrapper_imp( ) = default;
        SoundfileWrapper_imp( )
            : sound_handle( nullptr, sf_close ), file_handle( nullptr ) {
            std::memset( &info, 0, sizeof( SF_INFO ) );
        }
        SoundfileWrapper_imp( SoundfileWrapper_imp &&mov )
            : sound_handle( std::move( mov.sound_handle ) ),
              file_handle( std::move( mov.file_handle ) ) {
            std::memcpy( &this->info, &mov.info, sizeof( SF_INFO ) );
        }
        void operator=( SoundfileWrapper_imp &&mov ) {
            this->sound_handle = std::move( mov.sound_handle );
            this->file_handle = std::move( mov.file_handle );
            std::memcpy( &this->info, &mov.info, sizeof( SF_INFO ) );
        }
        SoundfileWrapper_imp( const SoundfileWrapper_imp & ) =
            delete; // No copy constructors
        void operator=( const SoundfileWrapper_imp & ) =
            delete; // No copy assignments
        SoundfileWrapper_imp( const char *path, bool readOnly = true )
            : info{ 0, 0, 0, 0, 0, 0 },
              sound_handle(
                  sf_open( path, ( readOnly ) ? SFM_READ : SFM_RDWR, &info ),
                  sf_close ),
              file_handle( nullptr ) {
            ;
        }
        SoundfileWrapper_imp( const char *path, SoundFormat format,
                              int samplerate, int channelCount, int mode )
            : info{ 0, samplerate, channelCount, int( format ), 0, 0 },
              sound_handle( sf_open( path, mode, &info ), sf_close ),
              file_handle( nullptr ) {
            ;
        }
        SoundfileWrapper_imp( Abstract::sFIO fio, bool readOnly = true )
            : info{ 0, 0, 0, 0, 0, 0 },
              sound_handle( sf_open_virtual( &sndFileIO,
                                             ( readOnly ) ? SFM_READ : SFM_RDWR,
                                             &info, fio.get( ) ),
                            sf_close ),
              file_handle( fio ) {}
        SoundfileWrapper_imp( Abstract::sFIO fio, SoundFormat format,
                              int samplerate, int channelCount, int mode )
            : info{ 0, samplerate, channelCount, int( format ), 0, 0 },
              sound_handle(
                  sf_open_virtual( &sndFileIO, mode, &info, fio.get( ) ),
                  sf_close ),
              file_handle( fio ) {
            ;
        }
        sf_count_t seek( sf_count_t frames, int whence ) const {
            assert( sound_handle );
            return sf_seek( sound_handle.get( ), frames, whence );
        }
        sf_count_t seekSet( sf_count_t frames ) const {
            return seek( frames, SEEK_SET );
        }
        sf_count_t seekCur( sf_count_t frames ) const {
            return seek( frames, SEEK_CUR );
        }
        sf_count_t seekEnd( sf_count_t frames ) const {
            return seek( frames, SEEK_END );
        }
        sf_count_t read( short *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_read_short( sound_handle.get( ), ptr, items );
        }
        sf_count_t read( int *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_read_int( sound_handle.get( ), ptr, items );
        }
        sf_count_t read( float *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_read_float( sound_handle.get( ), ptr, items );
        }
        sf_count_t read( double *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_read_double( sound_handle.get( ), ptr, items );
        }
        sf_count_t readf( short *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_readf_short( sound_handle.get( ), ptr, frames );
        }
        sf_count_t readf( int *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_readf_int( sound_handle.get( ), ptr, frames );
        }
        sf_count_t readf( float *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_readf_float( sound_handle.get( ), ptr, frames );
        }
        sf_count_t readf( double *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_readf_double( sound_handle.get( ), ptr, frames );
        }
        sf_count_t write( short *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_write_short( sound_handle.get( ), ptr, items );
        }
        sf_count_t write( int *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_write_int( sound_handle.get( ), ptr, items );
        }
        sf_count_t write( float *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_write_float( sound_handle.get( ), ptr, items );
        }
        sf_count_t write( double *ptr, sf_count_t items ) const {
            assert( sound_handle );
            return sf_write_double( sound_handle.get( ), ptr, items );
        }
        sf_count_t writef( short *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_writef_short( sound_handle.get( ), ptr, frames );
        }
        sf_count_t writef( int *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_writef_int( sound_handle.get( ), ptr, frames );
        }
        sf_count_t writef( float *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_writef_float( sound_handle.get( ), ptr, frames );
        }
        sf_count_t writef( double *ptr, sf_count_t frames ) const {
            assert( sound_handle );
            return sf_writef_double( sound_handle.get( ), ptr, frames );
        }
        sf_count_t read_raw( void *ptr, sf_count_t bytes ) const {
            assert( sound_handle );
            return sf_read_raw( sound_handle.get( ), ptr, bytes );
        }
        sf_count_t write_raw( void *ptr, sf_count_t bytes ) const {
            assert( sound_handle );
            return sf_write_raw( sound_handle.get( ), ptr, bytes );
        }
        void write_sync( ) const {
            assert( sound_handle );
            sf_write_sync( sound_handle.get( ) );
        }
        sf_count_t getFrameNum( ) const { return info.frames; }
        int getSamplerate( ) const { return info.samplerate; }
        int getChannels( ) const { return info.channels; }
        int getFormat( ) const { return info.format; }
        int getSections( ) const { return info.sections; }
        int getSeekable( ) const { return info.seekable; }
        const char *getString( int str_type ) const {
            assert( sound_handle );
            return sf_get_string( sound_handle.get( ), str_type );
        }
        int setString( int str_type, const char *str ) const {
            assert( sound_handle );
            return sf_set_string( sound_handle.get( ), str_type, str );
        }
        const char *getTitle( ) const { return getString( SF_STR_TITLE ); }
        int setTitle( const char *str ) const {
            return setString( SF_STR_TITLE, str );
        }
        const char *getCopyright( ) const {
            return getString( SF_STR_COPYRIGHT );
        }
        int setCopyright( const char *str ) const {
            return setString( SF_STR_COPYRIGHT, str );
        }
        const char *getSoftware( ) const {
            return getString( SF_STR_SOFTWARE );
        }
        int setSoftware( const char *str ) const {
            return setString( SF_STR_SOFTWARE, str );
        }
        const char *getArtist( ) const { return getString( SF_STR_ARTIST ); }
        int setArtist( const char *str ) const {
            return setString( SF_STR_ARTIST, str );
        }
        const char *getComment( ) const { return getString( SF_STR_COMMENT ); }
        int setComment( const char *str ) const {
            return setString( SF_STR_COMMENT, str );
        }
        const char *getDate( ) const { return getString( SF_STR_DATE ); }
        int setDate( const char *str ) const {
            return setString( SF_STR_DATE, str );
        }
        const char *getAlbum( ) const { return getString( SF_STR_ALBUM ); }
        int getAlbum( const char *str ) const {
            return setString( SF_STR_ALBUM, str );
        }
        const char *getLicense( ) const { return getString( SF_STR_LICENSE ); }
        int setLicense( const char *str ) const {
            return setString( SF_STR_LICENSE, str );
        }
        const char *getTrackNumber( ) const {
            return getString( SF_STR_TRACKNUMBER );
        }
        int setTrackNumber( const char *str ) const {
            return setString( SF_STR_TRACKNUMBER, str );
        }
        const char *getGenre( ) const { return getString( SF_STR_GENRE ); }
        int setGenre( const char *str ) const {
            return setString( SF_STR_GENRE, str );
        }
    };

    SoundfileWrapper::~SoundfileWrapper( ) = default;

    SoundfileWrapper::SoundfileWrapper( ) : pimpl( nullptr ) {}
    SoundfileWrapper::SoundfileWrapper( SoundfileWrapper &&mov )
        : pimpl( std::move( mov.pimpl ) ) {
        mov.pimpl = nullptr;
    }
    void SoundfileWrapper::operator=( SoundfileWrapper &&mov ) {
        this->pimpl = std::move( mov.pimpl );
    }
    SoundfileWrapper::SoundfileWrapper( const char *path, bool readOnly )
        : pimpl( new SoundfileWrapper_imp( path, readOnly ) ) {}
    SoundfileWrapper::SoundfileWrapper( const char *path, SoundFormat format,
                                        int samplerate, int channelCount,
                                        int mode )
        : pimpl( new SoundfileWrapper_imp( path, format, samplerate,
                                           channelCount, mode ) ) {}
    SoundfileWrapper::SoundfileWrapper( Abstract::sFIO fio, bool readOnly )
        : pimpl( new SoundfileWrapper_imp( fio, readOnly ) ) {}
    SoundfileWrapper::SoundfileWrapper( Abstract::sFIO fio, SoundFormat format,
                                        int samplerate, int channelCount,
                                        int mode )
        : pimpl( new SoundfileWrapper_imp( fio, format, samplerate,
                                           channelCount, mode ) ) {}

    long SoundfileWrapper::seekSet( long frames ) const {
        assert( pimpl );
        assert( pimpl );
        return long( pimpl->seekSet( sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::seekCur( long frames ) const {
        assert( pimpl );
        return long( pimpl->seekCur( sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::seekEnd( long frames ) const {
        assert( pimpl );
        return long( pimpl->seekEnd( sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::read( short *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->read( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::read( int *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->read( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::read( float *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->read( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::read( double *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->read( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::readf( short *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->readf( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::readf( int *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->readf( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::readf( float *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->readf( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::readf( double *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->readf( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::write( short *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->write( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::write( int *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->write( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::write( float *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->write( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::write( double *ptr, long items ) const {
        assert( pimpl );
        return long( pimpl->write( ptr, sf_count_t( items ) ) );
    }
    long SoundfileWrapper::writef( short *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->writef( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::writef( int *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->writef( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::writef( float *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->writef( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::writef( double *ptr, long frames ) const {
        assert( pimpl );
        return long( pimpl->writef( ptr, sf_count_t( frames ) ) );
    }
    long SoundfileWrapper::read_raw( void *ptr, long bytes ) const {
        assert( pimpl );
        return long( pimpl->read_raw( ptr, sf_count_t( bytes ) ) );
    }
    long SoundfileWrapper::write_raw( void *ptr, long bytes ) const {
        assert( pimpl );
        return long( pimpl->write_raw( ptr, sf_count_t( bytes ) ) );
    }
    void SoundfileWrapper::write_sync( ) const {
        assert( pimpl );
        pimpl->write_sync( );
    }

    // Getters of non-text data
	Audio::FrameCount SoundfileWrapper::getFrameNum( ) const {
        assert( pimpl );
		return Audio::FrameCount( pimpl->getFrameNum( ) );
    }
	Audio::Framerate SoundfileWrapper::getSamplerate( ) const {
        assert( pimpl );
        return Audio::Framerate{pimpl->getSamplerate( )};
    }
    Audio::ChannelCount SoundfileWrapper::getChannels( ) const {
        assert( pimpl );
        return Audio::ChannelCount{pimpl->getChannels( )};
    }
    int SoundfileWrapper::getFormat( ) const {
        assert( pimpl );
        return pimpl->getFormat( );
    }
    int SoundfileWrapper::getSections( ) const {
        assert( pimpl );
        return pimpl->getSections( );
    }
    int SoundfileWrapper::getSeekable( ) const {
        assert( pimpl );
        return pimpl->getSeekable( );
    }

    // Text getters
    const char *SoundfileWrapper::getTitle( ) const {
        assert( pimpl );
        return pimpl->getTitle( );
    }
    const char *SoundfileWrapper::getCopyright( ) const {
        assert( pimpl );
        return pimpl->getCopyright( );
    }
    const char *SoundfileWrapper::getSoftware( ) const {
        assert( pimpl );
        return pimpl->getSoftware( );
    }
    const char *SoundfileWrapper::getArtist( ) const {
        assert( pimpl );
        return pimpl->getArtist( );
    }
    const char *SoundfileWrapper::getComment( ) const {
        assert( pimpl );
        return pimpl->getComment( );
    }
    const char *SoundfileWrapper::getDate( ) const {
        assert( pimpl );
        return pimpl->getDate( );
    }
    const char *SoundfileWrapper::getAlbum( ) const {
        assert( pimpl );
        return pimpl->getAlbum( );
    }
    const char *SoundfileWrapper::getLicense( ) const {
        assert( pimpl );
        return pimpl->getLicense( );
    }
    const char *SoundfileWrapper::getTrackNumber( ) const {
        assert( pimpl );
        return pimpl->getTrackNumber( );
    }
    const char *SoundfileWrapper::getGenre( ) const {
        assert( pimpl );
        return pimpl->getGenre( );
    }

    // Text getters - C string
    int SoundfileWrapper::setTitle( const char *str ) const {
        assert( pimpl );
        return pimpl->setTitle( str );
    }
    int SoundfileWrapper::setCopyright( const char *str ) const {
        assert( pimpl );
        return pimpl->setCopyright( str );
    }
    int SoundfileWrapper::setSoftware( const char *str ) const {
        assert( pimpl );
        return pimpl->setSoftware( str );
    }
    int SoundfileWrapper::setArtist( const char *str ) const {
        assert( pimpl );
        return pimpl->setArtist( str );
    }
    int SoundfileWrapper::setComment( const char *str ) const {
        assert( pimpl );
        return pimpl->setComment( str );
    }
    int SoundfileWrapper::setDate( const char *str ) const {
        assert( pimpl );
        return pimpl->setDate( str );
    }
    int SoundfileWrapper::getAlbum( const char *str ) const {
        assert( pimpl );
        return pimpl->getAlbum( str );
    }
    int SoundfileWrapper::setTrackNumber( const char *str ) const {
        assert( pimpl );
        return pimpl->setTrackNumber( str );
    }
    int SoundfileWrapper::setLicense( const char *str ) const {
        assert( pimpl );
        return pimpl->setLicense( str );
    }
    int SoundfileWrapper::setGenre( const char *str ) const {
        assert( pimpl );
        return pimpl->setGenre( str );
    }

    // Text getters - C++ string
    int SoundfileWrapper::setTitle( const std::string &str ) const {
        return setTitle( str.c_str( ) );
    }
    int SoundfileWrapper::setCopyright( const std::string &str ) const {
        return setCopyright( str.c_str( ) );
    }
    int SoundfileWrapper::setSoftware( const std::string &str ) const {
        return setSoftware( str.c_str( ) );
    }
    int SoundfileWrapper::setArtist( const std::string &str ) const {
        return setArtist( str.c_str( ) );
    }
    int SoundfileWrapper::setComment( const std::string &str ) const {
        return setComment( str.c_str( ) );
    }
    int SoundfileWrapper::setDate( const std::string &str ) const {
        return setDate( str.c_str( ) );
    }
    int SoundfileWrapper::getAlbum( const std::string &str ) const {
        return getAlbum( str.c_str( ) );
    }
    int SoundfileWrapper::setTrackNumber( const std::string &str ) const {
        return setTrackNumber( str.c_str( ) );
    }
    int SoundfileWrapper::setLicense( const std::string &str ) const {
        return setLicense( str.c_str( ) );
    }
    int SoundfileWrapper::setGenre( const std::string &str ) const {
        return setGenre( str.c_str( ) );
    }

} // Namesapce Audio
} // namespace Mh

sf_count_t sfGetFilelen( void *user_data ) {
    return sf_count_t(
        reinterpret_cast< Abstract::pFIO >( user_data )->size( ) );
}
sf_count_t sfSeek( sf_count_t offset, int whence, void *user_data ) {
    auto chandle = reinterpret_cast< Abstract::pFIO >( user_data );
    sf_count_t cunt = 0;
    switch ( whence ) {
    case SEEK_SET:
        cunt =
            sf_count_t( chandle->seek( offset, Abstract::FIO::SeekPos::SET ) );
        break;
    case SEEK_CUR:
        cunt =
            sf_count_t( chandle->seek( offset, Abstract::FIO::SeekPos::CUR ) );
        break;
    case SEEK_END:
        cunt =
            sf_count_t( chandle->seek( offset, Abstract::FIO::SeekPos::END ) );
        break;
    default:
        cunt = -1;
    }
    if ( cunt != -1 )
        return chandle->tell( );
    else
        return -1;
}
sf_count_t sfRead( void *ptr, sf_count_t count, void *user_data ) {
    return sf_count_t(
        reinterpret_cast< Abstract::pFIO >( user_data )->read( ptr, count ) );
}
sf_count_t sfWrite( const void *ptr, sf_count_t count, void *user_data ) {
    return sf_count_t(
        reinterpret_cast< Abstract::pFIO >( user_data )->write( ptr, count ) );
}
sf_count_t sfTell( void *user_data ) {
    return sf_count_t(
        reinterpret_cast< Abstract::pFIO >( user_data )->tell( ) );
}
