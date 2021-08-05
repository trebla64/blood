#include "stdafx.h"
#include "globals.h"
#include "resource.h"
#include "pal.h"
#include "error.h"
#include "debug.h"

#include "jfbuild/compat.h"


Resource gSysRes;
Resource gGuiRes;
Resource gSoundRes;

#define RFF_SIGNATURE "RFF\x1A"

#pragma pack(push, 1)

struct RFFHeader
{
	uint8  signature[4];
	uint16 version;
	uint16 unknown0;
	uint32 dict_offset;
	uint32 dict_entries;
	uint32 unknown2;
	uint32 unknown3;
	uint32 unknown4;
	uint32 unknown5;
};
CT_ASSERT( sizeof(RFFHeader) == 32 );

struct DICTENTRY
{
//	void *    ptr;
	uint32    ptr;
//	DICTENTRY *prev;
	uint32    unused1;
//	DICTENTRY *next;
	uint32    unused2;
	int       count;
	uint32    offset;
	uint32    size;
	uint8     more_crap[8];
	uint8     flags;
	uint8     type[3];
	uint8     name[8];
	uint32    index;
};
CT_ASSERT(sizeof(DICTENTRY) == 48);

#pragma pack(pop)


struct DictEntry
{
	void  *ptr;
	uint32 size;
	uint8  flags;
	char   name[8];
	char   type[3];
	uint32 index;
	uint32 offset;
};


struct ResHandlePrivate
{
	DictEntry *dict;
};


ResHandle::ResHandle(DictEntry *pNode)
	: d(new ResHandlePrivate)
{
	d->dict = pNode;
}


ResHandle::~ResHandle()
{
	delete d;
}


bool ResHandle::isNull() const
{
	return (d->dict == NULL);
}


uint32 ResHandle::size() const
{
	return d->dict->size;
}


const ResHandle &ResHandle::operator = (const ResHandle &rhs)
{
	d->dict = rhs.d->dict;
	return *this;
}


