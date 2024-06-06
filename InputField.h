#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class InputField : public sf::RectangleShape
{
private:
    std::string value = "";

    bool is_active = false;

    sf::Vector2f position;
    sf::Vector2f size;

    sf::Text text;

public:
    InputField(sf::Vector2f pos, sf::Vector2f s) : sf::RectangleShape()
    {
        position = pos;
        size = s;

        setOrigin(size.x / 2, size.y / 2);
        setPosition(position);
        setSize(size);

        setFillColor(sf::Color::White);

        sf::Text text(value, sf::Font(), 24);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
    }

    void update(sf::RenderWindow &window)
    {
        text.setString(value);

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                setFillColor(sf::Color(120, 120, 120, 255));
                is_active = true;
            }
        }
    }

    bool is_field_active()
    {
        return is_active;
    }

    void handle_event(sf::Event &event)
    {
        std::cout << "handle_event called with event type: " << event.type << std::endl;

        if (is_active)
        {
            std::cout << "Input field is active" << std::endl;

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code: " << event.key.code << std::endl;

                if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
                {
                    char letter = 'A' + (event.key.code - sf::Keyboard::A);
                    value += letter;
                    std::cout << "Letter " << letter << " was pressed" << std::endl;
                }

                // You can handle specific keys like this:
                if (event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Enter key was pressed" << std::endl;
                    // Handle Enter key...
                }
                else if (event.key.code == sf::Keyboard::BackSpace)
                {
                    std::cout << "Backspace key was pressed" << std::endl;
                    // Handle Backspace key...
                }
                // Add more else if blocks for other keys you want to handle...
            }
        }
    }
};
