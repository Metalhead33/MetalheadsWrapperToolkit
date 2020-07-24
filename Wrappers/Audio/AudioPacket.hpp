#ifndef AUDIOPACKET_HPP
#define AUDIOPACKET_HPP
#include <cstddef>
#include <cstdint>
#include <string>

namespace MH33 {
namespace Audio {

    template<class Simple>
    struct iterator_base{
        Simple v;

        Simple operator*() const { return v; }
        iterator_base<Simple>& operator++() { ++v; return *this; }

        bool operator!=(const iterator_base<Simple> &other) const { return v != other.v; }
    };

    enum class InterleavingType : std::int8_t {
        DONT_CARE = -1, // We don't really care
        INTERLACED =
            0, // Each channels are stored separately, channel by channel
        INTERLEAVED = 1 // Each channels are stored together frame by frame
    };
    // typedef std::uint32_t FrameCount;
    struct Framerate {
        typedef std::uint32_t Type;
        Type value;
        // operator std::uint32_t() { return value; }
        operator Type( ) const { return value; }
        operator double() const { return value; }
        operator std::int32_t() const { return value; }
        operator std::string() const { return std::to_string( value ); }

        explicit Framerate( Type value ) : value{ value } {}
        explicit Framerate( int value )
            : value{ static_cast< Type >( value ) } {}

        bool operator!=(Framerate other) const { return value != other.value; }
    };
    struct Sample {
        typedef std::uint32_t Type;
        Type value;
        // operator std::uint32_t() { return value; }
        operator Type( ) { return value; }

        static constexpr Sample Zero() { return Sample{0}; }

        bool operator!=(Sample other) const { return value != other.value; }
        Sample &operator++() {
            value++; return *this; }
    };
    struct Channel {
        typedef std::uint8_t Type;
        Type value;

        operator Type() { return value; }

        explicit constexpr Channel( Type channel) : value{ channel } {}
        explicit constexpr Channel(int channel) : value{ static_cast< Type >(channel) } {}

        static constexpr Channel Zero( ) {
            return Channel{ static_cast< Type >( 0 ) };
        }

        bool operator!=(Channel other) const { return value != other.value; }
        Channel &operator++() {
            value++; return *this; }
    };
    struct ChannelCount {
        typedef iterator_base<Channel> iterator;

        typedef std::uint8_t Type;
        Type value;
        // operator std::uint8_t() { return value; }
        operator Type &( ) { return value; }
        operator const Type &( ) const { return value; }

        explicit constexpr ChannelCount( Type channels) : value{ channels } {}
        explicit constexpr ChannelCount( int channels ) : value{ static_cast< Type >(channels) } {}

        static constexpr ChannelCount Zero( ) {
            return ChannelCount{ static_cast< Type >( 0 ) };
        }

        iterator begin() { return iterator{Channel::Zero()}; }
        iterator begin() const { return iterator{Channel::Zero()}; }
        iterator end() { return iterator{Channel{ value }}; }
        iterator end() const { return iterator{Channel{ value }}; }
    };
    struct Frame {
        struct BaseSample {
            typedef iterator_base<Sample> iterator;

            typedef std::uint32_t Type;
            Type value;
            ChannelCount count;

            Sample operator[](Channel c) const { return Sample{ value + c.value }; }

            iterator begin() { return iterator{Sample{ value } }; }
            iterator begin() const { return iterator{Sample{ value }}; }
            iterator end() { return iterator{Sample{ value +count.value }}; }
            iterator end() const { return iterator{Sample{ value +count.value }}; }

        };

        typedef std::uint32_t Type;
        Type value;
        // operator std::uint32_t() { return value; }
        operator Type( ) { return value; }

        constexpr BaseSample samples(ChannelCount cnt) const {
            return BaseSample{cnt.value * value, cnt};
        }

		static constexpr Frame Zero() { return Frame{0}; }

        bool operator!=(Frame other) const { return value != other.value; }
        Frame &operator++() {
            value++; return *this; }
    };

    struct FrameCount;

    struct SampleCount {
        typedef iterator_base<Sample> iterator;

        typedef std::uint32_t Type;
        Type value;
        // operator std::uint32_t() { return value; }
        operator Type() { return value; }
        inline constexpr FrameCount toFrames( ChannelCount channels ) const ;
        inline constexpr std::size_t toBytes( ) const {
            return value * sizeof( float );
        }

