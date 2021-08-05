#include "stdafx.h"
#include "ini_file.h"
#include "debug.h"
#include "error.h"

#include "jfbuild/compat.h"


IniFile BloodINI("BLOOD.INI");


IniFile::IniFile()
{
}


IniFile::IniFile(const char *thefile)
{
//	dprintf("IniFile::IniFile(\"%s\")\n", thefile);
	section_head = NULL;
	section_tail = NULL;
	Load(thefile);
}


IniFile::~IniFile()
{
//	dprintf("IniFile::~IniFile\n");
	Free();
}


void IniFile::Free()
{
//	dprintf("IniFile::Free()\n");
	SectionNode *section, *section_next;
	for (section = section_head; section; section = section_next)
	{
		section_next = section->next;
		if (section->title)
		{
//			dprintf("Section \"%s\"\n", section->title);
			free(section->title);
		}
		// Free key/value nodes
		KeyValueNode *keyval, *keyval_next;
		for (keyval = section->key_head; keyval; keyval = keyval_next)
		{
			keyval_next = keyval->next;
//			dprintf("    \"%s\" = \"%s\"\n", keyval->key, keyval->value);
			if (keyval->key)
				free(keyval->key);
			if (keyval->value)
				free(keyval->value);
			free(keyval);
		}
		free(section);
	}
}


void IniFile::Load(const char *thefile)
{
	char line[256];
	char section[32];
	char key[64];
	char value[64];
	int section_pos;
	int key_pos;
	int value_pos;
	SectionNode *pSection = NULL;
	const char *begin;
//	const char *end;

//	dprintf("IniFile::Load(\"%s\")\n", thefile);

	FILE *fp = fopen(thefile, "rt");
	if (fp == NULL)
		ThrowError("Failed opening INI file %s", thefile);

	bool in_section = false;
	bool in_key = false;
	bool in_value = false;
	section_pos = 0;
	key_pos = 0;
	value_pos = 0;
	bool next_line = false;
	while (true)
	{
		if (!fgets(line, sizeof(line), fp))
			break;

		// Assume we are in a key
		in_key = true;
		in_value = false;

		for (const char *c = line; *c; c++)
		{
			switch (*c)
			{
				case '[':
					// Possible to have [] in value
					if (in_value)
						break;
					in_section = true;
					break;

				case ']':
				{
					// Possible to have [] in value
					if (in_value)
						break;
					in_section = false;
					pSection = AllocSection();
					pSection->title = (char *)malloc(section_pos + 1);
					if (pSection->title == NULL)
						ThrowError("Failed allocating memory for section title");
					memcpy(pSection->title, section, section_pos);
					pSection->title[section_pos] = '\0';
					section_pos = 0;
					break;
				}

				case '\n':
				{
					if (key_pos == 0 || value_pos == 0)
						break;
					KeyValueNode *pKeyValue = AllocKeyValue(pSection);
					begin = key;
					while ((*begin == ' ' || *begin == '\t') && begin != &key[key_pos])
						begin++;
					int key_len = &key[key_pos] - begin;
					pKeyValue->key = (char *)malloc(key_len + 1);
					if (pKeyValue->key == NULL)
						ThrowError("Failed allocating memory for key string");
					memcpy(pKeyValue->key, begin, key_len);
					pKeyValue->key[key_len] = '\0';
					key_pos = 0;

					begin = value;
					while ((*begin == ' ' || *begin == '\t') && begin != &value[value_pos])
						begin++;
					int value_len = &value[value_pos] - begin;
					pKeyValue->value = (char *)malloc(value_len + 1);
					if (pKeyValue->value == NULL)
						ThrowError("Failed allocating memory for value string");
					memcpy(pKeyValue->value, begin, value_len);
					pKeyValue->value[value_len] = '\0';
					value_pos = 0;
#if 0
					// Remove leading and trailing spaces from key/value
					begin = key;
					while ((*begin == ' ' || *begin == '\t') && begin != &key[key_pos])
						begin++;
					end = &key[key_pos - 1];
					while ((*end == ' ' || *end == '\t') && end >= &key[0])
						end--;
					end++;
					int key_len = end - begin;
					pKeyValue->key = (char *)malloc(key_len + 1);
					if (pKeyValue->key == NULL)
						ThrowError("Failed allocating memory for key string");
					memcpy(pKeyValue->key, begin, key_len);
					pKeyValue->key[key_len] = '\0';
					key_pos = 0;
					// Do the value
					begin = value;
					while ((*begin == ' ' || *begin == '\t') && begin != &value[value_pos])
						begin++;
					end = &value[value_pos - 1];
					while ((*end == ' ' || *end == '\t') && end >= &value[0])
						end--;
					end++;
					int value_len = end - begin;
					pKeyValue->value = (char *)malloc(value_len + 1);
					if (pKeyValue->value == NULL)
						ThrowError("Failed allocating memory for value string");
					memcpy(pKeyValue->value, begin, value_len);
					pKeyValue->value[value_len] = '\0';
					value_pos = 0;
#endif
					break;
				}

				case '=':
					in_key = false;
					in_value = true;
					break;

				case ';':
					next_line = true;
					break;

				// Just ignore carriage returns
				case '\r':
					break;

				case '\t':
				case ' ':
					// We don't accept spaces in section names or keys, so just
					// skip those
					if (in_section || in_key)
						break;
					// Fallthru
				default:
					if (in_section)
						section[section_pos++] = *c;
					else if (in_key)
						key[key_pos++] = *c;
					else if (in_value)
						value[value_pos++] = *c;
					break;
			}

			if (next_line)
			{
				next_line = false;
				break;
			}
		}
	}

	fclose(fp);
}


