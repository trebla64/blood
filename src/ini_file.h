#ifndef __INI_FILE_H__
#define __INI_FILE_H__

class IniFile
{
	struct KeyValueNode
	{
		char *key;
		char *value;
		KeyValueNode *next;
		KeyValueNode *prev;
	};

	struct SectionNode
	{
		char *title;
		SectionNode *next;
		SectionNode *prev;
		KeyValueNode *key_head;
		KeyValueNode *key_tail;
	};

	SectionNode *section_head;
	SectionNode *section_tail;

	IniFile();
	void Free();
	void Load(const char *thefile);
	SectionNode *AllocSection();
	KeyValueNode *AllocKeyValue(SectionNode *pSection);

public:
	IniFile(const char *thefile);
	~IniFile();

	bool  FindSection(const char *section);
	const char *GetKeyString(const char *section, const char *key);
	bool  KeyExists(const char *section, const char *key);
};

extern IniFile BloodINI;

#endif // __INI_FILE_H__
