#ifndef __WEATHER_H__
#define __WEATHER_H__


struct vec3_16_t
{
	int16 x, y, z;
};


class CWeather
{
public:
	vec3_16_t m_aVectors[4096];	// 4096 = kMaxVectors

public:
	CWeather();
	~CWeather();

	int  RandomizeVectors();
	void SetDefaultBuffer(unsigned char *pBuffer, int b, int c, int *pYLookup);
	void SetParticles(short, short, short);
	void SetTranslucency(int);
	void SetColor(unsigned char);
	void SetColorShift(unsigned char);
	int  Initialize(unsigned char *pBuffer, int b, int c, int d, int e, int *pYLookup, short nCount, short h, short nTile);
	void Draw(unsigned char *, int, int, int, int, int *, int, int, int, int, int, int, int);
	void Draw(int, int, int, int, int, int);
	void SetWeatherType(int);
};


extern CWeather gWeather;


#endif // __WEATHER_H__
