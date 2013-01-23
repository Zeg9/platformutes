/*
 Platformutes
 Copyright (C) 2012-2013 Zeg9 <dazeg9@gmail.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdexcept>
#include <AL/al.h>
#include <AL/alut.h>
#include <SDL/SDL.h>
#include <vorbis/vorbisfile.h>
#include "Environment.h"
#include "Sound.h"

#include <iostream>

#define BUFFER_SIZE 32768 // 32 KB

Sound::Sound(std::string filename) : buffer()
{
	// Decode the ogg file
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	vorbis_info *pInfo;
	OggVorbis_File oggFile;
	if (ov_fopen(filename.c_str(), &oggFile) != 0)
		throw std::runtime_error("Error opening sound: "+filename);
	pInfo = ov_info(&oggFile, -1);
	if (pInfo->channels == 1)
		format = AL_FORMAT_MONO16;
	else
		format = AL_FORMAT_STEREO16;
	freq = pInfo->rate;
	do {
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, 0, 2, 1, &bitStream);
		if (bytes < 0)
		{
			ov_clear(&oggFile);
			throw std::runtime_error("Error decoding "+filename);
		}
		buffer.insert(buffer.end(), array, array + bytes);
	} while (bytes > 0);
	ov_clear(&oggFile);
	
	// Upload data to openal
	alGenBuffers(1,&bufferID);
	alBufferData(bufferID, format, &buffer[0], buffer.size(), freq);
	
	alGenSources(1,&sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
}
Sound::~Sound()
{
	alDeleteBuffers(1, &bufferID);
	alDeleteSources(1, &sourceID);
}

SoundManager::SoundManager()
{
	alutInit(0,0);
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	ALfloat orientation[] = {0.f, 0.f, -1.f, 0.f, 1.f, 0.f};
	alListenerfv(AL_ORIENTATION, orientation);
}
SoundManager::~SoundManager()
{
	alutExit();
}

void SoundManager::playSound(Sound *s, int _x, int _y)
{
	float x = _x/TILE_WIDTH/4,
	      y = _y/TILE_HEIGHT/4;
	alSource3f(s->sourceID, AL_POSITION, x, y, 0);
	alSourcePlay(s->sourceID);
}

SoundManager &getSoundManager()
{
	static SoundManager soundManager;
	return soundManager;
}
