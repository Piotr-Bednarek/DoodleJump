#pragma once

#include <SFML/Graphics.hpp>
#include "Platform.h"

class Game
{
private:
    float gravity;
    std::vector<Platform> platforms;
    float score;

public:
    Game(float g)
    {
        gravity = g;
    }

    void create_platforms(int offset, int platform_width, int platform_height, int window_height, int window_width)
    {
        for (int i = 0; i < window_height; i = i + offset)
        {
            int platform_y = window_height - i;
            int platform_x = rand() % (window_width - platform_width);

            Platform platform(sf::Vector2f(platform_x, platform_y), sf::Vector2f(platform_width, platform_height));

            platforms.emplace_back(platform);
        }
    }

    void update(sf::Time dt, sf::RenderWindow &window)
    {

        for (Platform &platform : platforms)
        {
            platform.move(sf::Vector2f(0, gravity * dt.asSeconds()));
            if (platform.getPosition().y > window.getSize().y)
            {
                platform.setPosition(sf::Vector2f(rand() % window.getSize().x, 0));
            }
        }

        update_score();
    }

    void draw(sf::RenderWindow &window)
    {
        for (Platform &platform : platforms)
        {
            window.draw(platform);
        }
    }

    void update_score()
    {
        score += 0.1;
    }

    float get_score()
    {
        return score;
    }
};
