#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include "SFML\Audio.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <list>
#include <array>


enum class MusicName
{
	//THE_BEGINNING,

	NUM_OF_MUSIC_NAMES
};


enum class SoundName
{
	//HIT_BORDER,

	NUM_OF_SOUND_NAMES
};

typedef unsigned int RepeatedSoundHandle;

class SoundManager
{
private:
	static unsigned int mMaximalNumberOfRepeatedSounds;
	static unsigned int mMaximalNumberOfUnrepeatedSounds;

	static std::array<sf::SoundBuffer*, static_cast<unsigned int>(SoundName::NUM_OF_SOUND_NAMES)> mArrayOfSoundBuffers;

	static sf::Music* pMusic;

	static std::list<sf::Sound*> mListOfUnrepreatedSounds;
	static std::list<std::pair<sf::Sound*, RepeatedSoundHandle>> mListOfRepeatedSounds;



public:
	SoundManager() = delete;
	~SoundManager() = delete;
	SoundManager(SoundManager const &) = delete;
	SoundManager& operator=(SoundManager const &) = delete;

	static void Init();
	static void DeInit();

	static bool playOnce(SoundName soundName, float volume, sf::Vector3f const & soundPosition, bool relativeToListener = false);
	static bool addRepeatedSound(SoundName soundName, float volume, RepeatedSoundHandle& rsh, sf::Vector3f const & soundPosition, bool relativeToListener = false);
	static void removeRepeatedSound(RepeatedSoundHandle rsh);
	static void setListenerPosition(sf::Vector3f const & position);
	static void setListenerDirection(sf::Vector3f const & direction);

	static void playMusic(MusicName musicName, float volume);
	static void resumeMusic();
	static void stopMusic();
	static void pauseMusic();

private:
	static std::string getMusicPath(MusicName musicName);
	static sf::SoundBuffer* getSoundBuffer(SoundName soundName);
	static RepeatedSoundHandle generateValidRepeatedSoundHandle();
	static void cleanUpUnrepeatedSounds();

};


#endif //SOUNDMANAGER_HPP

