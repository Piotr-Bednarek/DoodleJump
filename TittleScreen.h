#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"

class TittleScreen
{

private:
    Button singleplayer_button;
    Button multiplayer_button;
    Button score_button;

public:
    TittleScreen(sf::Font &font)
        : singleplayer_button(sf::Vector2f(100, 450), sf::Vector2f(100, 100), "Singleplayer", font),
          multiplayer_button(sf::Vector2f(350, 450), sf::Vector2f(100, 100), "Multiplayer", font),
          score_button(sf::Vector2f(600, 450), sf::Vector2f(100, 100), "Score", font)
    {
    }

    void draw(sf::RenderWindow &window)
    {
        singleplayer_button.draw(window);
        multiplayer_button.draw(window);
        score_button.draw(window);
    }

    void update(sf::RenderWindow &window)
    {
        singleplayer_button.update(window);
        multiplayer_button.update(window);
        score_button.update(window);
    }
};
