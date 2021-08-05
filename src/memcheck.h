#ifndef __MEMCHECK_H__
#define __MEMCHECK_H__

#define malloc(x)	malloc_tracked(x, __FUNCTION__, __FILE__, __LINE__)
#define free(x)		free_tracked(x, __FUNCTION__, __FILE__, __LINE__)

void *malloc_tracked(unsigned size, const char *pzFunction, const char *pzFile, unsigned nLine);
void  free_tracked(void *pBuffer, const char *pzFunction, const char *pzFile, unsigned nLine);
void  DumpLeaks();

#endif // __MEMCHECK_H__