static uint32 table[256] = {
	0x0,
	0x77073096,
	0x0EE0E612C,
	0x990951BA,
	0x76DC419,
	0x706AF48F,
	0x0E963A535,
	0x9E6495A3,
	0x0EDB8832,
	0x79DCB8A4,
	0x0E0D5E91E,
	0x97D2D988,
	0x9B64C2B,
	0x7EB17CBD,
	0x0E7B82D07,
	0x90BF1D91,
	0x1DB71064,
	0x6AB020F2,
	0x0F3B97148,
	0x84BE41DE,
	0x1ADAD47D,
	0x6DDDE4EB,
	0x0F4D4B551,
	0x83D385C7,
	0x136C9856,
	0x646BA8C0,
	0x0FD62F97A,
	0x8A65C9EC,
	0x14015C4F,
	0x63066CD9,
	0x0FA0F3D63,
	0x8D080DF5,
	0x3B6E20C8,
	0x4C69105E,
	0x0D56041E4,
	0x0A2677172,
	0x3C03E4D1,
	0x4B04D447,
	0x0D20D85FD,
	0x0A50AB56B,
	0x35B5A8FA,
	0x42B2986C,
	0x0DBBBC9D6,
	0x0ACBCF940,
	0x32D86CE3,
	0x45DF5C75,
	0x0DCD60DCF,
	0x0ABD13D59,
	0x26D930AC,
	0x51DE003A,
	0x0C8D75180,
	0x0BFD06116,
	0x21B4F4B5,
	0x56B3C423,
	0x0CFBA9599,
	0x0B8BDA50F,
	0x2802B89E,
	0x5F058808,
	0x0C60CD9B2,
	0x0B10BE924,
	0x2F6F7C87,
	0x58684C11,
	0x0C1611DAB,
	0x0B6662D3D,
	0x76DC4190,
	0x1DB7106,
	0x98D220BC,
	0x0EFD5102A,
	0x71B18589,
	0x6B6B51F,
	0x9FBFE4A5,
	0x0E8B8D433,
	0x7807C9A2,
	0x0F00F934,
	0x9609A88E,
	0x0E10E9818,
	0x7F6A0DBB,
	0x86D3D2D,
	0x91646C97,
	0x0E6635C01,
	0x6B6B51F4,
	0x1C6C6162,
	0x856530D8,
	0x0F262004E,
	0x6C0695ED,
	0x1B01A57B,
	0x8208F4C1,
	0x0F50FC457,
	0x65B0D9C6,
	0x12B7E950,
	0x8BBEB8EA,
	0x0FCB9887C,
	0x62DD1DDF,
	0x15DA2D49,
	0x8CD37CF3,
	0x0FBD44C65,
	0x4DB26158,
	0x3AB551CE,
	0x0A3BC0074,
	0x0D4BB30E2,
	0x4ADFA541,
	0x3DD895D7,
	0x0A4D1C46D,
	0x0D3D6F4FB,
	0x4369E96A,
	0x346ED9FC,
	0x0AD678846,
	0x0DA60B8D0,
	0x44042D73,
	0x33031DE5,
	0x0AA0A4C5F,
	0x0DD0D7CC9,
	0x5005713C,
	0x270241AA,
	0x0BE0B1010,
	0x0C90C2086,
	0x5768B525,
	0x206F85B3,
	0x0B966D409,
	0x0CE61E49F,
	0x5EDEF90E,
	0x29D9C998,
	0x0B0D09822,
	0x0C7D7A8B4,
	0x59B33D17,
	0x2EB40D81,
	0x0B7BD5C3B,
	0x0C0BA6CAD,
	0x0EDB88320,
	0x9ABFB3B6,
	0x3B6E20C,
	0x74B1D29A,
	0x0EAD54739,
	0x9DD277AF,
	0x4DB2615,
	0x73DC1683,
	0x0E3630B12,
	0x94643B84,
	0x0D6D6A3E,
	0x7A6A5AA8,
	0x0E40ECF0B,
	0x9309FF9D,
	0x0A00AE27,
	0x7D079EB1,
	0x0F00F9344,
	0x8708A3D2,
	0x1E01F268,
	0x6906C2FE,
	0x0F762575D,
	0x806567CB,
	0x196C3671,
	0x6E6B06E7,
	0x0FED41B76,
	0x89D32BE0,
	0x10DA7A5A,
	0x67DD4ACC,
	0x0F9B9DF6F,
	0x8EBEEFF9,
	0x17B7BE43,
	0x60B08ED5,
	0x0D6D6A3E8,
	0x0A1D1937E,
	0x38D8C2C4,
	0x4FDFF252,
	0x0D1BB67F1,
	0x0A6BC5767,
	0x3FB506DD,
	0x48B2364B,
	0x0D80D2BDA,
	0x0AF0A1B4C,
	0x36034AF6,
	0x41047A60,
	0x0DF60EFC3,
	0x0A867DF55,
	0x316E8EEF,
	0x4669BE79,
	0x0CB61B38C,
	0x0BC66831A,
	0x256FD2A0,
	0x5268E236,
	0x0CC0C7795,
	0x0BB0B4703,
	0x220216B9,
	0x5505262F,
	0x0C5BA3BBE,
	0x0B2BD0B28,
	0x2BB45A92,
	0x5CB36A04,
	0x0C2D7FFA7,
	0x0B5D0CF31,
	0x2CD99E8B,
	0x5BDEAE1D,
	0x9B64C2B0,
	0x0EC63F226,
	0x756AA39C,
	0x26D930A,
	0x9C0906A9,
	0x0EB0E363F,
	0x72076785,
	0x5005713,
	0x95BF4A82,
	0x0E2B87A14,
	0x7BB12BAE,
	0x0CB61B38,
	0x92D28E9B,
	0x0E5D5BE0D,
	0x7CDCEFB7,
	0x0BDBDF21,
	0x86D3D2D4,
	0x0F1D4E242,
	0x68DDB3F8,
	0x1FDA836E,
	0x81BE16CD,
	0x0F6B9265B,
	0x6FB077E1,
	0x18B74777,
	0x88085AE6,
	0x0FF0F6A70,
	0x66063BCA,
	0x11010B5C,
	0x8F659EFF,
	0x0F862AE69,
	0x616BFFD3,
	0x166CCF45,
	0x0A00AE278,
	0x0D70DD2EE,
	0x4E048354,
	0x3903B3C2,
	0x0A7672661,
	0x0D06016F7,
	0x4969474D,
	0x3E6E77DB,
	0x0AED16A4A,
	0x0D9D65ADC,
	0x40DF0B66,
	0x37D83BF0,
	0x0A9BCAE53,
	0x0DEBB9EC5,
	0x47B2CF7F,
	0x30B5FFE9,
	0x0BDBDF21C,
	0x0CABAC28A,
	0x53B39330,
	0x24B4A3A6,
	0x0BAD03605,
	0x0CDD70693,
	0x54DE5729,
	0x23D967BF,
	0x0B3667A2E,
	0x0C4614AB8,
	0x5D681B02,
	0x2A6F2B94,
	0x0B40BBE37,
	0x0C30C8EA1,
	0x5A05DF1B,
	0x2D02EF8D
};

