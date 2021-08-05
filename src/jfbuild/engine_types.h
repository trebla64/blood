#ifndef __ENGINE_TYPES_H__
#define __ENGINE_TYPES_H__

#define CT_ASSERT(expr)	void __ct_assert(int a[!!(expr) * 2 - 1])

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

typedef signed char  int8;
typedef signed short int16;
typedef signed int   int32;

CT_ASSERT(sizeof(uint8)  == 1);
CT_ASSERT(sizeof(uint16) == 2);
CT_ASSERT(sizeof(uint32) == 4);

CT_ASSERT(sizeof(int8)  == 1);
CT_ASSERT(sizeof(int16) == 2);
CT_ASSERT(sizeof(int32) == 4);

#endif /* ENGINE_TYPES */
