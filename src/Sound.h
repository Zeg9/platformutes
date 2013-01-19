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
#include <SDL/SDL_mixer.h>

class Sound
{
	friend class SoundManager;
	public:
		Sound(std::string filename);
		~Sound();
	private:
		Mix_Chunk*chunk;
};

class SoundManager
{
	friend SoundManager &getSoundManager();
	public:
		void playSound(Sound *s);
	private:
		SoundManager();
		~SoundManager();
};
SoundManager &getSoundManager();

#endif//__SOUND_H__
