#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "AnimatedSprite.h"
#include "Projectile.h"
#include "Weapon.h"

class Enemy : public AnimatedSprite
{
private:
    float speed;
    int direction;

    int health_points = 10;

    sf::Texture enemy_texture;

    Weapon weapon;

    int game_left_bound;
    int game_right_bound;

    float scaleX;
    float scaleY;

public:
    Enemy(sf::Vector2f pos, float s, int dir, sf::Texture &texture, int left_bound, int right_bound, float sX, float sY) : AnimatedSprite(pos, 10), weapon(pos, WeaponType::SINGLE, left_bound, right_bound)
    {
        speed = s;
        direction = dir;

        scaleX = sX;
        scaleY = sY;

        if (direction == 1)
        {
            setScale(-scaleX, scaleY);
        }
        else if (direction == -1)
        {
            setScale(scaleX, scaleY);
        }

        enemy_texture = texture;

        game_left_bound = left_bound;
        game_right_bound = right_bound;

        setTexture(texture);
        step();
    }

    sf::Sprite &getSprite()
    {
        return *this;
    }

    void update(float dt, sf::RenderWindow &window)
    {
        sf::Vector2f weaponPos = getPosition();
        weaponPos.x += getGlobalBounds().width / 2.0f;
        weaponPos.y += getGlobalBounds().height / 2.0f;
        weapon.update(dt, weaponPos, window);

        int width = getGlobalBounds().width;
        int height = getGlobalBounds().height;

        step();
        draw(window);

        weapon.update(dt, getPosition() + sf::Vector2f(0, height / 2), window);
    }

    void move(float dt, float dy)
    {
        sf::Vector2f pos = getPosition();

        pos.x += speed * direction * dt;
        pos.y += dy;
        setPosition(pos);

        if (pos.x < 0 || pos.x > 800)
        {
            direction *= -1;
            bounce(pos, dy);
        }

        if (rand() % 100 < 5)
        {
            shoot(WeaponType::SINGLE);
        }

        weapon.move(0, dy);
    }

    void bounce(sf::Vector2f pos, float dy)
    {
        float width = getGlobalBounds().width;

        if (direction == 1)
        {
            setScale(-scaleX, scaleY);
            setPosition(pos.x + width / 2, pos.y);
        }
        else if (direction == -1)
        {
            setScale(scaleX, scaleY);
            setPosition(pos.x - width / 2, pos.y);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
        weapon.draw(window);

        // sf::RectangleShape hitbox;

        // hitbox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
        // hitbox.setPosition(getPosition().x - getGlobalBounds().width / 2, getPosition().y - getGlobalBounds().height / 2);
        // hitbox.setOutlineColor(sf::Color::Red);
        // hitbox.setOutlineThickness(1.0f);
        // hitbox.setFillColor(sf::Color::Transparent);

        // window.draw(hitbox);
    }

    void shoot(WeaponType type)
    {
        weapon.shoot(type, 180);
    }

    void update_health(int damage)
    {
        health_points -= damage;
    }

    int get_health()
    {
        return health_points;
    }

    int check_projeciltile_collision(sf::FloatRect bounds)
    {
        int result = weapon.check_collision(bounds);

        return result;
    }
};
