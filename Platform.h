#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Platform : public sf::Sprite
{
public:
    Platform(sf::Vector2f pos, sf::Vector2f s, sf::Texture &platform_texture) : sf::Sprite()
    {
        sf::Vector2u textureSize = platform_texture.getSize();

        if (textureSize.x == 0 && textureSize.y == 0)
        {
            std::cout << "Failed to load platform texture" << std::endl;
            return;
        }
        setPosition(pos);

        setTexture(platform_texture);

        setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
    }

    void randomizeTexture(sf::Texture &platform_texture)
    {
        sf::Vector2u textureSize = platform_texture.getSize();
        setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
        setTexture(platform_texture);
    }
};
