#ifndef __DEMO_H__
#define __DEMO_H__

struct NEW_INPUT;

class CDemo
{
public:
	CDemo();
	~CDemo();

	void Create(char *);
	void Write(NEW_INPUT *);
	void Close();
	void SetupPlayback(char *);
	void ProcessKeys();
	void Playback();
	void StopPlayback();
	void LoadDemoInfo();
	void NextDemo();
};

extern CDemo gDemo;

#endif // __DEMO_H__
