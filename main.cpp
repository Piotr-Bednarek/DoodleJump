#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <memory>

#include "Platform.h"
#include "Enemy.h"
#include "Game.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Button.h"
#include "TitleScreen.h"
#include "InputField.h"

#include "HighScoreManager.h"
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

    bool moveLeft1 = false;
    bool moveRight1 = false;
    bool moveLeft2 = false;
    bool moveRight2 = false;

    int which_player_won = 0;

    std::string name = "Player";
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

    sf::Sprite background(background_texture);

    //

    // if (!projectile_texture.loadFromFile("assets/fireball_projectile.png"))
    // {
    //     return 1;
    // }

    sf::Font font;
    sf::Font font1;

    if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
    {
        return 1;
    }
    if (!font1.loadFromFile("assets/fonts/Jaro.ttf"))
    {
        return 1;
    }

    sf::Text score = createText("Your Score: 0", font, 50, sf::Color::Black, sf::Vector2f(100, 40));

    HighScoreManager highScoreManager("highscores.txt");

    // ----------------------------------------------

    GameState state = GameState::TITLE;

    TitleScreen *title_screen = new TitleScreen(font, window, state);

    std::unique_ptr<Game> game1 = nullptr;
    std::unique_ptr<Game> game2 = nullptr;
    std::unique_ptr<Player> player1 = nullptr;
    std::unique_ptr<Player> player2 = nullptr;
    // std::unique_ptr<Player> player1 = std::make_unique<Player>(sf::Vector2f(WIDTH / 2, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH);
    // std::unique_ptr<Player> player2 = std::make_unique<Player>(sf::Vector2f(WIDTH / 4 * 3, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH);

    std::unique_ptr<InputField> username_field = std::make_unique<InputField>(sf::Vector2f(400, 300), sf::Vector2f(250, 75), font1, player1.get());

    title_screen->updateHighScore(highScoreManager);

    sf::View view1(sf::FloatRect(0, 0, WIDTH / 2, HEIGHT));
    sf::View view2(sf::FloatRect(0, 0, WIDTH / 2, HEIGHT));

    view1.setViewport(sf::FloatRect(0, 0, 0.5f, 1));
    view2.setViewport(sf::FloatRect(0.5f, 0, 0.5f, 1));

    sf::RectangleShape blackBar(sf::Vector2f(5, window.getSize().y));
    blackBar.setFillColor(sf::Color::Black);
    blackBar.setPosition(window.getSize().x / 2 - blackBar.getSize().x / 2, 0);

    // ----------------------------------------------

    // int random_index = ;
    // sf::Sprite background = background_sprites[random_index];

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
                if (username_field->is_field_active() && sf::Event::TextEntered && state == GameState::TITLE)
                {
                    username_field->handle_event(event);
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    moveLeft1 = true;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    moveRight1 = true;
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    if (player1 != nullptr)
                    {
                        player1->jump();
                    }
                }
                if (event.key.code == sf::Keyboard::C)
                {
                    player1->shoot(WeaponType::SINGLE);
                }

                if (event.key.code == sf::Keyboard::V)
                {
                    player1->shoot(WeaponType::MACHINEGUN);
                }

                if (event.key.code == sf::Keyboard::B)
                {
                    player1->shoot(WeaponType::TRIPLE);
                }

                // ----------------------------------------------

                if (event.key.code == sf::Keyboard::Left)
                {
                    moveLeft2 = true;
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    moveRight2 = true;
                }
                if (event.key.code == sf::Keyboard::Up)
                {
                    if (player2 != nullptr)
                    {
                        player2->jump();
                    }
                }
                if (event.key.code == sf::Keyboard::Numpad1)
                {
                    player2->shoot(WeaponType::SINGLE);
                }

                if (event.key.code == sf::Keyboard::Numpad2)
                {
                    player2->shoot(WeaponType::MACHINEGUN);
                }

                if (event.key.code == sf::Keyboard::Numpad3)
                {
                    player2->shoot(WeaponType::TRIPLE);
                }

                // if (event.key.code == sf::Keyboard::Space)
                // {
                //     switch (state)
                //     {
                //     case GameState::TITLE:
                //         std::cout << "TITLE" << std::endl;
                //         break;

                //     case GameState::SINGLEPLAYER:
                //         std::cout << "SINGLEPLAYER" << std::endl;
                //         break;
                //     }
                // }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::A)
                {
                    moveLeft1 = false;
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    moveRight1 = false;
                }

                if (event.key.code == sf::Keyboard::Left)
                {
                    moveLeft2 = false;
                }
                if (event.key.code == sf::Keyboard::Right)
                {
                    moveRight2 = false;
                }
            }
        }

        if (moveLeft1 && (state == GameState::SINGLEPLAYER || state == GameState::MULTIPLAYER))
        {
            player1->move(-1.0f, 0.0f);
        }
        if (moveRight1 && (state == GameState::SINGLEPLAYER || state == GameState::MULTIPLAYER))
        {
            player1->move(1.0f, 0.0f);
        }

        if (moveLeft2 && state == GameState::MULTIPLAYER)
        {
            player2->move(-1.0f, 0.0f);
        }
        if (moveRight2 && state == GameState::MULTIPLAYER)
        {
            player2->move(1.0f, 0.0f);
        }

        window.clear();

        switch (state)
        {
        case GameState::TITLE:
            window.draw(background);

            title_screen->update(window);
            title_screen->draw(window);

            username_field->draw(window);
            username_field->update(window);

            break;

        case GameState::SINGLEPLAYER:
            window.draw(background);

            if (player1 == nullptr)
            {
                player1 = std::make_unique<Player>(sf::Vector2f(WIDTH / 2, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH);
                player1->setName(name);
                username_field->updatePointer(player1.get());
            }
            if (game1 == nullptr)
            {
                game1 = std::make_unique<Game>(0, 350, 0, WIDTH);
                game1->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                score.setPosition(sf::Vector2f(100, 40));
            }

            game1->check_collision(*player1);
            game1->update(dt, window, *player1);
            player1->update(dt, window);

            game1->draw(window);
            player1->draw(window);

            score.setString("Your Score: " + std::to_string(static_cast<int>(game1->get_score())));
            window.draw(score);

            if (game1->get_game_state())
            {
                // std::cout << game.get_game_state() << std::endl;
                state = GameState::GAMEOVER;
                highScoreManager.addHighScore(HighScore(player1->getName(), game1->get_score()));
            }

            break;

        case GameState::MULTIPLAYER:
            if (player1 == nullptr)
            {
                player1 = std::make_unique<Player>(sf::Vector2f(WIDTH / 4, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH / 2);
            }
            if (game1 == nullptr)
            {
                game1 = std::make_unique<Game>(0, 350, 0, WIDTH / 2);
                game1->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                // score.setPosition(sf::Vector2f(150, 40));
            }

            window.setView(view1);
            window.draw(background);

            game1->check_collision(*player1);
            game1->update(dt, window, *player1);
            player1->update(dt, window);

            game1->draw(window);
            player1->draw(window);

            // ----------------------------------------------

            if (player2 == nullptr)
            {
                player2 = std::make_unique<Player>(sf::Vector2f(WIDTH / 4, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH / 2);
            }
            if (game2 == nullptr)
            {
                game2 = std::make_unique<Game>(0, 350, 0, WIDTH / 2);
                game2->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                // score.setPosition(sf::Vector2f(150, 40));
            }

            window.setView(view2);
            window.draw(background);

            game2->check_collision(*player2);
            game2->update(dt, window, *player2);
            player2->update(dt, window);

            game2->draw(window);
            player2->draw(window);

            window.setView(window.getDefaultView());
            window.draw(blackBar);

            if (game1->get_game_state())
            {
                // std::cout << game.get_game_state() << std::endl;
                std::cout << "PLAYER 2 WON" << std::endl;
                which_player_won = 2;
                state = GameState::GAMEOVER;
            }

            if (game2->get_game_state())
            {
                // std::cout << game.get_game_state() << std::endl;
                std::cout << "PLAYER 1 WON" << std::endl;
                which_player_won = 1;
                state = GameState::GAMEOVER;
            }

            break;
        case GameState::GAMEOVER:

            window.setView(window.getDefaultView());
            window.draw(background);

            if (which_player_won != 0)
            {

                title_screen->drawWinner(window, which_player_won);

                if (game1 != nullptr)
                {
                    game1.reset();
                    player1.reset();
                }

                if (game2 != nullptr)
                {
                    game2.reset();
                    player1.reset();
                }
            }
            else
            {
                window.draw(score);
                score.setPosition(WIDTH / 2.0f, 150);
                title_screen->drawGameOver(window, highScoreManager);
                if (game1 != nullptr)
                {
                    game1.reset();
                    name = player1->getName();
                    player1.reset();
                }
            }

            break;
        }

        window.display();
    }

    return 0;
}
