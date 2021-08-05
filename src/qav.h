#ifndef __QAV_H__
#define __QAV_H__


#define MAX_QAV_FRAMES 32


#pragma pack(push, 1)

struct QAVINNERFRAME
{
	int   nPic;		// 4
	int   field_4;	// 8
	int   field_8;	// 12
	int   field_C;	// 16
	int   field_10;	// 20
	int8  field_14;	// 21
	uint8 nPalette; // 22
	int16 field_16;	// 24
};
CT_ASSERT(sizeof(QAVINNERFRAME) == 24);

struct QAVFRAME
{
	int           field_0;	// 4
	int           field_4;	// 8
	int           field_8;	// 12
	QAVINNERFRAME aInnerFrames[8];
};
CT_ASSERT(sizeof(QAVFRAME) == (8*24+12));

struct QAVDATA
{
	int field_0;		// 4
	int field_4;		// 8
	int nFrames;		// 12
	int ticksPerFrame;	// 16
	int duration;		// 20
	int field_14;		// 24
	int field_18;		// 28
	int field_1C;		// 32
	int field_20;		// 36
	QAVFRAME aFrames[MAX_QAV_FRAMES];
};

CT_ASSERT(sizeof(QAVDATA) == (36+MAX_QAV_FRAMES*sizeof(QAVFRAME)));

#pragma pack(pop)


class QAV
{
private:
//	int field_0;
//	int field_4;
//	int nFrames;
//	int ticksPerFrame;
//	int field_10;
//	int field_14;
//	int field_18;
	QAVDATA *pData;

public:
	QAV(QAVDATA *data) : pData(data)	{}

	void Draw(int a, int b, int c);
	void Play(int a, int b, int nCallback, void *pArg);
	void Preload();

	inline int GetDuration() const	{ return pData->duration; }
};


#endif // __QAV_H__
