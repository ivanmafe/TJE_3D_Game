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
	
	HSAMPLE hSample;
	//El handler para un canal
	HCHANNEL hSampleChannel;
	MyAudioBass();
	~MyAudioBass();//{
		//iniciado = true;
	//};
	static void initbass();
	bool load(const char* filename);
	static MyAudioBass* Get(const char* filename);
	void setName(const char* filename) { sAudiosLoaded[filename] = this; }
	void PlaySoundAmbient(); 
};

#endif