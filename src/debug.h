#ifndef __DEBUG_H__
#define __DEBUG_H__


#ifdef _DEBUG
#	define dassert assert
#	define dprintf DebugPrint
#else
#	define dassert /* NULL */
#	define dprintf /* NULL */
#endif


#ifdef _MSC_VER
void __cdecl DebugPrint( const char *fmt, ... );
#else
void __attribute__((cdecl)) DebugPrint( const char *fmt, ... );
#endif


#endif // __DEBUG_H__
