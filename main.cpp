#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#include "Platform.h"
#include "Enemy.h"
#include "Game.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Button.h"
#include "TitleScreen.h"

#include "Player.h"

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

    int WIDTH = 800;
    int HEIGHT = 800;
    int FPS = 60;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);

    window.setFramerateLimit(FPS);

    window.setVerticalSyncEnabled(true);

    // ----------------------------------------------

    sf::Texture enemy_flying_texture;
    sf::Texture background_texture;

    sf::Texture platform_grass_texture;
    sf::Texture platform_stone_texture;

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

    sf::Text score = createText("Score: 0", font, 50, sf::Color::Black, sf::Vector2f(100, 40));

    // ----------------------------------------------

    Game game(0, 350, 0, WIDTH);

    GameState state = GameState::SINGLEPLAYER;

    TitleScreen tittle_screen(font, window);

    Enemy enemy1(sf::Vector2f(100, 100), 200, 1.0);

    Player player(sf::Vector2f(400, 400), sf::Vector2f(50, 50));

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
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // if (event.type == sf::Event::KeyPressed && aw

            // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
            // {
            //     player.move(sf::Vector2f(0, 50 * dt));
            // }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
            {
                player.move(sf::Vector2f(-150 * dt, 0));
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
            {
                player.move(sf::Vector2f(150 * dt, 0));
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                if (state == GameState::TITLE)
                    state = GameState::SINGLEPLAYER;

                // game.jump();

                player.jump();
            }

            // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Backspace)
            // {
            //     state = GameState::GAMEOVER;

            //     int final_score = game.end();

            //     score.setString("Score: " + std::to_string(static_cast<int>(std::round(final_score))));
            // }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            {
                game.shoot(WeaponType::SINGLE);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
            {
                game.shoot(WeaponType::MACHINEGUN);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
            {
                game.shoot(WeaponType::TRIPLE);
            }
        }

        window.clear();

        window.draw(sf::Sprite(background_texture));

        switch (state)
        {
        case GameState::TITLE:

            tittle_screen.update(window);
            tittle_screen.draw(window);

            break;
        case GameState::SINGLEPLAYER:

            game.check_collision(player);

            player.update(dt);

            game.draw(window);
            game.update(dt, window);

            window.draw(enemy1);
            enemy1.step();
            enemy1.move(1.0 / FPS);

            player.draw(window);

            score.setString("Score: " + std::to_string(static_cast<int>(std::round(game.get_score()))));

            window.draw(score);

            break;

        case GameState::MULTIPLAYER:
            break;
        case GameState::GAMEOVER:

            window.draw(score);
            score.setPosition(WIDTH / 2.0f, 150);

            break;
        }

        window.display();
    }

    return 0;
}
