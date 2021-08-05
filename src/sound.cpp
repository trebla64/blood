#include "stdafx.h"
#include "resource.h"


void sndPlaySong(char *, unsigned char)
{
}


void sndIsSongPlaying()
{
}


void sndFadeSong(int)
{
}


void sndSetMusicVolume(int)
{
}


void sndSetFXVolume(int)
{
}


void sndStopSong()
{
}


void sndStartSample(char *, int, int)
{
}


void sndStartSample(unsigned int, int, int)
{
}


void sndStartWavID(unsigned int, int, int)
{
}


void sndKillAllSounds()
{
}


void sndProcess()
{
}


void sndTerm()
{
}


void sndInit()
{
	gSoundRes.Init("SOUNDS.RFF", 0);	// TODO: 2nd arg should be "*.RAW"
}
