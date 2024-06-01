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
            shootDelay = sf::milliseconds(1);
            break;
        }
    }

    void shoot(WeaponType type, int angle_offset)
    {

        std::cout << "Projectile count: " << projectiles.size() << std::endl;

        setType(type);

        if (clock.getElapsedTime() >= shootDelay)
        {

            if (type == WeaponType::SINGLE || type == WeaponType::MACHINEGUN)
            {
                create_projectile(position, projectile_speed, 0 + angle_offset);
            }
            else if (type == WeaponType::TRIPLE)
            {

                create_projectile(position, projectile_speed, 0 + angle_offset);
                create_projectile(position, projectile_speed, 45 + angle_offset);
                create_projectile(position, projectile_speed, -45 + angle_offset);
            }

            clock.restart();
        }
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
            projectile.draw(window);
        }
    };

    void create_projectile(sf::Vector2f position, float speed, int angle_offset)
    {
        Projectile projectile(position, speed, angle_offset, projectile_texture[1]);

        projectiles.emplace_back(projectile);
    }
};