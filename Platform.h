#pragma once

#include <SFML/Graphics.hpp>

class Platform : public sf::RectangleShape
{
public:
    Platform(sf::Vector2f pos, sf::Vector2f s) : sf::RectangleShape()
    {
        setPosition(pos);
        setSize(s);
        setFillColor(sf::Color::Green);
    }
};