uint32 CRC( uint32 count, const uint8 *buffer )
{
	uint32 ret = 0xFFFFFFFF;

	for ( const uint8 *p = buffer; p < &buffer[count]; p++ )
	{
		uint8 b = *p;
		b ^= (ret & 0xFF);
		ret >>= 8;
		ret ^= table[b];
	}
	return ~ret;
}

static void DecryptFunction( uint8 *buffer, unsigned count, unsigned key )
{
	unsigned v5;
	for (unsigned i = 0; i < count; buffer[i - 1] = v5 ^ BYTEn(v5, 1)) {
		v5 = (unsigned short)key / 2;
		BYTEn(v5, 1) = buffer[i++];
		key++;
	}
}


Resource::Resource()
{
	dict        = NULL;
	indexName   = NULL;
	indexId     = NULL;
	dictSize    = 0;
	dictEntries = 0;
	fileHandle  = NULL;
}


Resource::~Resource()
{
	if (dict != NULL)
	{
		// Free the remaining data
		for (int i = 0; i < dictEntries; i++)
		{
			DictEntry *pNode = &dict[i];
			if (pNode->ptr)
			{
				free(pNode->ptr);
				pNode->ptr = NULL;
			}
		}

		Free(dict);
		dict = NULL;
		dictSize = 0;
		dictEntries = 0;
	}

	// Free the index arrays
	if (indexName)
	{
		free(indexName);
		indexName = NULL;
	}

	if (indexId)
	{
		free(indexId);
		indexId = NULL;
	}

	if (fileHandle != NULL)
	{
		fclose(fileHandle);
		fileHandle = NULL;
	}
}


