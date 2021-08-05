#ifndef __RESOURCE_H__
#define __RESOURCE_H__

struct ResHandlePrivate;
struct DictEntry;

class ResHandle
{
public:
	friend class Resource;

	ResHandle(DictEntry *pNode = NULL);
	~ResHandle();

	const ResHandle &operator = (const ResHandle &rhs);

	unsigned size() const;
	bool     isNull() const;

private:
	ResHandlePrivate * const d;
};


class Resource
{
	DictEntry * dict;
	DictEntry **indexName;
	DictEntry **indexId;
	int        dictSize;
	int        dictEntries;
	FILE *     fileHandle;
	bool       encrypted;

	// Private methods
	DictEntry **Probe( const char *name, const char *type );
	DictEntry **Probe( uint32 nId, const char *type );

public:
	Resource();
	~Resource();

	void       Init( const char *file, int unknown );
	void       Reindex();
	void       Free( void *p );
	void *     Lock( const ResHandle &h );
	void *     Lock( DictEntry *h );
	void *     Load( const ResHandle &h );
	void *     Load( DictEntry *h );
	void *     Alloc( int size );
	void       Read( const ResHandle &h );
	void       Read( DictEntry *h );
	ResHandle  Lookup( const char *name, const char *type );
	ResHandle  Lookup( uint32 nId, const char *type );
	void       Unlock( const ResHandle &h );
};


extern Resource gSysRes;
extern Resource gGuiRes;
extern Resource gSoundRes;


#endif // __RESOURCE_H__
