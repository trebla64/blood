#ifndef __TILE_H__
#define __TILE_H__

int  tileInit( unsigned char flag );
void tileTerm();
void tileBlit(int nTile, int x, int y, int nShade);
unsigned char *tileAllocTile(int a, int b, int c);

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *tileLoadTile(int nTile);

#ifdef __cplusplus
}
#endif

#endif // __TILE_H__
