#include "stdafx.h"
#include "Source\Framework\SoundManager.hpp"


//Init
void SoundManager::Init()
{
	//sf::SoundBuffer* hitBorderSoundBuffer = new sf::SoundBuffer;
	//hitBorderSoundBuffer->loadFromFile("Data/Sounds/SoundEffects/HitBorder.wav");
	//mArrayOfSoundBuffers.at(static_cast<unsigned int>(SoundName::HIT_BORDER)) = hitBorderSoundBuffer;

	pMusic = new sf::Music;
}

//DeInit
void SoundManager::DeInit()
{
	for (std::list<sf::Sound*>::iterator soundIt = mListOfUnrepreatedSounds.begin(); soundIt != mListOfUnrepreatedSounds.end(); )
	{
		delete (*soundIt);
		(*soundIt) = nullptr;
		soundIt = mListOfUnrepreatedSounds.erase(soundIt);
	}

	for (std::list<std::pair<sf::Sound*, RepeatedSoundHandle>>::iterator pairIt = mListOfRepeatedSounds.begin(); pairIt != mListOfRepeatedSounds.end(); )
	{
		delete pairIt->first;
		pairIt->first = nullptr;
		pairIt = mListOfRepeatedSounds.erase(pairIt);
	}

	for (std::array<sf::SoundBuffer*, static_cast<unsigned int>(SoundName::NUM_OF_SOUND_NAMES)>::iterator soundBufferIt = mArrayOfSoundBuffers.begin(); soundBufferIt != mArrayOfSoundBuffers.end(); )
	{
		delete (*soundBufferIt);
		*soundBufferIt = nullptr;
	}

	if (pMusic != nullptr)
	{
		delete pMusic;
		pMusic = nullptr;
	}
}

//Sound Controllers
bool SoundManager::playOnce(SoundName soundName, float volume, sf::Vector3f const & soundPosition, bool relativeToListener)
{
	cleanUpUnrepeatedSounds();
	bool possible = (mListOfUnrepreatedSounds.size() < mMaximalNumberOfUnrepeatedSounds);
	if (possible)
	{
		sf::Sound* sound = new sf::Sound(*getSoundBuffer(soundName));
		sound->setLoop(false);
		sound->setVolume(volume);
		sound->setRelativeToListener(relativeToListener);
		sound->setPosition(soundPosition);
		sound->play();
		mListOfUnrepreatedSounds.push_back(sound);
	}
	//std::cout << "Number of Unrepeated Sounds: " << mListOfUnrepreatedSounds.size() << std::endl;
	return possible;
}
bool SoundManager::addRepeatedSound(SoundName soundName, float volume, RepeatedSoundHandle& rsh, sf::Vector3f const & soundPosition, bool relativeToListener)
{
	bool possible = (mListOfRepeatedSounds.size() < mMaximalNumberOfRepeatedSounds);
	if (possible)
	{
		sf::Sound* sound = new sf::Sound(*getSoundBuffer(soundName));
		sound->setLoop(true);
		sound->setVolume(volume);
		sound->setRelativeToListener(relativeToListener);
		sound->setPosition(soundPosition);
		sound->play();
		rsh = generateValidRepeatedSoundHandle();
		mListOfRepeatedSounds.push_back(std::pair<sf::Sound*, RepeatedSoundHandle>(sound, rsh));
	}
	return possible;
}
void SoundManager::removeRepeatedSound(RepeatedSoundHandle rsh)
{
	for (std::list<std::pair<sf::Sound*, RepeatedSoundHandle>>::iterator pairIt = mListOfRepeatedSounds.begin(); pairIt != mListOfRepeatedSounds.end(); )
	{
		if (pairIt->second == rsh)
		{
			delete pairIt->first;
			pairIt->first = nullptr;
			mListOfRepeatedSounds.erase(pairIt);
			return;
		}
		else
		{
			++pairIt;
		}
	}
	throw "MusicManager::removeRepeatedSound(RepeatedSoundHandle rsh) : Handle rsh does not exist!";
}
void SoundManager::setListenerPosition(sf::Vector3f const & position)
{
	sf::Listener::setPosition(position);
}
void SoundManager::setListenerDirection(sf::Vector3f const & direction)
{
	sf::Listener::setDirection(direction);
}

//Music Controllers
void SoundManager::playMusic(MusicName musicName, float volume)
{
	pMusic->openFromFile(getMusicPath(musicName));
	pMusic->setLoop(true);
	pMusic->setVolume(volume);
	pMusic->play();
}
void SoundManager::resumeMusic()
{
	pMusic->play();
}
void SoundManager::stopMusic()
{
	pMusic->stop();
}
void SoundManager::pauseMusic()
{
	pMusic->pause();
}


//Internal Functions
std::string SoundManager::getMusicPath(MusicName musicName)
{
	switch (musicName)
	{
	//case MusicName::THE_BEGINNING:
	//	return "Data/Sounds/Music/The Beginning.wav";
	//	break;
	}
	return "";
}
sf::SoundBuffer* SoundManager::getSoundBuffer(SoundName soundName)
{
	return mArrayOfSoundBuffers.at(static_cast<unsigned int>(soundName));
}
RepeatedSoundHandle SoundManager::generateValidRepeatedSoundHandle()
{
	std::list<RepeatedSoundHandle> existingHandles;
	for (auto sound : mListOfRepeatedSounds)
	{
		existingHandles.push_back(sound.second);
	}
	RepeatedSoundHandle newHandle = 0;
	while (true)
	{
		newHandle++;
		bool handleValid = true;
		for (auto handle : existingHandles)
		{
			bool sameHandle = (handle == newHandle);
			handleValid = handleValid && !sameHandle;
			if (!handleValid)
			{
				continue;
			}
		}
		if (handleValid)
		{
			return newHandle;
		}
	}
}
void SoundManager::cleanUpUnrepeatedSounds()
{
	for (std::list<sf::Sound*>::iterator soundIt = mListOfUnrepreatedSounds.begin(); soundIt != mListOfUnrepreatedSounds.end(); )
	{
		if ((*soundIt)->getStatus() == sf::Sound::Status::Stopped)
		{
			delete (*soundIt);
			(*soundIt) = nullptr;
			soundIt = mListOfUnrepreatedSounds.erase(soundIt);
		}
		else
		{
			++soundIt;
		}
	}
}


//Initialize static variables
unsigned int SoundManager::mMaximalNumberOfRepeatedSounds = 20;
unsigned int SoundManager::mMaximalNumberOfUnrepeatedSounds = 40;
sf::Music* SoundManager::pMusic = nullptr;

std::array<sf::SoundBuffer*, static_cast<unsigned int>(SoundName::NUM_OF_SOUND_NAMES)> SoundManager::mArrayOfSoundBuffers = std::array<sf::SoundBuffer*, static_cast<unsigned int>(SoundName::NUM_OF_SOUND_NAMES)>();
std::list<sf::Sound*> SoundManager::mListOfUnrepreatedSounds = std::list<sf::Sound*>();
std::list<std::pair<sf::Sound*, RepeatedSoundHandle>> SoundManager::mListOfRepeatedSounds = std::list<std::pair<sf::Sound*, RepeatedSoundHandle>>();


