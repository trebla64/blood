#ifndef __ERROR_H__
#define __ERROR_H__


#ifdef _MSC_VER
void __cdecl ThrowError(const char *err, ...);
#else
void __attribute__((cdecl)) ThrowError(const char *err, ...);
#endif


#endif // __ERROR_H__
