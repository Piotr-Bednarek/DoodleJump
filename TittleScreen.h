#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"

class TittleScreen
{

private:
    Button singleplayer_button;
    Button multiplayer_button;
    Button score_button;

    sf::Text title;
    sf::Text info;

public:
    TittleScreen(sf::Font &font, sf::RenderWindow &window)
        : singleplayer_button(sf::Vector2f(100, 450), sf::Vector2f(100, 100), "Singleplayer", font),
          multiplayer_button(sf::Vector2f(350, 450), sf::Vector2f(100, 100), "Multiplayer", font),
          score_button(sf::Vector2f(600, 450), sf::Vector2f(100, 100), "Score", font)
    {

        int WIDTH = window.getSize().x;

        title = createText("Doodle Jump!", font, 100, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 75));
        info = createText("Press SPACE to start", font, 50, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 150));
    }

    void draw(sf::RenderWindow &window)
    {
        singleplayer_button.draw(window);
        multiplayer_button.draw(window);
        score_button.draw(window);

        window.draw(title);
        window.draw(info);
    }

    void update(sf::RenderWindow &window)
    {
        singleplayer_button.update(window);
        multiplayer_button.update(window);
        score_button.update(window);
    }

    sf::Text createText(const std::string &text, const sf::Font &font, int size, const sf::Color &color, const sf::Vector2f &position)
    {
        sf::Text sfText(text, font, size);
        sfText.setFillColor(color);

        sf::FloatRect textRect = sfText.getLocalBounds();
        sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        sfText.setPosition(position);

        return sfText;
    }
};
