#ifndef AUDIO_BASS_H
#define AUDIO_BASS_H

#include <string>
#include <iostream>
#include <bass.h>
#include <map>
#include <cassert>

class MyAudioBass
{
public:
	static std::map<std::string, MyAudioBass*> sAudiosLoaded;
	std::string filename;
	bool play = true;
	HSAMPLE hSample;
	HCHANNEL hSampleChannel;

	MyAudioBass();
	~MyAudioBass();//{
		//iniciado = true;
	//};
	static void initbass();
	bool load(const char* filename, bool loop);
	static MyAudioBass* Get(const char* filename, bool loop);
	void setName(const char* filename) { sAudiosLoaded[filename] = this; }
	void PlaySoundOnce();
	void PlaySoundAmbient(const char* filename);
	void StopSound();
};

#endif