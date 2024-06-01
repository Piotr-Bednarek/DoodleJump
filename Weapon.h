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

class Weapon
{
private:
    WeaponType type;
    sf::Vector2f position;

    std::vector<Projectile> projectiles;
    sf::Texture projectile_texture;
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

        std::string path = "assets/projectile.png";

        if (!projectile_texture.loadFromFile(path))
        {
            std::cout << "Failed to load texture from " << path << std::endl;
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

    void shoot(WeaponType type)
    {

        setType(type);

        if (type == WeaponType::SINGLE)
        {
            if (clock.getElapsedTime() >= shootDelay)
            {
                Projectile projectile(position, projectile_speed, 0, projectile_texture);

                projectiles.emplace_back(projectile);

                clock.restart();
            }
        }
        else if (type == WeaponType::MACHINEGUN)
        {
            if (clock.getElapsedTime() >= shootDelay)
            {
                Projectile projectile(position, projectile_speed, 0, projectile_texture);

                projectiles.emplace_back(projectile);

                clock.restart();
            }
        }
        else if (type == WeaponType::TRIPLE)
        {
            if (clock.getElapsedTime() >= shootDelay)
            {
                Projectile projectile1(position, projectile_speed, 0, projectile_texture);
                Projectile projectile2(position, projectile_speed, 45, projectile_texture);
                Projectile projectile3(position, projectile_speed, -45, projectile_texture);

                projectiles.emplace_back(projectile1);
                projectiles.emplace_back(projectile2);
                projectiles.emplace_back(projectile3);

                clock.restart();
            }
        }
    }

    void update(float dt)
    {
        for (Projectile &projectile : projectiles)
        {
            projectile.update(dt);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        for (Projectile &projectile : projectiles)
        {
            window.draw(projectile);
        }
    }
};
