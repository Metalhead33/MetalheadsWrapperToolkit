#ifndef MHRESAMPLER_HPP
#define MHRESAMPLER_HPP
#include "../../Io/Global.hpp"
namespace MH33 {
namespace Audio {

    enum class ResampleType : int {
        SINC_BEST_QUALITY = 0,
        SINC_MEDIUM_QUALITY = 1,
        SINC_FASTEST = 2,
        ZERO_ORDER_HOLD = 3,
        LINEAR = 4
    };

    DEFINE_CLASS( SimpleResampler )
    class SimpleResampler {
      private:
        DEFINE_STRUCT( SimpleResamplerImpl )
        uSimpleResamplerImpl pimpl;

      public:
        ~SimpleResampler( ) = default;
        SimpleResampler( );
        SimpleResampler( float *dataIn, long input_frames, float *dataOut,
                         long outputFrames, double src_ratio );
        SimpleResampler( const SimpleResampler &cpy );
        SimpleResampler( SimpleResampler &&mov );
        void operator=( const SimpleResampler &cpy );
        void operator=( SimpleResampler &&mov );

        int resample( ResampleType resamplerType, int channels ) const;
        const float *getDataIn( ) const;
        float *getDataOut( ) const;
        long getInputFrames( ) const;
        long getOutputFrames( ) const;
        double getRatio( ) const;
        bool getEndOfInput( ) const;
        void setDataIn( const float *set_to ) const;
        void setDataOut( float *set_to ) const;
        void setInputFrames( long set_to ) const;
        void setOutputFrames( long set_to ) const;
        void setRatio( double set_to ) const;
        void setEndOfInput( bool set_to ) const;
        void set( const float *data_in, float *data_out, long input_frames,
                  long output_frames, double ratio ) const;
    };

    DEFINE_CLASS( FullResampler )
    class FullResampler {
      private:
        DEFINE_STRUCT( FullResamplerImpl )
        uFullResamplerImpl pimpl;

      public:
        ~FullResampler( ) = default;
        FullResampler( );
        FullResampler( int channels, ResampleType converterType =
                                         ResampleType::SINC_FASTEST );
        FullResampler( const FullResampler &cpy );
        FullResampler( FullResampler &&mov );
        void operator=( const FullResampler &cpy );
        void operator=( FullResampler &&mov );
        int reset( ) const;
        int process( ) const;
        const float *getDataIn( ) const;
        float *getDataOut( ) const;
        long getInputFrames( ) const;
        long getOutputFrames( ) const;
        double getRatio( ) const;
        bool getEndOfInput( ) const;
        void setDataIn( const float *set_to ) const;
        void setDataOut( float *set_to ) const;
        void setInputFrames( long set_to ) const;
        void setOutputFrames( long set_to ) const;
        void setRatio( double set_to ) const;
        void setEndOfInput( bool set_to ) const;
        void set( const float *data_in, float *data_out, long input_frames,
                  long output_frames, double ratio ) const;

        int getChannelCount( ) const;
        ResampleType getResamplerType( ) const;
    };

    typedef long ( *samplerate_callback )( void *cb_data, float **data );

    DEFINE_CLASS( CallbackInterfaceBase )
    class CallbackInterfaceBase {
      public:
        typedef std::pair< long, float * > CallbackArgs;
        virtual ~CallbackInterfaceBase( ) = default;
        virtual CallbackArgs callBack( ) = 0;
    };

    DEFINE_CLASS( CallbackResampler )
    class CallbackResampler {
      private:
        DEFINE_STRUCT( CallbackResamplerImpl )
        uCallbackResamplerImpl pimpl;

      public:
        virtual ~CallbackResampler( ) = default;
        CallbackResampler( );
        CallbackResampler( CallbackResampler &&mov );
        void operator=( CallbackResampler &&mov );
        CallbackResampler( samplerate_callback func,
                           ResampleType converter_type, int channels,
                           void *userdata );
        CallbackResampler( CallbackInterfaceBase *interface,
                           ResampleType converter_type, int channels );
        long read( double src_ratio, long frames, float *data ) const;
        int reset( ) const;
        int getChannelCount( ) const;
        ResampleType getResamplerType( ) const;
    };

    DEFINE_CLASS( CallbackInterface )
    class CallbackInterface : public CallbackInterfaceBase,
                              public CallbackResampler {
      public:
        virtual ~CallbackInterface( ) = default;
        CallbackInterface( int channels = 1, ResampleType converter_type =
                                                 ResampleType::SINC_FASTEST );
        virtual CallbackArgs callBack( ) = 0;
    };

} // namespace Audio
} // namespace Mh
#endif // MHRESAMPLER_HPP
