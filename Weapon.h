#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "Projectile.h"
#include <iostream>

enum class WeaponType
{
    SINGLE,
    MACHINEGUN,
    TRIPLE,
};

enum class ProjectileType
{
    FIREBALL,
    SHURIKEN,
};

class Weapon
{
private:
    WeaponType type;
    sf::Vector2f position;

    std::vector<Projectile> projectiles;
    std::vector<sf::Texture> projectile_texture;
    ;
    float projectile_speed = 200;

    sf::Clock clock;
    sf::Time shootDelay;
    int burstCounter = 0;

public:
    Weapon(sf::Vector2f pos, WeaponType initial_type)
    {
        position = pos;
        type = initial_type;

        setType(type);

        std::vector<std::string> texturePaths = {"assets/fireball_projectile.png", "assets/shuriken_projectile.png"};

        for (const auto &path : texturePaths)
        {
            sf::Texture texture;
            if (!texture.loadFromFile(path))
            {
                std::cout << "Failed to load texture from " << path << std::endl;
                continue;
            }
            projectile_texture.push_back(std::move(texture));
        }
    }

    void setType(WeaponType newType)
    {
        type = newType;

        switch (type)
        {
        case WeaponType::SINGLE:
            shootDelay = sf::milliseconds(500);
            break;
        case WeaponType::MACHINEGUN:
            shootDelay = sf::milliseconds(100);
            break;
        case WeaponType::TRIPLE:
            shootDelay = sf::milliseconds(1000);
            break;
        }
    }

    void shoot(WeaponType type, int angle_offset)
    {

        std::cout << "Shooting" << std::endl;

        setType(type);

        if (type == WeaponType::SINGLE)
        {
            if (clock.getElapsedTime() >= shootDelay)
            {
                Projectile projectile(position, sf::Vector2f(64, 32), projectile_speed, 0 + angle_offset);

                for (int i = 0; i < 5; i++)
                {
                    projectile.add_animation_frame(sf::IntRect(64 * i, 0, 64, 32));
                }

                projectile.setTexture(projectile_texture[0]);

                projectiles.emplace_back(projectile);

                clock.restart();
            }
        }
        // else if (type == WeaponType::MACHINEGUN)
        // {
        //     if (clock.getElapsedTime() >= shootDelay)
        //     {
        //         Projectile projectile(position, projectile_speed, 0 + angle_offset, ProjectileType::FIREBALL);

        //         projectiles.emplace_back(projectile);

        //         clock.restart();
        //     }
        // }
        // else if (type == WeaponType::TRIPLE)
        // {
        //     if (clock.getElapsedTime() >= shootDelay)
        //     {
        //         Projectile projectile1(position, projectile_speed, 0 + angle_offset, ProjectileType::FIREBALL);
        //         Projectile projectile2(position, projectile_speed, 45 + angle_offset, ProjectileType::FIREBALL);
        //         Projectile projectile3(position, projectile_speed, -45 + angle_offset, ProjectileType::FIREBALL);

        //         projectiles.emplace_back(projectile1);
        //         projectiles.emplace_back(projectile2);
        //         projectiles.emplace_back(projectile3);

        //         clock.restart();
        //     }
        // }
    }

    void update(float dt, sf::Vector2f pos)
    {
        position = pos;

        for (Projectile &projectile : projectiles)
        {
            projectile.update(dt);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        for (Projectile &projectile : projectiles)
        {
            projectile.step();

            if (projectile.getGlobalBounds().width <= 64 && projectile.getGlobalBounds().height <= 64)
            {
                projectile.draw(window);
            }
        }
    };
};