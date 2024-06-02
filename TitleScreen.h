#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"

enum class GameState
{
    TITLE,
    SINGLEPLAYER,
    MULTIPLAYER,
    GAMEOVER
};

class TitleScreen
{
private:
    Button singleplayer_button;
    Button multiplayer_button;
    Button score_button;

    std::vector<Button> buttons;

    sf::Text title;
    sf::Text info;

    sf::Texture singleplayer_button_texture;
    sf::Texture multiplayer_button_texture;
    sf::Texture score_button_texture;

public:
    TitleScreen(sf::Font &font, sf::RenderWindow &window, GameState &game_state)
    {

        if (!singleplayer_button_texture.loadFromFile("assets/singleplayer_button.png"))
        {
            std::cout << "Failed to load texture from assets/singleplayer_button.png" << std::endl;
        }

        if (!multiplayer_button_texture.loadFromFile("assets/multiplayer_button.png"))
        {
            std::cout << "Failed to load texture from assets/multiplayer_button.png" << std::endl;
        }

        int WIDTH = window.getSize().x;

        create_buttons(font, game_state);

        title = createText("Doodle Jump!", font, 100, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 75));
        info = createText("Press SPACE to start", font, 50, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 150));
    }

    void create_buttons(sf::Font &font, GameState &game_state)
    {

        singleplayer_button = Button(sf::Vector2f(100, 450), sf::Vector2f(100, 100), "Singleplayer", font, singleplayer_button_texture, [&game_state]
                                     { game_state = GameState::SINGLEPLAYER; });

        multiplayer_button = Button(sf::Vector2f(350, 450), sf::Vector2f(100, 100), "Multiplayer", font, multiplayer_button_texture, [&game_state]
                                    { game_state = GameState::MULTIPLAYER; });

        score_button = Button(sf::Vector2f(600, 450), sf::Vector2f(100, 100), "Score", font, singleplayer_button_texture, [&game_state]
                              { game_state = GameState::GAMEOVER; });
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
