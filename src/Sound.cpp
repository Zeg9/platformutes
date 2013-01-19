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

#include <SDL/SDL_mixer.h>
#include "Sound.h"

#include <iostream>

Sound::Sound(std::string filename)
{
	chunk = Mix_LoadWAV(filename.c_str());
	if (!chunk)
		std::cerr << "[!] Error opening sound: " << Mix_GetError() << std::endl;
}
Sound::~Sound()
{
	Mix_FreeChunk(chunk);
}

SoundManager::SoundManager()
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		std::cerr << "[!] Error opening audio: " << Mix_GetError() << std::endl;
	Mix_AllocateChannels(10);
}
SoundManager::~SoundManager()
{
	Mix_CloseAudio();
}

void SoundManager::playSound(Sound *s)
{
	if (Mix_PlayChannel(-1, s->chunk, 0) == -1)
		std::cerr << "[!] Error playing sound: " << Mix_GetError() << std::endl;
}

SoundManager &getSoundManager()
{
	static SoundManager soundManager;
	return soundManager;
}
