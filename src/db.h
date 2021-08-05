#ifndef __DB_H__
#define __DB_H__


void   InsertSpriteSect( int nSprite, int nSector );
void   RemoveSpriteSect( int nSprite );
void   InsertSpriteStat( int nSprite, int nStat );
void   RemoveSpriteStat( int nSprite );
void   dbDeleteXSprite( int nXSprite );
void   InitFreeList( unsigned short *ptr, int b );
void   dbInit();
void   PropogateMarketReferences( int nSprite );
unsigned int dbReadMapCRC( const char *map );
void   dbLoadMap( const char *map, int *posx, int *posy, int *posz, short *ang, short *sectnum, unsigned int *seed );
void   dbLoadExternalMap( const char *map, int *posx, int *posy, int *posz, int *ang, int *sectnum, unsigned int *seed );


#endif // __DB_H__
