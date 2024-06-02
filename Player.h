#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Weapon.h"
#include "Enemy.h"

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

    int game_left_bound;
    int game_right_bound;

    Weapon weapon;

    int health_points = 100;

public:
    Player(sf::Vector2f pos, sf::Vector2f s, int left_bound, int right_bound) : sf::RectangleShape(), weapon(pos, WeaponType::SINGLE, left_bound, right_bound)
    {
        setPosition(pos);
        setSize(s);

        game_left_bound = left_bound;
        game_right_bound = right_bound;
    }

    void update(float dt, sf::RenderWindow &window)
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

        weapon.update(dt, getPosition() + sf::Vector2f(getSize().x / 2, 0), window);
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
    }

    void check_projeciltile_collision(std::vector<Enemy> &enemies)
    {
        for (Enemy &enemy : enemies)
        {

            sf::FloatRect bounds = enemy.getGlobalBounds();

            int result = weapon.check_collision(bounds);

            if (result != -1)
            {
                std::cout << "Collision" << std::endl;

                enemy.update_health(result);
            }
        }
    }

    void update_health(int damage)
    {
        health_points -= damage;

        std::cout << "Health: " << health_points << std::endl;
    }

    int get_health()
    {
        return health_points;
    }
};