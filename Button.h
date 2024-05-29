#pragma once

#include <SFML/Graphics.hpp>

class Button : public sf::RectangleShape
{
private:
    sf::Text text;

public:
    Button(sf::Vector2f pos, sf::Vector2f s, std::string t, sf::Font &font) : sf::RectangleShape()
    {
        setPosition(pos);
        setSize(s);
        setFillColor(sf::Color::Green);

        text = sf::Text(t, font, 40);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(pos.x + s.x / 2.0f, pos.y + s.y / 2.0f + s.y);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);

        window.draw(text);
    }

    void update(sf::RenderWindow &window)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {
            setFillColor(sf::Color::Red);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                setFillColor(sf::Color::Blue);
            }
        }
        else
        {
            setFillColor(sf::Color::Green);
        }
    }
};
