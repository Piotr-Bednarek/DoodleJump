#pragma once

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"

class Enemy : public AnimatedSprite
{
private:
    float speed;
    int direction;

    float health_points;
    float damage;

public:
    Enemy(sf::Vector2f pos, float s, int dir) : AnimatedSprite(pos, 10)
    {
        speed = s;
        direction = dir;

        if (direction == 1)
        {
            setScale(-1, 1);
        }
    }

    void move(float deltaTime)
    {
        sf::Vector2f pos = getPosition();
        pos.x += speed * direction * deltaTime;
        setPosition(pos);

        if (pos.x < 0 || pos.x > 800)
        {
            direction *= -1;
            bounce(pos);
        }
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
};
