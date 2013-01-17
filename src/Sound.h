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
		Mix_Chunk *chunk;
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
