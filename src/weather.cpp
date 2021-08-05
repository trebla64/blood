#include "stdafx.h"
#include "weather.h"
#include "debug.h"

#include "jfbuild/build.h"


CWeather gWeather;


enum WEATHERTYPE
{
};


CWeather::CWeather()
{
}


CWeather::~CWeather()
{
}


int CWeather::RandomizeVectors()
{
	for (int i = 0; i < 4096; i++)	// 4096 = kMaxVectors
	{
		m_aVectors[i].x = krand() & 0x3F00;
		m_aVectors[i].y = krand() & 0x3F00;
		m_aVectors[i].z = krand() & 0x3F00;
	}
	return 0;
}


void CWeather::SetDefaultBuffer(unsigned char *pBuffer, int b, int c, int *pYLookup)
{
}


void CWeather::SetParticles(short, short, short)
{
}


void CWeather::SetTranslucency(int)
{
}


void CWeather::SetColor(unsigned char)
{
}


void CWeather::SetColorShift(unsigned char)
{
}


int CWeather::Initialize(unsigned char *pBuffer, int b, int c, int d, int e, int *pYLookup, short nCount, short h, short nTile)
{
//	dassert(pBuffer != NULL);	// FIXME
	dassert(pYLookup != NULL);
	dassert(nCount >= 0 && nCount < 4096);	// 4096 = kMaxVectors
	dassert(nTile == -1 || (nTile >= 0 && nTile < 4096));	// -1 = kNoTile, 4096 = kMaxTiles
	RandomizeVectors();
	return 0;
}


void CWeather::Draw(unsigned char *, int, int, int, int, int *, int, int, int, int, int, int, int)
{
}


void CWeather::Draw(int, int, int, int, int, int)
{
}


void CWeather::SetWeatherType(int nWeatherType)
{
}
