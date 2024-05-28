#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "Platform.h"
#include "Enemy.h"
#include "Game.h"

enum class GameState
{
    TITLE,
    SINGLEPLAYER,
    MULTIPLAYER,
    GAMEOVER

};

sf::Text createText(const std::string &text, const sf::Font &font, int size, const sf::Color &color, const sf::Vector2f &position)
{
    sf::Text sfText(text, font, size);
    sfText.setFillColor(color);

    sf::FloatRect textRect = sfText.getLocalBounds();
    sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    sfText.setPosition(position);

    return sfText;
}

int main()
{
    srand(time(NULL));
    sf::Clock clock;

    const int WIDTH = 800;
    const int HEIGHT = 800;
    const int FPS = 60;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);

    window.setFramerateLimit(FPS);
    window.setVerticalSyncEnabled(true);

    // ----------------------------------------------

    sf::Texture enemy_flying_texture;
    sf::Texture background_texture;

    sf::Font font;

    if (!enemy_flying_texture.loadFromFile("assets/enemy/FLYING.png"))
    {
        return 1;
    }

    if (!background_texture.loadFromFile("assets/background.png"))
    {
        return 1;
    }

    if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
    {
        return 1;
    }

    sf::Text title = createText("Doodle Jump!", font, 100, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 75));
    sf::Text info = createText("Press SPACE to start", font, 50, sf::Color::Black, sf::Vector2f(WIDTH / 2.0f, 150));
    sf::Text score = createText("Score: 0", font, 50, sf::Color::Black, sf::Vector2f(100, 40));

    // ----------------------------------------------

    Game game(50, 250, 0, WIDTH);

    GameState state = GameState::SINGLEPLAYER;

    Enemy enemy1(sf::Vector2f(100, 100), 200, 1.0);

    enemy1.setTexture(enemy_flying_texture);

    for (int i = 0; i < 4; ++i)
    {
        enemy1.add_animation_frame(sf::IntRect(81 * i, 0, 71, 81));
    }

    // ----------------------------------------------

    game.create_platforms(50, 100, 20, HEIGHT, WIDTH);

    // ----------------------------------------------

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                if (state == GameState::TITLE)
                    state = GameState::SINGLEPLAYER;

                game.jump();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace)
            {
                state = GameState::GAMEOVER;

                int final_score = game.end();

                score.setString("Score: " + std::to_string(static_cast<int>(std::round(final_score))));
            }
        }

        window.clear();

        window.draw(sf::Sprite(background_texture));

        switch (state)
        {
        case GameState::TITLE:
            window.draw(title);
            window.draw(info);

            break;
        case GameState::SINGLEPLAYER:
            game.draw(window);
            game.update(dt, window);

            window.draw(enemy1);
            enemy1.step();
            enemy1.move(1.0 / FPS);

            score.setString("Score: " + std::to_string(static_cast<int>(std::round(game.get_score()))));

            window.draw(score);

            break;

        case GameState::MULTIPLAYER:
            break;
        case GameState::GAMEOVER:

            window.draw(title);
            window.draw(score);
            score.setPosition(WIDTH / 2.0f, 150);

            break;
        }

        window.display();
    }

    return 0;
}
