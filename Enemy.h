#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "AnimatedSprite.h"

#include "Projectile.h"

#include "Weapon.h"

class Enemy : public AnimatedSprite
{
private:
    float speed;
    int direction;

    float health_points;
    float damage;

    sf::Texture projectile_texture;
    sf::Texture enemy_texture;

    Weapon weapon;

public:
    Enemy(sf::Vector2f pos, float s, int dir, sf::Texture &texture, sf::Texture &proj_texture) : AnimatedSprite(pos, 10), weapon(pos, WeaponType::SINGLE)
    {
        speed = s;
        direction = dir;

        if (direction == 1)
        {
            setScale(-1, 1);
        }

        enemy_texture = texture;
        projectile_texture = proj_texture;

        setTexture(enemy_texture);
    }

    void update(float dt)
    {
        move(dt);

        sf::Vector2f weaponPos = getPosition();
        weaponPos.x += getGlobalBounds().width / 2.0f;
        weaponPos.y += getGlobalBounds().height / 2.0f;
        weapon.update(dt, weaponPos);

        step();
    }

    void move(float dt)
    {
        sf::Vector2f pos = getPosition();

        pos.x += speed * direction * dt;
        setPosition(pos);

        if (pos.x < 0 || pos.x > 800)
        {
            direction *= -1;
            bounce(pos);
        }

        if (rand() % 100 < 100) // 5% chance to shoot each frame
        {
            shoot(WeaponType::SINGLE);
        }

        weapon.update(dt, getPosition() + sf::Vector2f(getGlobalBounds().width / 2, 0));
    }

    void bounce(sf::Vector2f pos)
    {
        float width = getGlobalBounds().width;

        if (direction == 1)
        {
            setScale(-1, 1);
            setPosition(pos.x + width, pos.y);
        }
        else if (direction == -1)
        {
            setScale(1, 1);
            setPosition(pos.x - width, pos.y);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
        weapon.draw(window);

        sf::RectangleShape hitbox;

        // Update hitbox
        hitbox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
        hitbox.setPosition(getPosition());
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(1.0f);
        hitbox.setFillColor(sf::Color::Transparent);

        window.draw(hitbox);
    }

    void shoot(WeaponType type)
    {
        weapon.shoot(type, 180);
    }
};
