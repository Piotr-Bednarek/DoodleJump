#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Weapon.h"

class Player : public sf::RectangleShape
{
private:
    float gravity = 500;
    float jump_force = 500;

    sf::Vector2f velocity;
    bool is_on_ground = false;

    float speed = 5;
    float air_speed = 10;

    float friction = 0.9;
    float air_friction = 0.9;

    Weapon weapon;

public:
    Player(sf::Vector2f pos, sf::Vector2f s) : sf::RectangleShape(), weapon(pos, WeaponType::SINGLE)
    {
        setPosition(pos);
        setSize(s);
    }

    void update(float dt)
    {
        if (is_on_ground)
        {
            velocity.y = 0;
            velocity.x *= friction;
        }
        else
        {
            velocity.y += gravity * dt;
            velocity.x *= air_friction;
        }

        sf::Vector2f pos = getPosition();
        pos.x += velocity.x * dt;
        pos.y += velocity.y * dt;
        setPosition(pos);

        weapon.update(dt, getPosition() + sf::Vector2f(getSize().x / 2, 0));
    }

    void move(float dx, float dy)
    {
        sf::Vector2f pos = getPosition();
        pos.x += dx * speed;
        pos.y += dy;
        setPosition(pos);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);

        weapon.draw(window);
    }

    void jump()
    {
        if (is_on_ground)
        {
            is_on_ground = false;
            velocity.y = -jump_force;
        }
    }

    sf::Vector2f get_velocity()
    {
        return velocity;
    }

    void set_ground(bool ground)
    {
        is_on_ground = ground;
    }

    void shoot(WeaponType type)
    {
        weapon.shoot(type, 0);

        std::cout << "Shooting" << std::endl;
    }
};
