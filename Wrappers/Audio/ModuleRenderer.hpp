#ifndef MHMODULERENDERER_HPP
#define MHMODULERENDERER_HPP
#include "../../Io/FIO.hpp"
namespace MH33 {
namespace Audio {

    class ModuleRenderer {
      private:
        DEFINE_STRUCT( ModuleRenderer_impl )
        uModuleRenderer_impl pimpl;
        ModuleRenderer( const ModuleRenderer &cpy ) =
            delete; // Disallow copying
        void
        operator=( const ModuleRenderer &cpy ) = delete; // Disallow copying
      public:
        ~ModuleRenderer( );
        ModuleRenderer( Abstract::sFIO fio );
        ModuleRenderer( ModuleRenderer &&mov ); // Move constructor
        void operator=( ModuleRenderer &&mov ); // Move assignment operator

        // Getters and setters
        void setSubsong( int32_t subsong );
        int32_t getSubsong( ) const;
        void setRepeating( bool value );
        bool isRepeating( ) const;
        double getDuration( ) const;
        double setPosition( double seconds );
        double getPosition( ) const;
        void setMasterGain(
            int32_t
                value ); // set_render_param(OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL)
        int32_t getMasterGain( )
            const; // get_render_param(OPENMPT_MODULE_RENDER_MASTERGAIN_MILLIBEL)
        void setStereoSeparation(
            int32_t
                value ); // set_render_param(OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT)
        int32_t getStereoSeparation( )
            const; // get_render_param(OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT)
        void setInterpolationFilter(
            int32_t
                value ); // set_render_param(OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH)
        int32_t getInterpolationFilter( )
            const; // get_render_param(OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH)
        void setVolumeRamping(
            int32_t
                value ); // set_render_param(OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH)
        int32_t getVolumeRamping( )
            const; // get_render_param(OPENMPT_MODULE_RENDER_VOLUMERAMPING_STRENGTH)
        // Metadata
        const char *getType( ) const;         // get_metadata('type')
        const char *getTypeLong( ) const;     // get_metadata('type_long')
        const char *getOriginalType( ) const; // get_metadata('originaltype')
        const char *
        getOriginalTypeLong( ) const;      // get_metadata('originaltype_long')
        const char *getContainer( ) const; // get_metadata('container')
        const char *getContainerLong( ) const; // get_metadata('container_long')
        const char *getTracker( ) const;       // get_metadata('tracker')
        const char *getArtist( ) const;        // get_metadata('artist')
        const char *getTitle( ) const;         // get_metadata('title')
        const char *getDate( ) const;          // get_metadata('date')
        // Rendering
        size_t readMono( int32_t samplerate, size_t frames, int16_t *mono );
        size_t readMono( int32_t samplerate, size_t frames, float *mono );
        size_t readStereo( int32_t samplerate, size_t frames, int16_t *left,
                           int16_t *right );
        size_t readStereo( int32_t samplerate, size_t frames, float *left,
                           float *right );
        size_t readQuad( int32_t samplerate, size_t frames, int16_t *left,
                         int16_t *right, int16_t *rear_left,
                         int16_t *rear_right );
        size_t readQuad( int32_t samplerate, size_t frames, float *left,
                         float *right, float *rear_left, float *rear_right );
        size_t readInterleavedStereo( int32_t samplerate, size_t frames,
                                      int16_t *interleaved_stereo );
        size_t readInterleavedStereo( int32_t samplerate, size_t frames,
                                      float *interleaved_stereo );
        size_t readInterleavedQuad( int32_t samplerate, size_t frames,
                                    int16_t *interleaved_quad );
        size_t readInterleavedQuad( int32_t samplerate, size_t frames,
                                    float *interleaved_quad );
	};
} // Namespace Audio
} // namespace Mh
#endif // MHMODULERENDERER_HPP
