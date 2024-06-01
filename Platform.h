#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// #include "Player.h"

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

    // void check_collision(Player &player)
    // {
    //     player.set_ground(false); // Assume the player is not on the ground to start with

    //     sf::FloatRect playerBounds = player.getGlobalBounds();
    //     sf::FloatRect platformBounds = getGlobalBounds();

    //     if (playerBounds.intersects(platformBounds))
    //     {
    //         sf::FloatRect intersection;
    //         playerBounds.intersects(platformBounds, intersection);

    //         sf::Vector2f player_velocity = player.get_velocity();

    //         if (player_velocity.y > 0 && intersection.height <= intersection.width)
    //         {
    //             player.setPosition(player.getPosition().x, platformBounds.top - playerBounds.height);
    //             player.set_ground(true); // Set the player to be on the ground if a collision is found
    //         }
    //     }
    // }
};
