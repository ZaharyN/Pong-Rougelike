#pragma once
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>

class AudioManager
{
	static constexpr float DAMPING_FACTOR = 0.3f;

	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::list<sf::Sound> soundPool;
	sf::Music backgroundMusic;

public:
	AudioManager();

	void PlaySound(const std::string_view soundName);

	void PlayBackgroundMusic();

	void SetPitch(float speedRatio);

	void StopBackgroundMusic();
};