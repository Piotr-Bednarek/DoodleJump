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

    bool moveLeft = false;
    bool moveRight = false;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);

    window.setFramerateLimit(FPS);

    window.setVerticalSyncEnabled(true);

    // ----------------------------------------------

    // sf::Texture enemy_flying_texture;
    sf::Texture background_texture;

    // sf::Texture projectile_texture;

    sf::Texture platform_grass_texture;
    sf::Texture platform_stone_texture;

    // if (!enemy_flying_texture.loadFromFile("assets/enemy/dragon_flying.png"))
    // {
    //     return 1;
    // }

    if (!background_texture.loadFromFile("assets/enviroment/background.png"))
    {
        return 1;
    }

    // if (!projectile_texture.loadFromFile("assets/fireball_projectile.png"))
    // {
    //     return 1;
    // }

    sf::Font font;

    if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
    {
        return 1;
    }

    sf::Text score = createText("Score: 0", font, 50, sf::Color::Black, sf::Vector2f(100, 40));

    // ----------------------------------------------

    Game game(0, 350, 0, WIDTH);

    GameState state = GameState::SINGLEPLAYER;

    TitleScreen tittle_screen(font, window, state);

    Player player(sf::Vector2f(400, 400), sf::Vector2f(50, 50), 0, WIDTH);

    // Enemy enemy1(sf::Vector2f(100, 100), 200, 1.0, enemy_flying_texture, 0, WIDTH);

    // for (int i = 0; i < 4; i++)
    // {
    //     enemy1.add_animation_frame(sf::IntRect(81 * i, 0, 71, 81));
    // }

    // ----------------------------------------------

    game.create_platforms(50, 100, 20, HEIGHT, WIDTH);
    game.create_enemy();

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

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    moveLeft = true;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    moveRight = true;
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    // if (state == GameState::TITLE)
                    // state = GameState::SINGLEPLAYER;

                    player.jump();
                }

                if (event.key.code == sf::Keyboard::Space)
                {
                    switch (state)
                    {
                    case GameState::TITLE:
                        std::cout << "TITLE" << std::endl;
                        break;

                    case GameState::SINGLEPLAYER:
                        std::cout << "SINGLEPLAYER" << std::endl;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    moveLeft = false;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    moveRight = false;
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
            {
                player.shoot(WeaponType::SINGLE);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
            {
                player.shoot(WeaponType::MACHINEGUN);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
            {
                player.shoot(WeaponType::TRIPLE);
            }
        }

        if (moveLeft)
        {
            player.move(-1.0f, 0.0f);
        }
        if (moveRight)
        {
            player.move(1.0f, 0.0f);
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

            game.update(dt, window, player);
            player.update(dt, window);

            player.draw(window);
            game.draw(window);

            score.setString("Score: " + std::to_string(static_cast<int>(std::round(game.get_score()))));
            window.draw(score);

            if (game.get_game_state())
                state = GameState::GAMEOVER;

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
