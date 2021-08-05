// cache1d.h

#ifndef __cache1d_h__
#define __cache1d_h__

#ifdef __cplusplus
extern "C" {
#endif

void	initcache(int32_t dacachestart, int32_t dacachesize);
void	allocache(intptr_t *newhandle, int32_t newbytes, char *newlockptr);
void	suckcache(int32_t *suckptr);
void	agecache(void);

int     addsearchpath(const char *p);
int     openfrompath(const char *fn, int flags, int mode);
BFILE  *fopenfrompath(const char *fn, const char *mode);

int32_t	initgroupfile(char *filename);
void	uninitsinglegroupfile(int32_t grphandle);
void	uninitgroupfile(void);
int32_t	kopen4load(char *filename, char searchfirst);	// searchfirst: 0 = anywhere, 1 = first group, 2 = any group
int32_t	kread(int32_t handle, void *buffer, int32_t leng);
int32_t	klseek(int32_t handle, int32_t offset, int32_t whence);
int32_t	kfilelength(int32_t handle);
int32_t	ktell(int32_t handle);
void	kclose(int32_t handle);

int	beginsearchgroup(char *ext);
int	getsearchgroupnext(char *name, int32_t *size);

int	kdfread(void *buffer, bsize_t dasizeof, bsize_t count, int32_t fil);
int	dfread(void *buffer, bsize_t dasizeof, bsize_t count, BFILE *fil);
void	dfwrite(void *buffer, bsize_t dasizeof, bsize_t count, BFILE *fil);

int32_t	lzwcompress(char *lzwinbuf, int32_t uncompleng, char *lzwoutbuf);
int32_t	lzwuncompress(char *lzwinbuf, int32_t compleng, char *lzwoutbuf);

#ifdef __cplusplus
}
#endif

#endif // __cache1d_h__

