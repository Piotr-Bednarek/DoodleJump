#pragma once

#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite
{
private:
    sf::Vector2f position;
    float speed;
    float angle;
    const double PI = 3.14159265358979323846;

public:
    Projectile(sf::Vector2f pos, float s, float a, sf::Texture &projectile_texture) : sf::Sprite()
    {
        position = pos;
        speed = s;
        angle = a;

        setTexture(projectile_texture);
        setPosition(position);
        rotate(angle);
    }

    void update(sf::Time dt)
    {
        float radian_angle = (90 - angle) * PI / 180.0f;
        move(speed * cos(radian_angle) * dt.asSeconds(), -speed * sin(radian_angle) * dt.asSeconds());
    }

    void rotate(float a)
    {
        setOrigin(getLocalBounds().width / 2, getLocalBounds().height / 2);
        setRotation(a);
    }
};