		inline constexpr SampleCount operator-( const SampleCount &other ) const {
            return SampleCount{ value - other.value };
        }
		inline constexpr SampleCount operator+( const SampleCount &other ) const {
            return SampleCount{ value + other.value };
        }
		inline constexpr SampleCount operator*( const SampleCount &other ) const {
			return SampleCount{ value * other.value };
		}
		inline constexpr SampleCount operator/( const SampleCount &other ) const {
			return SampleCount{ value / other.value };
		}
		inline constexpr SampleCount operator%( const SampleCount &other ) const {
			return SampleCount{ value % other.value };
		}

        iterator begin() { return iterator{Sample::Zero()}; }
        iterator begin() const { return iterator{Sample::Zero()}; }
        iterator end() { return iterator{Sample{ value }}; }
        iterator end() const { return iterator{Sample{ value }}; }

		inline constexpr bool operator <(SampleCount other) const { return value < other.value; }
		inline constexpr bool operator >(SampleCount other) const { return value > other.value; }
		inline constexpr bool operator <=(SampleCount other) const { return value <= other.value; }
		inline constexpr bool operator >=(SampleCount other) const { return value >= other.value; }
		inline constexpr bool operator ==(SampleCount other) const { return value == other.value; }
		inline constexpr bool operator !=(SampleCount other) const { return value == other.value; }
    };
    struct FrameCount {
        typedef iterator_base<Frame> iterator;

        typedef std::uint32_t Type;
        Type value;

        // operator std::uint32_t() { return value; }
        operator Type &( ) { return value; }
        operator const Type &( ) const { return value; }
        inline constexpr SampleCount toSamples( ChannelCount channels ) const {
			return SampleCount{ value * channels.value };
        }
        inline constexpr std::size_t toBytes( ChannelCount channels ) const {
            return toSamples( channels ).toBytes( );
        }

        explicit constexpr FrameCount( Type value ) : value{ value } {}
        explicit constexpr FrameCount( uint64_t value )
            : value{ static_cast< Type >( value ) } {}
        explicit constexpr FrameCount( long value )
            : value{ static_cast< Type >( value ) } {}
        explicit constexpr FrameCount( double value )
            : value{ static_cast< Type >( value ) } {}

		inline constexpr FrameCount operator-( const FrameCount &other ) const {
			return FrameCount{ value - other.value };
		}
		inline constexpr FrameCount operator+( const FrameCount &other ) const {
			return FrameCount{ value + other.value };
		}
		inline constexpr FrameCount operator*( const FrameCount &other ) const {
			return FrameCount{ value * other.value };
		}
		inline constexpr FrameCount operator/( const FrameCount &other ) const {
			return FrameCount{ value / other.value };
		}
		inline constexpr FrameCount operator%( const FrameCount &other ) const {
			return FrameCount{ value % other.value };
		}
		inline constexpr bool operator <(FrameCount other) const { return value < other.value; }
		inline constexpr bool operator >(FrameCount other) const { return value > other.value; }
		inline constexpr bool operator <=(FrameCount other) const { return value <= other.value; }
		inline constexpr bool operator >=(FrameCount other) const { return value >= other.value; }
		inline constexpr bool operator ==(FrameCount other) const { return value == other.value; }
		inline constexpr bool operator !=(FrameCount other) const { return value == other.value; }

        static constexpr FrameCount Zero( ) {
            return FrameCount{ static_cast< Type >( 0 ) };
        }

        iterator begin() { return iterator{Frame::Zero()}; }
        iterator begin() const { return iterator{Frame::Zero()}; }
        iterator end() { return iterator{Frame{ value }}; }
        iterator end() const { return iterator{Frame{ value }}; }
    };

	struct WrapperRW {
		struct FrameWrapper {
			float * const samples;
			const ChannelCount channelCnt;
			FrameWrapper(float* nsamples, ChannelCount nchannelCnt)
				: samples(nsamples), channelCnt(nchannelCnt)
			{

			}
			float& at(Channel chn) { return samples[chn]; }
			float& operator[](Channel chn) { return samples[chn]; }
			float* begin() { return samples; }
			float* end() { return &samples[channelCnt.end().v]; }
		};
		struct iterator{
			WrapperRW& parent;
			Frame frameId;

			FrameWrapper operator*() const { return parent.at(frameId); }
			iterator& operator++() { ++frameId; return *this; }

