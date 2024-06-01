#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Player : public sf::RectangleShape
{
private:
    float gravity = 500;
    float jump_force = 500;
    sf::Vector2f velocity;
    bool is_on_ground = false;
    // bool is_falling = false;

public:
    Player(sf::Vector2f pos, sf::Vector2f s) : sf::RectangleShape()
    {
        setPosition(pos);
        setSize(s);
    }

    void update(float dt)
    {

        // std::cout << "On ground: " << is_on_ground << std::endl;
        // std::cout << "Falling: " << is_falling << std::endl;

        if (is_on_ground)
        {
            velocity.y = 0;
            // is_falling = false;
        }
        // else if (is_falling)
        else
        {
            velocity.y += gravity * dt;
            sf::Vector2f pos = getPosition();
            pos.y += velocity.y * dt;
            setPosition(pos);
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
    }

    void jump()
    {
        std::cout << "Jumping: " << is_on_ground << std::endl;
        if (is_on_ground)
        {
            is_on_ground = false;
            // is_falling = true;
            velocity.y = -jump_force;
        }

        // std::cout << velocity.y << std::endl;
        // std::cout << -jump_force << std::endl;
    }

    sf::Vector2f get_velocity()
    {
        return velocity;
    }

    void set_ground(bool ground)
    {
        is_on_ground = ground;

        // std::cout << "Ground: " << is_on_ground << std::endl;
    }
};
