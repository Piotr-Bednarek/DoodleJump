#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "AnimatedSprite.h"

class Projectile : public sf::Sprite
{
private:
    sf::Vector2f position;
    float speed;
    int angle;

    int damage = 10;

    sf::Texture projectile_texture;

public:
    Projectile(sf::Vector2f pos, float s, int a, sf::Texture &texture, int dmg) : sf::Sprite(), position(pos), speed(s), angle(a), projectile_texture(texture), damage(dmg)
    {

        setTexture(texture);

        setPosition(position);

        sf::FloatRect bounds = getGlobalBounds();

        setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);

        rotate(angle);

        setScale(1.5f, 1.5f);
    }

    void update(float dt)
    {
        setTexture(projectile_texture);
        const double PI = 3.14159265358979323846;

        float radian_angle = (90 - angle) * PI / 180.0f;
        move(speed * cos(radian_angle) * dt, -speed * sin(radian_angle) * dt);
    }

    void rotate(float angle)
    {
        setRotation(angle);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
    }

    int get_damage()
    {
        return damage;
    }
};
