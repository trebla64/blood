#include "stdafx.h"


#ifdef _MSC_VER
void __cdecl ThrowError(const char *err, ...)
#else
void __attribute__((cdecl)) ThrowError(const char *err, ...)
#endif
{
	char buf[1024];
	va_list argp;

	va_start(argp, err);
	vsnprintf(buf, sizeof(buf), err, argp);
	va_end(argp);

//	MessageBox(NULL, buf, "Error", MB_ICONERROR);
	fprintf(stderr, "%s", buf);
//	ExitProcess(1);
	exit(1);
}
