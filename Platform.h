#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "PowerUp.h"

class Platform : public sf::Sprite
{
private:
    bool powerup_spawned = true;
    PowerUp* powerup = nullptr;

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

        
        if(textureSize.x != s.x && textureSize.y != s.y){
            platform_texture.setRepeated(true);
        }
        setTexture(platform_texture);
        setTextureRect(sf::IntRect(0, 0, s.x, s.y));
    }

    void randomize_texture(sf::Texture &platform_texture)
    {
        sf::Vector2u textureSize = platform_texture.getSize();
        setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
        setTexture(platform_texture);
    }

    void setPowerUpSpawned(bool value)
    {
        powerup_spawned = value;
    }
    bool getPowerUpSpawned()
    {
        return powerup_spawned;
    }
    PowerUp* getPowerUp()
    {
        return powerup;
    }
    void setPowerUp(PowerUp* p)
    {
        powerup = p;
    }
};
