#ifndef __STDAFX_H__
#define __STDAFX_H__

#ifdef _MSC_VER
#	define STRICT
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

//#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER
#	include "jfbuild/msvc/inttypes.h"
#else
#	include <inttypes.h>
#endif

// FreeTypei
#include <ft2build.h>
#include FT_FREETYPE_H

// These need to be before "mmgr.h"
//#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <assert.h>
//#include <string.h>
#include <time.h>
//#include <stdarg.h>
//#include <new>
//
//#ifndef	WIN32
//#include <unistd.h>
//#endif

#ifdef _MSC_VER
#	pragma warning(disable : 4996)
#	pragma warning(disable : 4117)

#	define snprintf _snprintf
#endif

//#define STRESS_TEST
//#include "mmgr.h"

#define NUM_ELEMENTS(array)	(sizeof(array) / sizeof(array[0]))

//#if defined(__STDCXX_VERSION__) || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(__GXX_EXPERIMENTAL_CPP0X__) || defined(static_assert) || (defined(__clang__) && __has_feature(cxx_static_assert))
//#	define assert_compile(expr) static_assert(expr, #expr )
//#else
//#	define assert_compile(expr) typedef int __ct_assert__[1 - 2 * !(expr)]
#define CT_ASSERT(expr) typedef int __ct_assert__[1 - 2 * !(expr)]
//#endif

//#define CT_ASSERT assert_compile

//#if 1i
//#	define CT_ASSERT(expr)	static_assert(expr, #expr)
//#else
//#	define CT_ASSERT(expr) typedef int __ct_assert__[1 - 2 * !(expr)]
//#endif

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned int    uint32;
//typedef unsigned _int64 uint64;
typedef unsigned char   byte;

typedef signed char   int8;
typedef signed short  int16;
typedef signed int    int32;
//typedef signed _int64 int64;

// 64-bit type
#ifdef _MSC_VER
	typedef unsigned _int64 uint64;
	typedef signed   _int64 int64;
#elif __GNUC__
	typedef unsigned long long uint64;
	typedef signed long long   int64;
#else
#	error Unsupported compiler
#endif

CT_ASSERT(sizeof(uint8)  == 1);
CT_ASSERT(sizeof(uint16) == 2);
CT_ASSERT(sizeof(uint32) == 4);
CT_ASSERT(sizeof(uint64) == 8);

CT_ASSERT(sizeof(int8)  == 1);
CT_ASSERT(sizeof(int16) == 2);
CT_ASSERT(sizeof(int32) == 4);
CT_ASSERT(sizeof(int64) == 8);

CT_ASSERT(sizeof(byte) == 1);
CT_ASSERT(sizeof(bool) == 1);

#define BYTEn(x, n)	(*((byte *)&(x)+n))
#define BYTE2(x)	BYTEn(x, 2)
#define LODWORD(x)	(*((uint32 *)&(x)))
#define HIDWORD(x)	(*((uint32 *)&(x)+1))

// Engine types
typedef signed char    schar;
typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef signed short   sshort;
//typedef unsigned int   ulong;

//#define INT32_MAX int32(2147483647)
//#define INT32_MIN int32(-2147483648)

#define BIT_ISSET(value, bit)	(value & (1 << bit))

#define Clamp(a, min, max)	((a < min) ? min : ((a > max) ? max : a))

#endif // __STDAFX_H__