void Resource::Init( const char *file, int unknown )
{
	RFFHeader header;

	dprintf( "Resource::Init(\"%s\")\n", file );
	if (!file)
		return;

	fileHandle = fopen(file, "rb");
	if (fileHandle == NULL)
		ThrowError("Failed opening file %s", file);

//	FileRead( fileHandle, &header, sizeof(RFFHeader) );
	fread( &header, sizeof(RFFHeader), 1, fileHandle );
	if ( strncmp( (const char *)header.signature, RFF_SIGNATURE, 4 ) )
	{
		fclose( fileHandle );
		ThrowError( "RFF Header corrupted" );
	}
	if (this == &gSysRes)
		dprintf("RFF Version [blood.rff]: %04X\n", header.version);
	uint16 hi_version = header.version & 0xFF00;
	if (hi_version == 768)
		encrypted = true;
	else
	{
		if (hi_version == 512)
			encrypted = false;
		else
		{
			fclose(fileHandle);
			ThrowError("Unknown RFF version\n");
		}
	}
//	dprintf(
//		"    Read header successfully:\n"
//		"        Version:      %d.%d\n"
//		"        Dict offset:  %d\n"
//		"        Dict entries: %d\n"
//		, (header.version & 0xFF00) >> 8
//		, header.version & 0xFF
//		, header.dict_offset
//		, header.dict_entries
//	);
	// TODO: Fix the version check
//	if ( (header.version & 0xFF00) != 512 )
//	{
//		fclose( fileHandle );
//		dprintf( "Wrong RFF version\n" );
//		ThrowError( "Wrong RFF version" );
//	}

	// Allocate dictionary
	dictEntries = header.dict_entries;
	if ( dictEntries )
	{
		// We keep doubling the size until we have enough to store all
		// dictionary entries
		dictSize = 1;
		while ( dictSize < dictEntries )
			dictSize *= 2;
//		dprintf( "Allocating %d dictionary entries\n", dictSize );

		// Allocate, clear, and read the nodes
		DICTENTRY *tmp_dict = (DICTENTRY *)malloc( dictSize * sizeof(DICTENTRY) );
		if ( tmp_dict == NULL )
			ThrowError("Failed allocating memory for temporary disk dictionary");

		memset( tmp_dict, 0, dictSize * sizeof(DICTENTRY) );
		fseek( fileHandle, header.dict_offset, SEEK_SET );
		fread( tmp_dict, sizeof(DICTENTRY), dictEntries, fileHandle );
		// Decrypt it maybe
		if (encrypted)
			DecryptFunction( (uint8 *)tmp_dict, dictEntries * sizeof(DICTENTRY), header.dict_offset + header.dict_offset * (header.version & 0xFF) );

		// Copy disk dictionary to the real dictionary
		dict = new DictEntry[dictSize];
		memset(dict, 0, dictSize * sizeof(DictEntry));

		const DICTENTRY *pSrc = tmp_dict;
		DictEntry *pDst = dict;
		for (int i = 0; i < dictEntries; i++)
		{
			pDst->size  = pSrc->size;
			pDst->flags = pSrc->flags;
			memcpy(pDst->name, pSrc->name, 8);
			memcpy(pDst->type, pSrc->type, 3);
			pDst->index  = pSrc->index;
			pDst->offset = pSrc->offset;
			pDst++;
			pSrc++;
		}

		// Free temporary dictionary
		free(tmp_dict);

//		const DICTENTRY *p = dict;
//		for ( int i = 0; i < dictEntries; i++ )
//		{
//			if (this == &gSoundRes && !memcmp(p->type, "SFX", 3))
//			{
//				dprintf("%.8s.%.3s (Index %d)\n", p->name, p->type, p->index);
//			}
//			if (!memcmp(p->type, "QFN", 3))
//			{
//				dprintf("%.8s.%.3s\n", p->name, p->type);
//			}
//			dprintf( "%.8s.%.3s, offs = %u, size = %u, external = %s, encrypted = %s\n"
//			, p->name, p->type, p->offset, p->size, p->flags & 0x2 ? "Yes" : "No", p->flags & 0x10 ? "Yes" : "No");
//			uint8 *buf = (uint8 *)malloc(p->size);
//			fseek( fileHandle, p->offset, SEEK_SET );
//			fread(buf, p->size, 1, fileHandle);
//			if ( p->flags & 0x10 )
//				DecryptFunction(buf, p->size <= 256 ? p->size : 256, 0);
//			char filename[128];
//			Bsnprintf( filename, sizeof(filename), "C:\\BloodFiles\\%.8s.%.3s", p->name, p->type );
//			FILE *fp = fopen(filename, "wb");
//			if (fp)
//			{
//				fwrite(buf, p->size, 1, fp);
//				fflush(fp);
//				fclose(fp);
//			}
//			free(buf);
//			p++;
//		}
	}

	if (dict == NULL)
	{
		dictSize = 16;
//		dict = (DICTENTRY *)Alloc(sizeof(DICTENTRY) * dictSize);
//		memset(dict, 0, sizeof(DICTENTRY) * dictSize);
		dict = new DictEntry[dictSize];
		memset(dict, 0, sizeof(DictEntry) * dictSize);
	}

	Reindex();

	DictEntry *pNode = dict;
	for (int i = 0; i < dictEntries; i++)
	{
		if (pNode->flags & 8)
			Lock(pNode);
		if (pNode->flags & 4)
			Load(pNode);
		pNode++;
	}
}


