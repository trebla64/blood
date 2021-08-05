#include "stdafx.h"
#include "resource.h"
#include "error.h"
#include "debug.h"


int32 costable[2048];


void trigInit( Resource *res )
{
	ResHandle h = res->Lookup("cosine", "dat");
	if (h.isNull())
		ThrowError("Cosine table not found");
	if (h.size() != 2048)
		ThrowError("Cosine table incorrect size");
	memcpy(costable, res->Load(h), h.size());
	costable[512] = 0;

	for (int i = 513; i <= 1024;)
	{
		int32 nFoo = costable[1024 - i++];
		costable[i] = -nFoo;
	}

	for (int i = 1025; i < 2048;)
	{
		int nFoo = 2048 - i++;
		costable[i] = costable[nFoo];
	}
}
