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
    int left_bound = 0;
    int WIDTH;
    int HEIGHT;

    Button singleplayer_button;
    Button multiplayer_button;
    Button score_button;
    Button home_button;

    std::vector<Button> buttons;

    sf::Font font;

    sf::Text title;
    sf::Text info;
    sf::Text highScoresText;

    sf::Texture singleplayer_button_texture;
    sf::Texture multiplayer_button_texture;
    sf::Texture score_button_texture;
    sf::Texture home_button_texture;

    sf::Clock clock;

public:
    TitleScreen(sf::Font &font_, int left, int width, int height, GameState &game_state) : font(font_),left_bound(left), WIDTH(width), HEIGHT(height)
    {
        if (!singleplayer_button_texture.loadFromFile("assets/buttons/singleplayer_button.png"))
        {
            std::cout << "Failed to load texture from assets/buttons/singleplayer_button.png" << std::endl;
        }

        if (!multiplayer_button_texture.loadFromFile("assets/buttons/multiplayer_button.png"))
        {
            std::cout << "Failed to load texture from assets/buttons/multiplayer_button.png" << std::endl;
        }

        if (!score_button_texture.loadFromFile("assets/buttons/score_button.png"))
        {
            std::cout << "Failed to load texture from assets/buttons/score_button.png" << std::endl;
        }

        if (!home_button_texture.loadFromFile("assets/buttons/home_button.png"))
        {
            std::cout << "Failed to load texture from assets/buttons/home_button.png" << std::endl;
        }


        create_buttons(font, game_state);

        title = createText("Doodle Jump!", font, 100, sf::Color::Black, sf::Vector2f(left + (WIDTH-left) / 2.0f, 75));
        info = createText("Press SPACE to start", font, 50, sf::Color::Black, sf::Vector2f(left + (WIDTH-left) / 2.0f, 150));
    }

    void create_buttons(sf::Font &font, GameState &game_state)
    {
        int center_x = left_bound + (WIDTH-100 - left_bound) / 2.0f;
        singleplayer_button = Button(sf::Vector2f(center_x-250, 450), sf::Vector2f(100, 100), "Singleplayer", font, singleplayer_button_texture, [&game_state]
                                     { game_state = GameState::SINGLEPLAYER; });

        multiplayer_button = Button(sf::Vector2f(center_x, 450), sf::Vector2f(100, 100), "Multiplayer", font, multiplayer_button_texture, [&game_state]
                                    { game_state = GameState::MULTIPLAYER; });

        score_button = Button(sf::Vector2f(center_x+250, 450), sf::Vector2f(100, 100), "Score", font, score_button_texture, [&game_state]
                              { game_state = GameState::GAMEOVER; });

        home_button = Button(sf::Vector2f(center_x, 600), sf::Vector2f(100, 100), "Home", font, home_button_texture, [&game_state]
                             { game_state = GameState::TITLE; });
    }

    void draw(sf::RenderWindow &window)
    {
        singleplayer_button.draw(window);
        multiplayer_button.draw(window);
        score_button.draw(window);
        // home_button.draw(window);

        window.draw(title);
        window.draw(info);
    }

    void update(sf::RenderWindow &window)
    {
        if (clock.getElapsedTime().asMilliseconds() > 300)
        {
            singleplayer_button.update(window);
            multiplayer_button.update(window);
            score_button.update(window);
        }
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

    void updateHighScore(HighScoreManager &highScoreManager)
    {
        highScoresText.setFillColor(sf::Color::White);
        highScoresText.setFont(font);
        highScoresText.setCharacterSize(50);
        auto updateHighScoresText = [&]()
        {
            std::string highScoresString = "High Scores\n";
            const auto &scores = highScoreManager.getHighScores();
            for (const auto &hs : scores)
            {
                highScoresString += "   " + hs.name + ": " + std::to_string(hs.score) + "\n";
            }
            highScoresText.setString(highScoresString);
        };
        updateHighScoresText();
        sf::FloatRect textRect = highScoresText.getLocalBounds();
        highScoresText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        highScoresText.setPosition(sf::Vector2f(left_bound + (WIDTH-left_bound) / 2.0f, HEIGHT / 2.0f));
    }

    void drawGameOver(sf::RenderWindow &window, HighScoreManager &highScoreManager)
    {
        updateHighScore(highScoreManager);
        highScoreManager.saveHighScores();
        window.draw(highScoresText);

        home_button.draw(window);
        home_button.update(window);
        if (home_button.isClicked(window))
        {
            clock.restart();
        }
    }
    void drawGameOverMultiplayer(sf::RenderWindow &window, HighScoreManager &highScoreManager)
    {
        updateHighScore(highScoreManager);
        highScoreManager.saveHighScores();
        window.draw(highScoresText);

        home_button.draw(window);
        home_button.update(window);
        if (home_button.isClicked(window))
        {
            clock.restart();
        }
    }
    void drawMultiplayer(sf::RenderWindow &window)
    {
        //singleplayer_button.draw(window);
       // multiplayer_button.draw(window);
        score_button.draw(window);
        // home_button.draw(window);

        window.draw(title);
        window.draw(info);
    }
};