DictEntry **Resource::Probe( const char *name, const char *type )
{
//	dprintf("Resource::Probe(\"%.8s\", \"%.3s\")\n", name, type);
	dassert( indexName != NULL );
	dassert( dict != NULL );

	// Prepare the key
	char key[12];
	memset(key, 0, sizeof(key));
	Bsnprintf(key, sizeof(key), "%.3s%.8s", type, name);
	char copy_key[12];
	memcpy(copy_key, key, sizeof(key));
	int len = 0;
	char *c = key;
	for (;;)
	{
		if (*c == '\0' || c >= &key[11])
			break;
		if (*c >= 'a' && *c <= 'z')
			*c = *c - 'a' + 'A';
		len++;
		c++;
	}

	uint32 crc = CRC(len, (const uint8 *)key);
	uint32 i = (dictSize - 1) & crc;
	uint32 probe_end = i;

	DictEntry **pNode = NULL;
	for (;;)
	{
		pNode = &indexName[i];
		if (*pNode == NULL)
			break;
		char match_key[12];
		memset(match_key, 0, sizeof(match_key));
		Bsnprintf(match_key, sizeof(match_key), "%.3s%.8s", (*pNode)->type, (*pNode)->name);
		if (memcmp(key, match_key, 12) == 0)
//		if (!strncmp((const char *)(*pNode)->name, &search[3], 8) && !strncmp((const char *)(*pNode)->type, search, 3))
//			return pNode;
			break;
		i++;
		if (i == dictSize)
			i = 0;
		if (i == probe_end)
			ThrowError("Linear probe failed to find match or unused node!");
	}

	return pNode;
}


DictEntry **Resource::Probe( uint32 nId, const char *type )
{
	char key[7];

	dassert(indexName != NULL);
	memset(key, 0, sizeof(key));
	Bsnprintf(key, sizeof(key), "%.3s", type);
	*(uint32 *)&key[3] = nId;
	dassert(dict != NULL);
	uint32 crc = CRC(sizeof(key), (const uint8 *)key);
	uint32 i = (dictSize - 1) & crc;
	uint32 probe_end = i;

	DictEntry **pNode = NULL;
	for (;;)
	{
		pNode = &indexId[i];
		if (*pNode == NULL)
			break;
		char match_key[7];
		memset(match_key, 0, sizeof(match_key));
		Bsnprintf(match_key, sizeof(match_key), "%.3s", (*pNode)->type);
		*(uint32 *)&match_key[3] = (*pNode)->index;
		if (!memcmp(key, match_key, sizeof(key)))
			break;
		i++;
		if (i == dictSize)
			i = 0;
		if (i == probe_end)
			ThrowError("Linear probe failed to find match or unused node!");
	}
	return pNode;
}


void Resource::Reindex()
{
//	dprintf("Resource::Reindex()\n");
	if ( indexName )
		Free( indexName );
//	indexName = (DICTENTRY **)Alloc(4 * dictSize);
	indexName = new DictEntry *[dictSize];
	memset(indexName, 0, dictSize * sizeof(DictEntry *));
	for (int i = 0; i < dictEntries; i++)
	{
		DictEntry **pNode = Probe((const char *)dict[i].name, (const char *)dict[i].type);
		*pNode = &dict[i];
	}

	if ( indexId )
		Free( indexId );
//	indexId = (DICTENTRY **)Alloc(4 * dictSize);
	indexId = new DictEntry *[dictSize];
	memset(indexId, 0, dictSize * sizeof(DictEntry *));
	for (int i = 0; i < dictEntries; i++)
	{
		DictEntry *pFoo = &dict[i];
		if (pFoo->flags & 1)
		{
			DictEntry **pNode = Probe(pFoo->index, (const char *)pFoo->type);
			*pNode = &dict[i];
		}
	}

/*
	dprintf("After Reindex:\n");
	for (int i = 0; i < dictSize; i++)
	{
		DICTENTRY *pNode = indexName[i];
		if (pNode)
		{
			dprintf("Slot %d, Node %p, Name %.8s.%.3s\n", i, pNode, pNode->name, pNode->type);
		}
	}
*/
}


