#ifndef MHVECTOR_HPP
#define MHVECTOR_HPP
namespace MH33 {
namespace GFX {
#if defined (_MSC_VER)
#pragma pack(push, 1)
#define PACKED
#elif defined (__GNUC__)
#define PACKED __attribute__((packed))
#endif

template <typename T> struct PACKED Vec2
{
	T x;
	T y;
};

template <typename T> struct PACKED Vec3
{
	T x;
	T y;
	T z;
};

#if defined (_MSC_VER)
#pragma pack(pop)
#endif
}
}
#endif // MHVECTOR_HPP
