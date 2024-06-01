#pragma once

#include <SFML/Graphics.hpp>
#include "Platform.h"
#include <iostream>
#include "Weapon.h"
#include "Player.h"

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

    std::vector<sf::Texture> platform_textures;

public:
    Game(float grav, float jump, int left_bound, int right_bound)
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

        Platform platform(sf::Vector2f(400, 700), sf::Vector2f(800, 50), platform_textures[rand() % platform_textures.size()]);
        platforms.emplace_back(platform);

        // Platform platform1(sf::Vector2f(400, 500), sf::Vector2f(800, 50), platform_textures[rand() % platform_textures.size()]);
        // platforms.emplace_back(platform1);
    }

    void update(float dt, sf::RenderWindow &window, Player &player)
    {
        int platform_width = platforms[0].getLocalBounds().width;

        for (Platform &platform : platforms)
        {
            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;

            platform.move(sf::Vector2f(0, velocity * dt));
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
                velocity -= 50 * slow_down * dt;
            }
            else
            {
                velocity -= gravity * slow_down * dt;
            }
        }
        else
        {
            velocity = gravity;
        }

        float threshold = window.getSize().y * 0.5f;
        if (player.getPosition().y < threshold)
        {
            float diff = threshold - player.getPosition().y;
            player.move(0, diff);

            for (Platform &platform : platforms)
            {
                platform.move(0, diff);

                if (platform.getPosition().y > window.getSize().y)
                {
                    platform.setPosition(sf::Vector2f(platform.getPosition().x, 0));
                    platform.randomizeTexture(platform_textures[rand() % platform_textures.size()]);
                }
            }
        }
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

    void check_collision(Player &player)
    {
        int platformIndex = find_platform_index(player);

        // std::cout << "Platform index: " << platformIndex << std::endl;

        if (platformIndex != -1)
        {
            Platform &platform = platforms[platformIndex];
            sf::FloatRect playerBounds = player.getGlobalBounds();
            sf::FloatRect platformBounds = platform.getGlobalBounds();

            bool isPlayerOutsidePlatform = playerBounds.left + playerBounds.width < platformBounds.left || playerBounds.left > platformBounds.left + platformBounds.width;

            if (isPlayerOutsidePlatform)
            {
                player.set_ground(false);
            }
            else if (playerBounds.intersects(platformBounds))
            {
                sf::FloatRect intersection;
                playerBounds.intersects(platformBounds, intersection);

                sf::Vector2f player_velocity = player.get_velocity();

                if (player_velocity.y > 0 && intersection.height <= intersection.width)
                {
                    player.setPosition(player.getPosition().x, platformBounds.top - playerBounds.height);
                    player.set_ground(true);
                }
            }
        }
        else
        {
            player.set_ground(false);
        }
    }

    int find_platform_index(Player &player)
    {
        for (int i = 0; i < platforms.size(); i++)
        {
            sf::FloatRect playerBounds = player.getGlobalBounds();
            sf::FloatRect platformBounds = platforms[i].getGlobalBounds();

            float offset = 5.0f;
            playerBounds.height += offset;

            if (playerBounds.intersects(platformBounds))
            {
                return i;
            }
        }

        return -1;
    }
};
