#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Player.h"

class InputField : public sf::Sprite
{
private:
    std::string value = "";

    bool is_active = false;

    sf::Vector2f position;
    sf::Vector2f size;

    sf::Text text;
    int char_lmit = 10;

    sf::Font font;

    sf::Texture texture;

    Player *player;

public:
    InputField(sf::Vector2f pos, sf::Vector2f s, sf::Font &font_, Player *playerD) : sf::Sprite(), font(font_)
    {
        if (!texture.loadFromFile("assets/buttons/inputfield_background.png"))
        {
            std::cout << "Failed to load inputfield texture" << std::endl;
        }

        position = pos;
        size = s;

        setTexture(texture);
        setPosition(position.x - size.x / 2, position.y - size.y / 2);
        setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);

        text.setFont(font);
        text.setCharacterSize(30);
        text.setFillColor(sf::Color::Black);
        text.setPosition(position.x - size.x / 2 + 12, position.y - size.y / 2 + 24);

        this->player = playerD;
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
        window.draw(text);
    }

    void update(sf::RenderWindow &window)
    {
        text.setString(value);
        float textWidth = text.getLocalBounds().width;
        text.setPosition(position.x - textWidth / 2, position.y - size.y / 2 + 18);

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

        if (getGlobalBounds().contains(mouse_pos.x, mouse_pos.y))
        {

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                setColor(sf::Color(120, 120, 120, 255));
                is_active = true;
            }
        }
        else if (!is_active)
        {
            setColor(sf::Color::White);
        }
    }

    bool is_field_active()
    {
        return is_active;
    }

    void handle_event(sf::Event &event)
    {
        if (is_active)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z && value.size() < char_lmit)
                {
                    char letter = 'A' + (event.key.code - sf::Keyboard::A);
                    value += std::toupper(letter);
                }

                if (event.key.code == sf::Keyboard::Enter)
                {
                    is_active = false;

                    player->setName(value);
                }
                else if (event.key.code == sf::Keyboard::BackSpace)
                {
                    if (value.size() > 0)
                    {
                        value.pop_back();
                    }
                }
            }
        }
    }
    void updatePointer(Player *playerD)
    {
        player = playerD;
    }
    void setText(std::string s)
    {
        value = s;
    }
};
