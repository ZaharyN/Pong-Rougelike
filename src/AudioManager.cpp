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

	sf::SoundBuffer energyPickUpBuffer;
	if (energyPickUpBuffer.loadFromFile("Assets/Audio/energy_picked.ogg"))
	{
		buffers["energy_picked"] = std::move(energyPickUpBuffer);
	}

	if (!backgroundMusic.openFromFile("Assets/Audio/background.mp3"))
	{
		std::cout << "Error: Could not find background music in Assets/Audio folder!" << std::endl;
	}
}

void AudioManager::PlaySound(const std::string_view& soundName)
{
	soundPool.remove_if([](sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Status::Stopped;
		});
	
	auto it = buffers.find(std::string(soundName));

	if (it != buffers.end())
	{
		soundPool.emplace_back(it->second);
		soundPool.back().play();
	}
}

void AudioManager::PlayBackgroundMusic()
{
	backgroundMusic.setLooping(true);
	backgroundMusic.setPitch(1.f);
	backgroundMusic.play();
}

void AudioManager::SetPitch(float speedRation)
{
	float newPitch = 1.f + (speedRation - 1.0) * DAMPING_FACTOR;
	backgroundMusic.setPitch(newPitch);
}

void AudioManager::StopBackgroundMusic()
{
	backgroundMusic.stop();
}