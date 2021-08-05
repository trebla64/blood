#include "stdafx.h"

#include "jfbuild/compat.h"


#ifdef _MSC_VER
void __cdecl DebugPrint( const char *fmt, ... )
#else
void __attribute__((cdecl)) DebugPrint( const char *fmt, ... )
#endif
{
	char buffer[2048];
	va_list argp;

	va_start( argp, fmt );
	Bvsnprintf( buffer, sizeof(buffer) - 1, fmt, argp );
	va_end( argp );

#ifdef _MSC_VER
	OutputDebugString( buffer );
#else
	fprintf(stderr, "%s", buffer);
#endif
}