IniFile::SectionNode *IniFile::AllocSection()
{
	SectionNode *pNode = (SectionNode *)malloc(sizeof(SectionNode));
	if (pNode == NULL)
		ThrowError("Failed allocating memory for section node");
	memset(pNode, 0, sizeof(SectionNode));
	if (!section_head)
		section_head = section_tail = pNode;
	else
	{
		section_tail->next = pNode;
		pNode->prev = section_tail;
		section_tail = pNode;
	}
	return pNode;
}


IniFile::KeyValueNode *IniFile::AllocKeyValue(SectionNode *pSection)
{
	dassert(pSection != NULL);
	KeyValueNode *pNode = (KeyValueNode *)malloc(sizeof(KeyValueNode));
	if (pNode == NULL)
		ThrowError("Failed allocating memory for key/value node");
	memset(pNode, 0, sizeof(KeyValueNode));
	if (!pSection->key_head)
		pSection->key_head = pSection->key_tail = pNode;
	else
	{
		pSection->key_tail->next = pNode;
		pNode->prev = pSection->key_tail;
		pSection->key_tail = pNode;
	}
	return pNode;
}


bool IniFile::KeyExists(const char *section, const char *key)
{
//	dprintf("IniFile::KeyExists(%p, \"%s\", \"%s\")\n", this, section, key);
	for (const SectionNode *pSection = section_head; pSection; pSection = pSection->next)
		if (!Bstrcasecmp(pSection->title, section))
			for (const KeyValueNode *pKey = pSection->key_head; pKey; pKey = pKey->next)
				if (!Bstrcasecmp(pKey->key, key))
					return true;
	return false;
}


const char *IniFile::GetKeyString(const char *section, const char *key)
{
//	dprintf("IniFile::GetKeyString(%p, \"%s\", \"%s\")\n", this, section, key);
	for (const SectionNode *pSection = section_head; pSection; pSection = pSection->next)
		if (!Bstrcasecmp(pSection->title, section))
			for (const KeyValueNode *pKey = pSection->key_head; pKey; pKey = pKey->next)
				if (!Bstrcasecmp(pKey->key, key))
				{
//					dprintf("    Returning \"%s\"\n", pKey->value);
					return pKey->value;
				}
	return NULL;
}


bool IniFile::FindSection(const char *section)
{
//	dprintf("IniFile::FindSection(%p, \"%s\")\n", this, section);
	for (const SectionNode *pSection = section_head; pSection; pSection = pSection->next)
		if (!Bstrcasecmp(pSection->title, section))
			return true;
	return false;
}