void Resource::Free( void *p )
{
//	dprintf("Resource::Free(%p)\n", p);
	dassert( p != NULL );
//	heap->Free( p );
	free(p);
}


void *Resource::Lock( const ResHandle &h )
{
#if 0
//	dprintf("Resource::Lock(%p [%.8s.%.3s])\n", h, h->name, h->type);
	dassert( !h.isNull() );
	DictEntry *pNode = h.d->dict;
	if (pNode->ptr)
	{
//		if (h->count == 0)
//		{
//			DICTENTRY *next = h->next;
//			h->prev->next = next;
//			next->prev = h->prev;
//		}
	}
	else
	{
		pNode->ptr = Alloc(pNode->size);
		Read(h);
	}
//	h->count++;
	return pNode->ptr;
#endif
	return Lock(h.d->dict);
}


void *Resource::Lock( DictEntry *h )
{
	if (h->ptr)
	{
	}
	else
	{
		h->ptr = Alloc(h->size);
		Read(h);
	}
	return h->ptr;
}


void *Resource::Load( const ResHandle &h )
{
#if 0
//	dprintf("Resource::Load(%p [%.8s.%.3s])\n", h, h->name, h->type);
	dassert( !h.isNull() );
	DictEntry *pNode = h.d->dict;
	if (pNode->ptr)
	{
	}
	else
	{
		pNode->ptr = Alloc(pNode->size);
		Read(h);
	}
	return pNode->ptr;
#endif
	return Load(h.d->dict);
}


void *Resource::Load( DictEntry *h )
{
	if (!h->ptr)
	{
		h->ptr = Alloc(h->size);
		Read(h);
	}
	return h->ptr;
}


void *Resource::Alloc( int size )
{
//	dprintf("Resource::Alloc(%d)\n", size);
	void *p = malloc(size);
	if (p == NULL)
		ThrowError("Failed allocating %d bytes", size);
	return p;
}


ResHandle Resource::Lookup( const char *name, const char *type )
{
//	dprintf("Resource::Lookup(\"%s\", \"%s\")\n", name, type);
	dassert(name != NULL);
	dassert(type != NULL);
//	char name_upr[16];
//	char type_upr[16];
//	Bsnprintf(name_upr, sizeof(name_upr), "%s", name);
//	Bsnprintf(type_upr, sizeof(type_upr), "%s", type);
//	strupr(name_upr);
//	strupr(type_upr);
//	DictEntry *pNode = *Probe(name, type);
//	dprintf("    Returning %p\n", pNode);
	return ResHandle(*Probe(name, type));;
}


ResHandle Resource::Lookup( uint32 nId, const char *type )
{
	dassert(type != NULL);
	return ResHandle(*Probe(nId, type));
}


void Resource::Read( DictEntry *pNode )
{
//	dassert(!n.isNull());
//	DictEntry *pNode = n.d->dict;
	if (pNode->flags & 2)
	{
		// This is an external resource
		dprintf("Reading external resource file: %.8s.%.3s\n", pNode->name, pNode->type);
	}
	else
	{
		fseek(fileHandle, pNode->offset, SEEK_SET);
		fread(pNode->ptr, pNode->size, 1, fileHandle);
		if (pNode->flags & 0x10)
			DecryptFunction((uint8 *)pNode->ptr, pNode->size <= 256 ? pNode->size : 256, 0);
	}
}


void Resource::Read( const ResHandle &h)
{
	Read(h.d->dict);
}


void Resource::Unlock( const ResHandle &h )
{
}
