#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "Platform.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "HighScoreManager.h"

class Game
{
private:
    bool game_over = false;

    float time = 0;

    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;

    float score = 0;

    float elevation;

    float jump_force;
    float velocity;

    int game_left_bound;
    int game_right_bound;

    float slow_down = 7.5;

    int last_enemy_spawn;

    std::vector<sf::Texture> platform_textures;
    std::vector<sf::Texture> enemy_textures;
    std::vector<sf::Texture> powerUp_textures;

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

        std::vector<std::string> enemy_texture_paths = {"assets/enemy/dragon_flying.png", "assets/enemy/bee_flying.png", "assets/enemy/kamikaze_flying.png"};

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

        std::vector<std::string> powerUp_texture_paths = {"assets/powerup/heart.png", "assets/powerup/shield.png", "assets/powerup/jump.png", "assets/powerup/massacre.png"};
        for (const auto &path : powerUp_texture_paths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
            {
                std::cout << "Failed to load texture from " << path << std::endl;
                continue;
            }
            powerUp_textures.push_back(std::move(texture));
        }
    }

    void create_platforms(int offset, int platform_width, int platform_height, int window_height, int window_width)
    {
        Platform platform1(sf::Vector2f(game_left_bound, window_height - 35), sf::Vector2f(window_width, 34), platform_textures[0]);
        platforms.emplace_back(platform1);
        for (int i = platform_height * 2 + offset; i < window_height; i = i + offset)
        {
            int platform_y = window_height - i;
            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;

            Platform platform(sf::Vector2f(platform_x, platform_y), sf::Vector2f(platform_width, platform_height), platform_textures[rand() % platform_textures.size()]);

            platforms.emplace_back(platform);
        }
    }

    void update(float dt, sf::RenderWindow &window, Player &player)
    {
        time = dt;

        check_if_player_is_dead(player);

        int platform_width = platforms[1].getLocalBounds().width;
        int platform_height = platforms[1].getLocalBounds().height;

        for (Platform &platform : platforms)
        {

            platform.move(sf::Vector2f(0, velocity * dt));
            if (platform.getPowerUp() != nullptr)
            {
                platform.getPowerUp()->move(0, velocity * dt);
            }

            if (!platform.getPowerUpSpawned())
            {
                platform.setPowerUp(create_powerUps(platform));
                platform.setPowerUpSpawned(true);
            }
            if (platform.getPowerUp() != nullptr)
            {
                if (platform.getPowerUp()->getGlobalBounds().intersects(player.getGlobalBounds()))
                {
                    applyPowerUpEffect(*platform.getPowerUp(), player);
                    platform.setPowerUp(nullptr);
                }
            }
        }
        for (Enemy &enemy : enemies)
        {
            if (enemy.getGlobalBounds().intersects(player.getGlobalBounds()) && player.get_massacre())
            {
                enemy.update_health(100000);
            }
        }
        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy &enemy){ return enemy.get_health() <= 0; }),enemies.end());

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

            for (int i = 0; i < platforms.size(); i++)
            {
                platforms[i].move(0, diff);

                if (platforms[i].getPowerUp() != nullptr)
                {
                    platforms[i].getPowerUp()->move(0, diff);
                }

                if (!platforms[i].getPowerUpSpawned())
                {
                    platforms[i].setPowerUp(create_powerUps(platforms[i]));
                    platforms[i].setPowerUpSpawned(true);
                }

                if (platforms[i].getGlobalBounds().getPosition().y > window.getSize().y)
                {
                    int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;
                    platforms[i].setPosition(sf::Vector2f(platform_x, -platform_height - rand() % (platform_height) + player.get_velocity().y * dt));
                    for (int j = 0; j < platforms.size(); j++)
                    {
                        if (platforms[i].getGlobalBounds().intersects(platforms[j].getGlobalBounds()) && i != j)
                        {
                            int platform_x = rand() % (game_right_bound - platform_width - game_left_bound) + game_left_bound;
                            platforms[i].setPosition(sf::Vector2f(platform_x, -platform_height - rand() % (platform_height) + player.get_velocity().y * dt));
                            j = 0;

                        }
                    }
                    platforms[i].randomize_texture(platform_textures[rand() % platform_textures.size()]);
                    platforms[i].setPowerUpSpawned(false);
                    delete platforms[i].getPowerUp();
                    platforms[i].setPowerUp(nullptr);
                }
            }

            for (Enemy &enemy : enemies)
            {
                enemy.move(0, diff);
            }
        }

        player.check_projeciltile_collision(enemies);

        for (int i = 0; i < enemies.size(); i++)
        {
            Enemy &enemy = enemies[i];
            if (enemy.get_enemy_type() == EnemyType::KAMIKAZE)
            {
                int result = enemy.check_kamikaze_collision(player.getGlobalBounds(), player.get_massacre());

                if (result != -1)
                {
                    player.update_health(result);
                    enemies.erase(enemies.begin() + i);
                    i--;
                }
                continue;
            }

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
            if (platform.getPowerUp() != nullptr)
            {
                window.draw(static_cast<PowerUp>(*platform.getPowerUp()));
            }
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

    void check_collision(Player &player)
    {
        int platformIndex = find_platform_index(player);

        if (platformIndex != -1)
        {
            Platform &platform = platforms[platformIndex];
            sf::FloatRect playerBounds = player.getGlobalBounds();
            sf::FloatRect platformBounds = platform.getGlobalBounds();

            bool isPlayerOutsidePlatform = true;

            if (playerBounds.left + playerBounds.width > platformBounds.left && playerBounds.left < platformBounds.left + platformBounds.width && playerBounds.top + playerBounds.height >= platformBounds.top && playerBounds.top + playerBounds.height <= platformBounds.top + 5 + player.get_velocity().y * time)
            {
                isPlayerOutsidePlatform = false;
            }

            if (isPlayerOutsidePlatform)
            {
                player.set_ground(false);
            }
            else if (playerBounds.intersects(platformBounds))
            {
                sf::FloatRect intersection;
                playerBounds.intersects(platformBounds, intersection);

                sf::Vector2f player_velocity = player.get_velocity();

                if (player_velocity.y > 0) //&& intersection.height <= intersection.width)
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
        sort(platforms.begin(), platforms.end(), [](Platform &a, Platform &b)
             { return a.getPosition().y > b.getPosition().y; });
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

        int enemy_type = rand() % 3;

        // Enemy enemy(position, speed, direction, enemy_textures[2], game_left_bound, game_right_bound, 1.5, 1.5, ProjectileType::SHURIKEN, EnemyType::KAMIKAZE);
        // for (int i = 0; i < 15; i++)
        // {
        //     enemy.add_animation_frame(sf::IntRect(62 * i, 0, 62, 72));
        // }
        // enemies.emplace_back(enemy);

        if (enemy_type == 0)
        {
            Enemy enemy(position, speed, direction, enemy_textures[0], game_left_bound, game_right_bound, 1, 1, ProjectileType::FIREBALL, EnemyType::DRAGON);

            for (int i = 0; i < 4; i++)
            {
                enemy.add_animation_frame(sf::IntRect(81 * i, 0, 71, 81));
            }
            enemies.emplace_back(enemy);
        }
        else if (enemy_type == 1)
        {
            Enemy enemy(position, speed, direction, enemy_textures[1], game_left_bound, game_right_bound, 2, 2, ProjectileType::SHURIKEN, EnemyType::BEE);

            for (int i = 0; i < 4; i++)
            {
                enemy.add_animation_frame(sf::IntRect(64 * i, 0, 64, 64));
            }

            enemies.emplace_back(enemy);
        }
        else if (enemy_type == 2)
        {
            Enemy enemy(position, speed, direction, enemy_textures[2], game_left_bound, game_right_bound, 1.5, 1.5, ProjectileType::SHURIKEN, EnemyType::KAMIKAZE);

            for (int i = 0; i < 15; i++)
            {
                enemy.add_animation_frame(sf::IntRect(62 * i, 0, 62, 72));
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

    PowerUp *create_powerUps(Platform &platform)
    {
        PowerUp *powerUp = nullptr;
        if (rand() % 100 < 25)
        {
            PowerUpType type = static_cast<PowerUpType>(rand() % (static_cast<int>(PowerUpType::MASSACRE) + 1));
            switch (type)
            {
            case PowerUpType::HEAL:
                powerUp = new PowerUp(powerUp_textures[(int)type], type, 50);
                break;
            case PowerUpType::INVINCIBILITY:
                powerUp = new PowerUp(powerUp_textures[(int)type], type, 10);
                break;
            case PowerUpType::JUMPBOOST:
                powerUp = new PowerUp(powerUp_textures[(int)type], type, -1500);
                powerUp->setScale(0.3, 0.3);
                break;
            case PowerUpType::MASSACRE:
                powerUp = new PowerUp(powerUp_textures[(int)type], type, 5);
                powerUp->setScale(0.1, 0.1);
                break;
            default:
                std::cout << "Unknown power-up type!" << std::endl;
                break;
            }
            float powerUpX = platform.getPosition().x + rand() % (static_cast<int>(platform.getGlobalBounds().width - powerUp->getGlobalBounds().width));
            float powerUpY = platform.getPosition().y - powerUp->getGlobalBounds().height;
            powerUp->setPosition(powerUpX, powerUpY);
        }
        return powerUp;
    }
    void applyPowerUpEffect(PowerUp &powerUp, Player &player)
    {
        PowerUpType type = powerUp.getType();

        switch (type)
        {
        case PowerUpType::HEAL:
            player.restoreHealth(powerUp.getEffectAmount());
            break;
        case PowerUpType::INVINCIBILITY:
            player.set_invincible(powerUp.getEffectAmount());
            break;
        case PowerUpType::JUMPBOOST:
            player.boostJump(powerUp.getEffectAmount());
            break;
        case PowerUpType::MASSACRE:
            player.set_massacre(powerUp.getEffectAmount());
            break;
        default:
            std::cout << "Unknown power-up type!" << std::endl;
            break;
        }
    }
    Platform *getFirstPlatform()
    {
        return &platforms[0];
    }
    int getRightBound()
    {
        return game_right_bound;
    } 
    int getLeftBound()
    {
        return game_left_bound;
    }
};
