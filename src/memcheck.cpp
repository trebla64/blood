#include "stdafx.h"
#include "debug4g.h"
#include "error.h"

#ifdef ENABLE_MEMORY_LEAK_DETECTION
#	undef malloc
#	undef free
#endif

struct AllocationNode
{
	AllocationNode *prev;
	AllocationNode *next;
	void    *pMemory;
	unsigned nSize;
	const char *pzFunction;
	const char *pzFile;
	unsigned    nLine;
};

static AllocationNode *head_alloc = NULL;
static AllocationNode *tail_alloc = NULL;

void *malloc_tracked(unsigned size, const char *pzFunction, const char *pzFile, unsigned nLine)
{
	void *pMem = malloc(size);
	if (pMem == NULL)
		ThrowError("Failed allocating managed memory");

	// Alloc node
	AllocationNode *pNode = (AllocationNode *)malloc(sizeof(AllocationNode));
	if (pNode == NULL)
		ThrowError("Failed allocating allocation node in managed memory");
	memset(pNode, 0, sizeof(AllocationNode));

	if (!head_alloc)
	{
		head_alloc = tail_alloc = pNode;
	}
	else
	{
		pNode->prev = tail_alloc;
		tail_alloc->next = pNode;
		tail_alloc = pNode;
	}

	pNode->pMemory    = pMem;
	pNode->nSize      = size;
	pNode->pzFunction = pzFunction;
	pNode->pzFile     = pzFile;
	pNode->nLine      = nLine;

	return pMem;
}


void free_tracked(void *pBuffer, const char *pzFunction, const char *pzFile, unsigned nLine)
{
	AllocationNode *pNode, *pNode_next;

	bool found = false;
	for (pNode = head_alloc; pNode; pNode = pNode_next)
	{
		pNode_next = pNode->next;
		if (pBuffer == pNode->pMemory)
		{
			// Found it
			if (pNode->prev)
				pNode->prev->next = pNode->next;
			if (pNode->next)
				pNode->next->prev = pNode->prev;
			found = true;
			break;
		}
	}

	if (!found)
		ThrowError("Attempting to free memory in file %s, line %u that was never allocated", pzFile, nLine);

	free(pBuffer);
}


void DumpLeaks()
{
	FILE *fout = fopen("leaks.txt", "wt");
	if (!fout) return;
	unsigned nTotalLeaked = 0;
	for (AllocationNode *pNode = head_alloc; pNode; pNode = pNode->next)
	{
		fprintf(fout, "%p    %10u    %15s    %15s    %5u\n", pNode->pMemory, pNode->nSize, pNode->pzFunction, pNode->pzFile, pNode->nLine);
		nTotalLeaked += pNode->nSize;
	}
	fprintf(fout, "\nLeaked a total of %u bytes\n", nTotalLeaked);
	fflush(fout);
	fclose(fout);
}
