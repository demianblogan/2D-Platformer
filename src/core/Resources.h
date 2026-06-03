#pragma once

#include "core/ResourceManager.h"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <string>

struct Resources
{
	void LoadTexturesFromFile(const std::string& path);

	ResourceManager<sf::Texture> textures;
	ResourceManager<sf::Font> fonts;
	ResourceManager<sf::SoundBuffer> sounds;
	ResourceManager<sf::Music> music;
	ResourceManager<sf::Shader> shaders;
};