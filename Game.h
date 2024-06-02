#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "Platform.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"

class Game
{
private:
    bool game_over;

    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;

    float score;

    float elevation;

    float jump_force;
    float velocity;

    int game_left_bound;
    int game_right_bound;

    float slow_down = 7.5;

    int last_enemy_spawn;

    std::vector<sf::Texture> platform_textures;
    std::vector<sf::Texture> enemy_textures;

public:
    Game(float grav, float jump, int left_bound, int right_bound)
    {
        jump_force = jump;
        velocity = grav;
        game_left_bound = left_bound;
        game_right_bound = right_bound;

        std::vector<std::string> platform_texture_paths = {"assets/enviroment/platform_grass.png", "assets/enviroment/platform_stone.png"};

        for (const auto &path : platform_texture_paths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
            {
                std::cout << "Failed to load texture from " << path << std::endl;
                continue;
            }
            platform_textures.push_back(std::move(texture));
        }

        std::vector<std::string> enemy_texture_paths = {"assets/enemy/dragon_flying.png", "assets/enemy/bee_flying.png"};

        for (const auto &path : enemy_texture_paths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
            {
                std::cout << "Failed to load texture from " << path << std::endl;
                continue;
            }
            enemy_textures.push_back(std::move(texture));
        }
    }

    void create_platforms(int offset, int platform_width, int platform_height, int window_height, int window_width)
    {
        for (int i = -platform_height * 2; i < window_height; i = i + offset)
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

        check_if_player_is_dead(player);

        int platform_width = platforms[0].getLocalBounds().width;
        int platform_height = platforms[0].getLocalBounds().height;

        for (Platform &platform : platforms)
        {
            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;

            platform.move(sf::Vector2f(0, velocity * dt));
            if (platform.getPosition().y > window.getSize().y)
            {
                platform.setPosition(sf::Vector2f(platform_x, -platform_height));
                platform.randomize_texture(platform_textures[rand() % platform_textures.size()]);
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy &enemy)
                                     { return enemy.get_health() <= 0; }),
                      enemies.end());

        for (Enemy &enemy : enemies)
        {
            enemy.move(dt, velocity * dt);
            enemy.update(dt, window, player.getPosition());

            // if (enemy.get_health() <= 0)
            // {
            //     enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
            //     continue;
            // }
        }

        check_if_spawn_enemy();

        float threshold = window.getSize().y * 0.5f;
        if (player.getPosition().y < threshold)
        {
            float diff = threshold - player.getPosition().y;
            player.move(0, diff);

            score += diff;

            elevation += diff;

            for (Platform &platform : platforms)
            {
                platform.move(0, diff);

                if (platform.getPosition().y > window.getSize().y)
                {
                    platform.setPosition(sf::Vector2f(platform.getPosition().x, -platform_height));
                    platform.randomize_texture(platform_textures[rand() % platform_textures.size()]);
                }
            }

            for (Enemy &enemy : enemies)
            {
                enemy.move(0, diff);
            }
        }

        player.check_projeciltile_collision(enemies);

        for (Enemy &enemy : enemies)
        {
            int result = enemy.check_projeciltile_collision(player.getGlobalBounds());

            if (result != -1)
            {
                player.update_health(result);
            }
        }
    }

    void check_if_spawn_enemy()
    {

        int enemy_spawn = static_cast<int>(round(elevation / 100) * 100);

        int threshold = 700;

        if (enemy_spawn % 500 == 0 && enemy_spawn != last_enemy_spawn)
        {
            create_enemy();
            last_enemy_spawn = enemy_spawn;
        }
    }

    void draw(sf::RenderWindow &window)
    {
        for (Platform &platform : platforms)
        {
            window.draw(platform);
        }

        for (Enemy &enemy : enemies)
        {
            window.draw(enemy);
        }
    }

    float get_score()
    {
        return score;
    }

    float end()
    {

        return score;
    }

    void check_collision(Player &player)
    {
        int platformIndex = find_platform_index(player);

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

    void create_enemy()
    {
        sf::Vector2f position;
        float speed;
        int direction;

        if (rand() % 2 == 0)
        {
            position = sf::Vector2f(game_left_bound, 0);
            direction = 1;
        }
        else
        {
            position = sf::Vector2f(game_right_bound, 0);
            direction = -1;
        }

        speed = rand() % 101 + 100;

        if (rand() % 2 == 0)
        {
            Enemy enemy(position, speed, direction, enemy_textures[0], game_left_bound, game_right_bound, 1, 1);

            for (int i = 0; i < 4; i++)
            {
                enemy.add_animation_frame(sf::IntRect(81 * i, 0, 71, 81));
            }

            enemies.emplace_back(enemy);
        }
        else
        {
            Enemy enemy(position, speed, direction, enemy_textures[1], game_left_bound, game_right_bound, 2, 2);

            for (int i = 0; i < 4; i++)
            {
                enemy.add_animation_frame(sf::IntRect(64 * i, 0, 64, 64));
            }

            enemies.emplace_back(enemy);
        }
    }

    void check_if_player_is_dead(Player &player)
    {
        if (player.get_health() <= 0 || player.getPosition().y > 800)
        {
            game_over = true;
        }
    }

    bool get_game_state()
    {
        return game_over;
    }
};
