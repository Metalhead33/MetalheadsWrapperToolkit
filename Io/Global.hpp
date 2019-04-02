#pragma once
#include <exception>
#include <memory>
#include <iostream>
#include <cstdint>
#include <thread>
#include <experimental/propagate_const>

#define MIN(a,b)             (((a) < (b)) ? (a) : (b)) // min: Choose smaller of two scalars.
#define MAX(a,b)             (((a) > (b)) ? (a) : (b)) // max: Choose greater of two scalars.
#define CLAMP(a, mi,ma)      min(max(a,mi),ma)         // clamp: Clamp value into set range.

#ifndef _MSC_VER
#define __enforce_inlining __attribute__((always_inline))
#elif
#define __enforce_inlining __forceinline
#endif

// From util.h - Written by Daniël Sonck
#define DEFINE_PTR(a) typedef a *p##a; \
	typedef std::shared_ptr< a > s##a; \
	typedef std::weak_ptr< a > w##a; \
	typedef std::unique_ptr< a > u##a; \
	typedef std::experimental::propagate_const<std::unique_ptr< a >> pimpl##a;

#define DEFINE_PTR2(s,a) typedef s::a *p##a; \
	typedef std::shared_ptr< s::a > s##a; \
	typedef std::weak_ptr< s::a > w##a; \
	typedef std::unique_ptr< a > u##a;

#define DEFINE_CLASS(klass) class klass; DEFINE_PTR(klass)

#define DEFINE_STRUCT(klass) struct klass; DEFINE_PTR(klass)

typedef std::shared_ptr<std::thread> ThreadPointer;

//End of former util.h

// Some useful defines
#define EXCEPTION(A,B) struct A : public std::exception \
{ \
  const char * what () const throw () \
  { \
	return B; \
  } \
};

#if defined(DEBUG) || defined(NDEBUG) ||defined(_DEBUG) || defined(DEBUG_BUILD)
#   define IF_DEBUG( ... )   __VA_ARGS__
#else
#   define IF_DEBUG( ... )
#endif /* DEBUG */
