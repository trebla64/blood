#ifndef __SOUND_H__
#define __SOUND_H__

void sndPlaySong(char *, unsigned char);
void sndIsSongPlaying();
void sndFadeSong(int);
void sndSetMusicVolume(int);
void sndSetFXVolume(int);
void sndStopSong();
void sndStartSample(char *, int, int);
void sndStartSample(unsigned int, int, int);
void sndStartWavID(unsigned int, int, int);
void sndKillAllSounds();
void sndProcess();
void sndTerm();
void sndInit();

#endif // __SOUND_H__
