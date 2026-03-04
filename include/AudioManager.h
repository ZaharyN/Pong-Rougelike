#pragma once
#include <unordered_map>
#include <SFML/Audio.hpp>
#include <iostream>

class AudioManager
{
	std::unordered_map<std::string, sf::SoundBuffer> buffers;
	std::optional<sf::Sound> sound;
	sf::Music backgroundMusic;

public:
	AudioManager();

	void PlaySound(const std::string_view& soundName);

	void PlayBackgroundMusic();

	void SetBackgroundMusicPitch(float pitch);
};