#pragma once

#include <SFML/Graphics.hpp>
#include "Platform.h"
#include <iostream>
#include "Weapon.h"

class Game
{
private:
    bool game_over;
    float gravity;
    std::vector<Platform> platforms;
    float score;
    float jump_force;
    float velocity;

    int game_left_bound;
    int game_right_bound;

    float slow_down = 7.5;

    Weapon weapon;

    std::vector<sf::Texture> platform_textures;

public:
    Game(float grav, float jump, int left_bound, int right_bound) : weapon(sf::Vector2f(500, 500), WeaponType::SINGLE)
    {
        gravity = grav;
        jump_force = jump;
        velocity = grav;
        game_left_bound = left_bound;
        game_right_bound = right_bound;

        std::vector<std::string> texturePaths = {"assets/platform_grass.png", "assets/platform_stone.png"};

        for (const auto &path : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
            {
                std::cout << "Failed to load texture from " << path << std::endl;
                continue;
            }
            platform_textures.push_back(std::move(texture));
        }
    }

    void create_platforms(int offset, int platform_width, int platform_height, int window_height, int window_width)
    {
        for (int i = 0; i < window_height; i = i + offset)
        {
            int platform_y = window_height - i;
            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;

            Platform platform(sf::Vector2f(platform_x, platform_y), sf::Vector2f(platform_width, platform_height), platform_textures[rand() % platform_textures.size()]);

            platforms.emplace_back(platform);
        }
    }

    void update(sf::Time dt, sf::RenderWindow &window)
    {
        int platform_width = platforms[0].getLocalBounds().width;

        for (Platform &platform : platforms)
        {
            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;

            platform.move(sf::Vector2f(0, velocity * dt.asSeconds()));
            if (platform.getPosition().y > window.getSize().y)
            {
                platform.setPosition(sf::Vector2f(platform_x, 0));
                platform.randomizeTexture(platform_textures[rand() % platform_textures.size()]);
            }
        }

        update_score();

        if (velocity > 0)
        {
            if (gravity == 0)
            {
                velocity -= 50 * slow_down * dt.asSeconds();
                // std::cout << velocity << std::endl;
            }
            else
            {
                velocity -= gravity * slow_down * dt.asSeconds();
            }
        }
        else
        {
            velocity = gravity;
        }

        weapon.update(dt);
    }

    void draw(sf::RenderWindow &window)
    {
        for (Platform &platform : platforms)
        {
            window.draw(platform);
        }

        weapon.draw(window);
    }

    void jump()
    {
        velocity += jump_force;
    }

    void update_score()
    {
        score += 0.05 * velocity;
    }

    float get_score()
    {
        return score;
    }

    float end()
    {
        game_over = true;
        return score;
    }

    void shoot(WeaponType type)
    {
        weapon.shoot(type);
    }
};
