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

#ifndef __SOUND_H__
#define __SOUND_H__

#include <string>
#include <vector>
#include <AL/al.h>

class Sound
{
	friend class SoundManager;
	public:
		Sound(std::string filename);
		~Sound();
	private:
		ALenum format;
		ALsizei freq;
		ALuint bufferID;
		ALuint sourceID;
		std::vector<char> buffer;
};

class SoundManager
{
	friend SoundManager &getSoundManager();
	public:
		void playSound(Sound *s, int x=0, int y=0);
	private:
		SoundManager();
		~SoundManager();
};
SoundManager &getSoundManager();

#endif//__SOUND_H__
