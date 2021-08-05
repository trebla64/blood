#ifndef __ENGINE_ACCESS_H__
#define __ENGINE_ACCESS_H__

#ifdef __cplusplus
extern "C" {
#endif

// db.cpp
void dbInitSpriteLists();
int  InsertSprite(int nSector, int nStat);
void DeleteSprite(int nSprite);
int  ChangeSpriteSect(int nSprite, int nNewSector);
int  ChangeSpriteStat(int nSprite, int nNewStat);

// Misc
void FakeTimerHandler();

#ifdef __cplusplus
}
#endif

#endif // __ENGINE_ACCESS_H__
