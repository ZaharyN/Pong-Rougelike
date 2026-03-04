#include "AudioManager.h"

AudioManager::AudioManager()
{
	sf::SoundBuffer selectBuffer;
	if (selectBuffer.loadFromFile("Assets/Audio/UI_select.ogg"))
	{
		buffers["select"] = std::move(selectBuffer);
	}

	sf::SoundBuffer hitBuffer;
	if (hitBuffer.loadFromFile("Assets/Audio/paddle_hit.wav"))
	{
		buffers["hit"] = std::move(hitBuffer);
	}

	if (!backgroundMusic.openFromFile("Assets/Audio/background.mp3"))
	{
		std::cout << "Error: Could not find background music in Assets/Audio folder!" << std::endl;
	}
}

void AudioManager::PlaySound(const std::string_view& soundName)
{
	auto it = buffers.find(std::string(soundName));
	
	if (it != buffers.end())
	{
		sound.emplace(it->second);
		sound->play();
	}
}

void AudioManager::PlayBackgroundMusic()
{
	backgroundMusic.setLooping(true);
	backgroundMusic.play();
}

void AudioManager::SetBackgroundMusicPitch(float pitch)
{
	backgroundMusic.setPitch(pitch);
}