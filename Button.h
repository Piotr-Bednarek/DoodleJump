#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Button : public sf::Sprite
{
private:
    sf::Text text;

    float scaleX;
    float scaleY;

    sf::Vector2f position;
    sf::Vector2f size;

public:
    Button() : sf::Sprite()
    {
        setPosition(sf::Vector2f(0, 0));

        // setTextureRect(sf::IntRect(0, 0, 0, 0));

        // text = sf::Text("", sf::Font(), 40);

        // sf::FloatRect textRect = text.getLocalBounds();
        // text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        // text.setPosition(0, 0);
    }

    Button(sf::Vector2f pos, sf::Vector2f s, std::string t, sf::Font &font, sf::Texture &texture) : sf::Sprite()
    {
        sf::Vector2u textureSize = texture.getSize();

        if (textureSize.x == 0 && textureSize.y == 0)
        {
            std::cout << "Failed to load button texture" << std::endl;
            return;
        }

        position = pos;
        size = s;

        setPosition(position);
        setTexture(texture);

        scaleX = size.x / textureSize.x;
        scaleY = size.y / textureSize.y;
        setScale(scaleX, scaleY);

        text = sf::Text(t, font, 40);

        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f + size.y);
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

            setColor(sf::Color(120, 120, 120, 255));
            setScale(scaleX * 1.2, scaleY * 1.2);

            center_button();

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                setColor(sf::Color(180, 180, 180, 255));
                setScale(scaleX * 0.975, scaleY * 0.975);

                center_button();
            }
        }
        else
        {
            setColor(sf::Color::White);
            setScale(scaleX, scaleY);

            center_button();
        }
    }

    void center_button()
    {
        sf::FloatRect bounds = getGlobalBounds();
        setPosition(position.x + size.x / 2.0f - bounds.width / 2.0f, position.y + size.y / 2.0f - bounds.height / 2.0f);
    }
};
