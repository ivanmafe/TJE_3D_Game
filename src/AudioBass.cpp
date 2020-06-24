#include "AudioBass.h"


std::map<std::string, MyAudioBass*> MyAudioBass::sAudiosLoaded;

MyAudioBass::MyAudioBass() {
	
}

MyAudioBass::~MyAudioBass() {

}

bool MyAudioBass::load(const char* filename, bool loop)
{
	std::string str = filename;
	if (loop) hSample = BASS_SampleLoad(false, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
	else hSample = BASS_SampleLoad(false, filename, 0, 0, 3, 0);
	std::cout << " + Texture loading: " << filename << " ... ";

	this->filename = filename;

	//upload to VRAM
	return true;
}

void MyAudioBass::initbass() {
	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) {

	}
}


MyAudioBass* MyAudioBass::Get(const char* filename, bool loop)
{
	assert(filename);

	//check if loaded

	auto it = sAudiosLoaded.find(filename);
	//std::map<std::string, MyAudioBass*>::iterator it = sAudiosLoaded.find(filename);
	
	if (it != sAudiosLoaded.end())
		return it->second;

	//load it
	MyAudioBass* audioBass = new MyAudioBass();
	
	if (!audioBass->load(filename,loop))
	{
		delete audioBass;
		return NULL;
	}
	audioBass->setName(filename);	
	audioBass->hSampleChannel = BASS_SampleGetChannel(audioBass->hSample, false);
	return audioBass;
}


void MyAudioBass::PlaySoundOnce() {
	if (play) {
		if (hSample == 0) {
			std::cout << "file not found" << std::endl;
			//file not found
		}
		hSampleChannel = BASS_SampleGetChannel(hSample, false);
		//Lanzamos un sample
		BASS_ChannelPlay(hSampleChannel, true);
	}
	else std::cout << "1- OUT!!\N";
}


void MyAudioBass::PlaySoundAmbient(const char* filename) {

	//El handler para un sample

	//Cargamos un sample del disco duro (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	if(play){
		hSample = BASS_SampleLoad(false, filename, 0, 0, 3, BASS_SAMPLE_LOOP);
		if (hSample == 0) {
			std::cout << "file not found" << std::endl;
			//file not found
		}

		//Creamos un canal para el sample
		hSampleChannel = BASS_SampleGetChannel(hSample, false);


		//Lanzamos un sample
		BASS_ChannelPlay(hSampleChannel, true);
	}
	else std::cout << "2- OUT!!\N";
}

void MyAudioBass::StopSound() {
	BASS_Stop();
	//BASS_ChannelStop(hSampleChannel);
	BASS_Start();
}