#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "AnimatedSprite.h"

class Projectile : public AnimatedSprite
{
private:
    sf::Vector2f position;
    sf::Vector2f size;
    float speed;
    float angle;

public:
    Projectile(sf::Vector2f pos, sf::Vector2f sprite_size, float s, float a) : AnimatedSprite(pos, 10)
    {
        position = pos;
        speed = s;
        angle = a;

        size = sprite_size;

        setPosition(position);
        rotate(angle - 90);
    }

    void update(float dt)
    {
        const double PI = 3.14159265358979323846;

        float radian_angle = (90 - angle) * PI / 180.0f;
        move(speed * cos(radian_angle) * dt, -speed * sin(radian_angle) * dt);

        step();
    }

    void rotate(float a)
    {
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
        setRotation(a);
    }

    void draw(sf::RenderWindow &window)
    {
        static bool firstDraw = true;
        if (firstDraw)
        {
            firstDraw = false;
            return;
        }

        window.draw(*this);
    }
};
