#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>

#include "Projectile.h"

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

    int game_left_bound;
    int game_right_bound;

    ProjectileType projectile_type;

public:
    Weapon(sf::Vector2f pos, WeaponType initial_type, int left_bound, int right_bound, ProjectileType proj_type) : position(pos), type(initial_type), game_left_bound(left_bound), game_right_bound(right_bound), projectile_type(proj_type)
    {
        setType(type);

        std::vector<std::string> texturePaths = {"assets/enviroment/fireball_projectile.png", "assets/enviroment/shuriken_projectile.png"};

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
            shootDelay = sf::milliseconds(200);
            break;
        case WeaponType::TRIPLE:
            shootDelay = sf::milliseconds(500);
            break;
        }
    }

    bool shoot(WeaponType type, int angle_offset)
    {

        setType(type);

        if (clock.getElapsedTime() >= shootDelay)
        {

            if (type == WeaponType::SINGLE)
            {
                create_projectile(position, projectile_speed, 0 + angle_offset, 10);
            }
            else if (type == WeaponType::MACHINEGUN)
            {
                create_projectile(position, projectile_speed, 0 + angle_offset, 5);
            }
            else if (type == WeaponType::TRIPLE)
            {

                create_projectile(position, projectile_speed, 0 + angle_offset, 15);
                create_projectile(position, projectile_speed, 45 + angle_offset, 15);
                create_projectile(position, projectile_speed, -45 + angle_offset, 15);
            }

            clock.restart();

            return true;
        }

        return false;
    }

    void update(float dt, sf::Vector2f pos, sf::RenderWindow &window)
    {
        position = pos;

        for (auto it = projectiles.begin(); it != projectiles.end();)
        {
            it->update(dt);

            if (it->getPosition().x < game_left_bound || it->getPosition().x > game_right_bound || it->getPosition().y < 0 || it->getPosition().y > window.getSize().y)
            {
                it = projectiles.erase(it);
            }
            else
            {
                it++;
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {
        for (Projectile &projectile : projectiles)
        {
            projectile.draw(window);
        }
    };

    void create_projectile(sf::Vector2f position, float speed, int angle_offset, int damage)
    {

        if (projectile_type == ProjectileType::FIREBALL)
        {
            Projectile projectile(position, speed, angle_offset, projectile_texture[0], damage);
            projectiles.emplace_back(projectile);
        }
        else if (projectile_type == ProjectileType::SHURIKEN)
        {
            Projectile projectile(position, speed, angle_offset, projectile_texture[1], damage);
            projectiles.emplace_back(projectile);
        }
    }

    void move(float dx, float dy)
    {
        for (Projectile &projectile : projectiles)
        {
            projectile.move(dx, dy);
        }
    }

    void move(float width)
    {
        position.x += width;
    }

    int check_collision(sf::FloatRect bounds)
    {
        for (int i = 0; i < projectiles.size(); i++)
        {
            if (projectiles[i].getGlobalBounds().intersects(bounds))
            {
                int damage = projectiles[i].get_damage();
                projectiles.erase(projectiles.begin() + i);
                return damage;
            }
        }

        return -1;
    }
};