			bool operator!=(const iterator &other) const { return frameId != other.frameId; }
			iterator(WrapperRW& nparent,Frame nframe)
				: parent(nparent),frameId(nframe)
			{

			}
		};
		float * const samples;
		const FrameCount frameCnt;
		const ChannelCount channelCnt;
		WrapperRW(float* nsamples, FrameCount nframeCnt, ChannelCount nchannelCnt)
			: samples(nsamples), frameCnt(nframeCnt), channelCnt(nchannelCnt)
		{

		}
		FrameWrapper at(Frame frm) { return FrameWrapper(&samples[frm.value*channelCnt.value],channelCnt); }
		FrameWrapper operator[](Frame frm) { return FrameWrapper(&samples[frm.value*channelCnt.value],channelCnt); }
		iterator begin() { return iterator(*this,frameCnt.begin().v); }
		iterator end() { return iterator(*this,frameCnt.end().v); }

	};
	struct WrapperRO {
		struct FrameWrapper {
			const float * const samples;
			const ChannelCount channelCnt;
			FrameWrapper(const float* nsamples, ChannelCount nchannelCnt)
				: samples(nsamples), channelCnt(nchannelCnt)
			{

			}
			const float& at(Channel chn) const { return samples[chn]; }
			const float& operator[](Channel chn) const { return samples[chn]; }
			const float* begin() const { return samples; }
			const float* end() const { return &samples[channelCnt.end().v]; }
		};
		struct iterator{
			const WrapperRO& parent;
			Frame frameId;

			FrameWrapper operator*() const { return parent.at(frameId); }
			iterator& operator++() { ++frameId; return *this; }

			bool operator!=(const iterator &other) const { return frameId != other.frameId; }
			iterator(const WrapperRO& nparent,Frame nframe)
				: parent(nparent),frameId(nframe)
			{

			}
		};
		const float * const samples;
		const FrameCount frameCnt;
		const ChannelCount channelCnt;
		WrapperRO(const float* nsamples, FrameCount nframeCnt, ChannelCount nchannelCnt)
			: samples(nsamples), frameCnt(nframeCnt), channelCnt(nchannelCnt)
		{

		}
		const FrameWrapper at(Frame frm) const { return FrameWrapper(&samples[frm.value*channelCnt.value],channelCnt); }
		const FrameWrapper operator[](Frame frm) const { return FrameWrapper(&samples[frm.value*channelCnt.value],channelCnt); }
		iterator begin() const { return iterator(*this,frameCnt.begin().v); }
		iterator end() const { return iterator(*this,frameCnt.end().v); }

	};

    struct Output {

        float *dst;              // Destination
        FrameCount frameCnt;     // How many frames requested/allowed at max?
        Framerate frameRate;     // Sampling rate of the audio
        ChannelCount channelCnt; // How many channels?
        InterleavingType interleavingType; // Should channels be interleaved, or
                                           // should they be separated?
		WrapperRW getWrapper() { return WrapperRW(dst,frameCnt,channelCnt); }

        Output(
            float *ndst = nullptr, FrameCount nframeCnt = FrameCount::Zero( ),
            Framerate nframeRate = Framerate{ 0 },
            ChannelCount nChannelCnt = ChannelCount::Zero(),
            InterleavingType nInterleavingType = InterleavingType::DONT_CARE )
            : dst( ndst ), frameCnt( nframeCnt ), frameRate( nframeRate ),
              channelCnt( nChannelCnt ), interleavingType( nInterleavingType ) {

        }

        inline constexpr SampleCount sampleCnt( ) const {
            return frameCnt.toSamples( channelCnt );
        }
        inline constexpr std::size_t byteCnt( ) const {
            return sampleCnt( ).toBytes( );
        }
    };
    struct Input {
        const float *src;                  // Source
        FrameCount frameCnt;               // How many frames do we have?
        Framerate frameRate;               // Sampling rate of the audio
        ChannelCount channelCnt;           // How many channels do we have?
        InterleavingType interleavingType; // Is the source audio interleaved?
		WrapperRO getWrapper() { return WrapperRO(src,frameCnt,channelCnt); }

        explicit Input(
            const float *nsrc = nullptr,
            FrameCount nframeCnt = FrameCount::Zero( ),
            Framerate nframeRate = Framerate{ 0 },
            ChannelCount nChannelCnt = ChannelCount::Zero(),
            InterleavingType nInterleavingType = InterleavingType::DONT_CARE )
            : src( nsrc ), frameCnt( nframeCnt ), frameRate( nframeRate ),
              channelCnt( nChannelCnt ), interleavingType( nInterleavingType ) {

        }

        inline constexpr SampleCount sampleCnt( ) const {
            return frameCnt.toSamples( channelCnt );
        }
        inline constexpr std::size_t byteCnt( ) const {
            return sampleCnt( ).toBytes( );
        }
    };

    constexpr FrameCount SampleCount::toFrames(ChannelCount channels) const {
		return FrameCount{ value / channels.value };
    }

} // namespace Audio
}

#endif // AUDIOPACKET_HPP
