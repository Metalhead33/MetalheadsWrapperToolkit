#include "ModuleRenderer.hpp"
#include <libopenmpt/libopenmpt.h>

#include <utility>

size_t vfio_openmpt_read( void *stream, void *dst, size_t bytes );
int vfio_openmpt_seek( void *stream, int64_t offset, int whence );
int64_t vfio_openmpt_tell( void *stream );

static openmpt_stream_callbacks OpenmptIO = {
    vfio_openmpt_read, vfio_openmpt_seek, vfio_openmpt_tell };

namespace MH33 {
namespace Audio {

    struct ModuleRenderer::ModuleRenderer_impl {
        typedef std::unique_ptr< openmpt_module,
                                 decltype( &openmpt_module_destroy ) >
            ModulePtr;
        ModulePtr mod;
        Abstract::sFIO fio;
        ModuleRenderer_impl( const ModuleRenderer_impl &cpy ) = delete;
        void operator=( const ModuleRenderer_impl &cpy ) = delete;
        ModuleRenderer_impl( ModuleRenderer_impl &&mov )
            : mod( std::move( mov.mod ) ), fio( std::move( mov.fio ) ) {}
        void operator=( ModuleRenderer_impl &&mov ) {
            this->mod = std::move( mov.mod );
            this->fio = std::move( mov.fio );
        }
        ModuleRenderer_impl( )
            : mod( nullptr, openmpt_module_destroy ), fio( nullptr ) {}
        ModuleRenderer_impl( Abstract::sFIO nfio )
            : mod( openmpt_module_create2( OpenmptIO, nfio.get( ), nullptr,
                                           nullptr, nullptr, nullptr, nullptr,
                                           nullptr, nullptr ),
                   openmpt_module_destroy ),
              fio( nfio ) {}
        int select_subsong( int32_t subsong ) {
            if ( mod )
                return openmpt_module_select_subsong( mod.get( ), subsong );
            else
                return 0;
        }
        int32_t get_selected_subsong( ) const {
            if ( mod )
                return openmpt_module_get_selected_subsong( mod.get( ) );
            else
                return 0;
        }
        int set_repeat_count( int32_t repeat_count ) {
            if ( mod )
                return openmpt_module_set_repeat_count( mod.get( ),
                                                        repeat_count );
            else
                return 0;
        }
        int32_t get_repeat_count( ) const {
            if ( mod )
                return openmpt_module_get_repeat_count( mod.get( ) );
            else
                return 0;
        }
        double get_duration_seconds( ) const {
            if ( mod )
                return openmpt_module_get_duration_seconds( mod.get( ) );
            else
                return 0.0;
        }
        double set_position_seconds( double seconds ) {
            if ( mod )
                return openmpt_module_set_position_seconds( mod.get( ),
                                                            seconds );
            else
                return 0.0;
        }
        double get_position_seconds( ) const {
            if ( mod )
                return openmpt_module_get_position_seconds( mod.get( ) );
            else
                return 0.0;
        }
        double set_position_order_row( int32_t order, int32_t row ) const {
            if ( mod )
                return openmpt_module_set_position_order_row( mod.get( ), order,
                                                              row );
            else
                return 0.0;
        }
        int get_render_param( int param ) const {
            int32_t returner;
            if ( mod )
                if ( openmpt_module_get_render_param( mod.get( ), param,
                                                      &returner ) )
                    return returner;
                else
                    return 0;
            else
                return 0;
        }
        int set_render_param( int param, int32_t value ) {
            if ( mod )
                return openmpt_module_set_render_param( mod.get( ), param,
                                                        value );
            else
                return 0;
        }
        size_t read_mono( int32_t samplerate, size_t count, int16_t *mono ) {
            if ( mod )
                return openmpt_module_read_mono( mod.get( ), samplerate, count,
                                                 mono );
            else
                return 0;
        }
        size_t read_stereo( int32_t samplerate, size_t count, int16_t *left,
                            int16_t *right ) {
            if ( mod )
                return openmpt_module_read_stereo( mod.get( ), samplerate,
                                                   count, left, right );
            else
                return 0;
        }
        size_t read_quad( int32_t samplerate, size_t count, int16_t *left,
                          int16_t *right, int16_t *rear_left,
                          int16_t *rear_right ) {
            if ( mod )
                return openmpt_module_read_quad( mod.get( ), samplerate, count,
                                                 left, right, rear_left,
                                                 rear_right );
            else
                return 0;
        }
        size_t read_float_mono( int32_t samplerate, size_t count,
                                float *mono ) {
            if ( mod )
                return openmpt_module_read_float_mono( mod.get( ), samplerate,
                                                       count, mono );
            else
                return 0;
        }
        size_t read_float_stereo( int32_t samplerate, size_t count, float *left,
                                  float *right ) {
            if ( mod )
                return openmpt_module_read_float_stereo( mod.get( ), samplerate,
                                                         count, left, right );
            else
                return 0;
        }
        size_t read_float_quad( int32_t samplerate, size_t count, float *left,
                                float *right, float *rear_left,
                                float *rear_right ) {
            if ( mod )
                return openmpt_module_read_float_quad( mod.get( ), samplerate,
                                                       count, left, right,
                                                       rear_left, rear_right );
            else
                return 0;
        }
        size_t read_interleaved_stereo( int32_t samplerate, size_t count,
                                        int16_t *interleaved_stereo ) {
            if ( mod )
                return openmpt_module_read_interleaved_stereo(
                    mod.get( ), samplerate, count, interleaved_stereo );
            else
                return 0;
        }
        size_t read_interleaved_quad( int32_t samplerate, size_t count,
                                      int16_t *interleaved_quad ) {
            if ( mod )
                return openmpt_module_read_interleaved_quad(
                    mod.get( ), samplerate, count, interleaved_quad );
            else
                return 0;
        }
        size_t read_interleaved_float_stereo( int32_t samplerate, size_t count,
                                              float *interleaved_stereo ) {
            if ( mod )
                return openmpt_module_read_interleaved_float_stereo(
                    mod.get( ), samplerate, count, interleaved_stereo );
            else
                return 0;
        }
        size_t read_interleaved_float_quad( int32_t samplerate, size_t count,
                                            float *interleaved_quad ) {
            if ( mod )
                return openmpt_module_read_interleaved_float_quad(
                    mod.get( ), samplerate, count, interleaved_quad );
            else
                return 0;
        }
        const char *get_metadata_keys( ) {
            if ( mod )
                return openmpt_module_get_metadata_keys( mod.get( ) );
            else
                return nullptr;
        }
        const char *get_metadata( const char *key ) {
            if ( mod )
                return openmpt_module_get_metadata( mod.get( ), key );
            else
                return nullptr;
        }
        int32_t get_current_speed( ) {
            if ( mod )
                return openmpt_module_get_current_speed( mod.get( ) );
            else
                return 0;
        }
        int32_t get_current_tempo( ) {
            if ( mod )
                return openmpt_module_get_current_tempo( mod.get( ) );
            else
                return 0;
        }
        int32_t get_current_order( ) {
            if ( mod )
                return openmpt_module_get_current_order( mod.get( ) );
            else
                return 0;
        }
        int32_t get_current_pattern( ) {
            if ( mod )
                return openmpt_module_get_current_pattern( mod.get( ) );
            else
                return 0;
        }
        int32_t get_current_row( ) {
            if ( mod )
                return openmpt_module_get_current_row( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_subsongs( ) {
            if ( mod )
                return openmpt_module_get_num_subsongs( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_channels( ) {
            if ( mod )
                return openmpt_module_get_num_channels( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_orders( ) {
            if ( mod )
                return openmpt_module_get_num_orders( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_patterns( ) {
            if ( mod )
                return openmpt_module_get_num_patterns( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_instruments( ) {
            if ( mod )
                return openmpt_module_get_num_instruments( mod.get( ) );
            else
                return 0;
        }
        int32_t get_num_samples( ) {
            if ( mod )
                return openmpt_module_get_num_samples( mod.get( ) );
            else
                return 0;
        }
        const char *get_subsong_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_subsong_name( mod.get( ), index );
            else
                return nullptr;
        }
        const char *get_channel_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_channel_name( mod.get( ), index );
            else
                return nullptr;
        }
        const char *get_order_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_order_name( mod.get( ), index );
            else
                return nullptr;
        }
        const char *get_pattern_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_pattern_name( mod.get( ), index );
            else
                return nullptr;
        }
        const char *get_instrument_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_instrument_name( mod.get( ), index );
            else
                return nullptr;
        }
        const char *get_sample_name( int32_t index ) {
            if ( mod )
                return openmpt_module_get_sample_name( mod.get( ), index );
            else
                return nullptr;
        }
        // Deprecated function!
        /*const char *ctl_get( const char *ctl ) {
            if ( mod )
                return openmpt_module_ctl_get( mod.get( ), ctl );
            else
                return nullptr;
        }*/
        // Deprecated function!
        /*int ctl_set( const char *ctl, const char *value ) {
            if ( mod )
                return openmpt_module_ctl_set( mod.get( ), ctl, value );
            else
                return 0;
        }*/
    };
    ModuleRenderer::~ModuleRenderer( ) = default;
    ModuleRenderer::ModuleRenderer( Abstract::sFIO fio )
        : pimpl{ new ModuleRenderer_impl( std::move( fio ) ) } {}

    ModuleRenderer::ModuleRenderer( ModuleRenderer &&mov )
        : pimpl( std::move( mov.pimpl ) ) {}

    void ModuleRenderer::operator=( ModuleRenderer &&mov ) {
        this->pimpl = std::move( mov.pimpl );
    }
    // Getters and setters
    void ModuleRenderer::setSubsong( int32_t subsong ) {
        pimpl->select_subsong( subsong );
    }
    int32_t ModuleRenderer::getSubsong( ) const {
        return pimpl->get_selected_subsong( );
    }
    void ModuleRenderer::setRepeating( bool value ) {
        pimpl->set_repeat_count( ( value ) ? -1 : 0 );
    }
    bool ModuleRenderer::isRepeating( ) const {
        return pimpl->get_repeat_count( ) != 0;
    }
    double ModuleRenderer::getDuration( ) const {
        return pimpl->get_duration_seconds( );
    }
    double ModuleRenderer::setPosition( double seconds ) {
        return pimpl->set_position_seconds( seconds );
    }
    double ModuleRenderer::getPosition( ) const {
        return pimpl->get_position_seconds( );
    }
    void ModuleRenderer::setMasterGain( int32_t value ) {
        pimpl->set_render_param( OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL,
                                 value );
    } // set_render_param(OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL)
    int32_t ModuleRenderer::getMasterGain( ) const {
        return pimpl->get_render_param(
            OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL );
    } // get_render_param(OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL)
    void ModuleRenderer::setStereoSeparation( int32_t value ) {
        pimpl->set_render_param( OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT,
                                 value );
    } // set_render_param(OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT)
    int32_t ModuleRenderer::getStereoSeparation( ) const {
        return pimpl->get_render_param(
            OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT );
    } // get_render_param(OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT)
    void ModuleRenderer::setInterpolationFilter( int32_t value ) {
        pimpl->set_render_param(
            OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, value );
    } // set_render_param(OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH)
    int32_t ModuleRenderer::getInterpolationFilter( ) const {
        return pimpl->get_render_param(
            OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH );
    } // get_render_param(OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH)
    void ModuleRenderer::setVolumeRamping( int32_t value ) {
        pimpl->set_render_param( OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH,
                                 value );

    } // set_render_param(OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH)
    int32_t ModuleRenderer::getVolumeRamping( ) const {
        return pimpl->get_render_param(
            OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH );
    } // get_render_param(OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH)
    // Metadata
    const char *ModuleRenderer::getType( ) const {
        return pimpl->get_metadata( "type" );
    } // get_metadata('type')
    const char *ModuleRenderer::getTypeLong( ) const {
        return pimpl->get_metadata( "type_long" );
    } // get_metadata('type_long')
    const char *ModuleRenderer::getOriginalType( ) const {
        return pimpl->get_metadata( "originaltype" );
    } // get_metadata('originaltype')
    const char *ModuleRenderer::getOriginalTypeLong( ) const {
        return pimpl->get_metadata( "originaltype_long" );
    } // get_metadata('originaltype_long')
    const char *ModuleRenderer::getContainer( ) const {
        return pimpl->get_metadata( "container" );
    } // get_metadata('container')
    const char *ModuleRenderer::getContainerLong( ) const {
        return pimpl->get_metadata( "container_long" );
    } // get_metadata('container_long')
    const char *ModuleRenderer::getTracker( ) const {
        return pimpl->get_metadata( "tracker" );
    } // get_metadata('tracker')
    const char *ModuleRenderer::getArtist( ) const {
        return pimpl->get_metadata( "artist" );
    } // get_metadata('artist')
    const char *ModuleRenderer::getTitle( ) const {
        return pimpl->get_metadata( "title" );
    } // get_metadata('title')
    const char *ModuleRenderer::getDate( ) const {
        return pimpl->get_metadata( "date" );
    } // get_metadata('date')
    // Rendering
    size_t ModuleRenderer::readMono( int32_t samplerate, size_t frames,
                                     int16_t *mono ) {
        return pimpl->read_mono( samplerate, frames, mono );
    }
    size_t ModuleRenderer::readMono( int32_t samplerate, size_t frames,
                                     float *mono ) {
        return pimpl->read_float_mono( samplerate, frames, mono );
    }
    size_t ModuleRenderer::readStereo( int32_t samplerate, size_t frames,
                                       int16_t *left, int16_t *right ) {
        return pimpl->read_stereo( samplerate, frames, left, right );
    }
    size_t ModuleRenderer::readStereo( int32_t samplerate, size_t frames,
                                       float *left, float *right ) {
        return pimpl->read_float_stereo( samplerate, frames, left, right );
    }
    size_t ModuleRenderer::readQuad( int32_t samplerate, size_t frames,
                                     int16_t *left, int16_t *right,
                                     int16_t *rear_left, int16_t *rear_right ) {
        return pimpl->read_quad( samplerate, frames, left, right, rear_left,
                                 rear_right );
    }
    size_t ModuleRenderer::readQuad( int32_t samplerate, size_t frames,
                                     float *left, float *right,
                                     float *rear_left, float *rear_right ) {
        return pimpl->read_float_quad( samplerate, frames, left, right,
                                       rear_left, rear_right );
    }
    size_t
    ModuleRenderer::readInterleavedStereo( int32_t samplerate, size_t frames,
                                           int16_t *interleaved_stereo ) {
        return pimpl->read_interleaved_stereo( samplerate, frames,
                                               interleaved_stereo );
    }
    size_t ModuleRenderer::readInterleavedStereo( int32_t samplerate,
                                                  size_t frames,
                                                  float *interleaved_stereo ) {
        return pimpl->read_interleaved_float_stereo( samplerate, frames,
                                                     interleaved_stereo );
    }
    size_t ModuleRenderer::readInterleavedQuad( int32_t samplerate,
                                                size_t frames,
                                                int16_t *interleaved_quad ) {
        return pimpl->read_interleaved_quad( samplerate, frames,
                                             interleaved_quad );
    }
    size_t ModuleRenderer::readInterleavedQuad( int32_t samplerate,
                                                size_t frames,
                                                float *interleaved_quad ) {
        return pimpl->read_interleaved_float_quad( samplerate, frames,
                                                   interleaved_quad );
	}

} // Namesapce Audio
} // namespace Mh

size_t vfio_openmpt_read( void *stream, void *dst, size_t bytes ) {
    return size_t( reinterpret_cast< Abstract::pFIO >( stream )->read(
        dst, int64_t( bytes ) ) );
}
int vfio_openmpt_seek( void *stream, int64_t offset, int whence ) {
    Abstract::pFIO chandle = reinterpret_cast< Abstract::pFIO >( stream );
    switch ( whence ) {
    case OPENMPT_STREAM_SEEK_SET:
        return int( chandle->seek( offset, Abstract::FIO::SeekPos::SET ) );
    case OPENMPT_STREAM_SEEK_CUR:
        return int( chandle->seek( offset, Abstract::FIO::SeekPos::CUR ) );
    case OPENMPT_STREAM_SEEK_END:
        return int( chandle->seek( offset, Abstract::FIO::SeekPos::END ) );
    default:
        return -1;
    }
}
int64_t vfio_openmpt_tell( void *stream ) {
    return int64_t( reinterpret_cast< Abstract::pFIO >( stream )->tell( ) );
}
