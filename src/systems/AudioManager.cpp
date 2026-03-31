#include "AudioManager.h"

AudioManager::AudioManager()
{
	sf::SoundBuffer selectBuffer;
	if (selectBuffer.loadFromFile("Assets/Audio/UI_select.ogg"))
		buffers["select"] = std::move(selectBuffer);
	else
		std::cout << "Could not load select sound\n";

	sf::SoundBuffer hitBuffer;
	if (hitBuffer.loadFromFile("Assets/Audio/paddle_hit.wav"))
		buffers["hit"] = std::move(hitBuffer);
	else
		std::cout << "Could not load hit sound\n";

	sf::SoundBuffer energyPickUpBuffer;
	if (energyPickUpBuffer.loadFromFile("Assets/Audio/energy_picked.ogg"))
		buffers["energy_picked"] = std::move(energyPickUpBuffer);
	else
		std::cout << "Could not load energy_picked sound\n";

	sf::SoundBuffer deadZoneHitBuffer;
	if (deadZoneHitBuffer.loadFromFile("Assets/Audio/dead_zone_hit.ogg"))
		buffers["dead_zone_hit"] = std::move(deadZoneHitBuffer);
	else
		std::cout << "Could not load dead_zone_hit sound\n";

	if (!backgroundMusic.openFromFile("Assets/Audio/background.mp3"))
		std::cout << "Error: Could not find background music in Assets/Audio folder!\n";
}

void AudioManager::PlaySound(const std::string_view soundName)
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

void AudioManager::SetPitch(float speedRatio)
{
	float newPitch = 1.f + (speedRatio - 1.0) * DAMPING_FACTOR;
	backgroundMusic.setPitch(newPitch);
}

void AudioManager::StopBackgroundMusic()
{
	backgroundMusic.stop();
}