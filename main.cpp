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

    bool moveLeft = false;
    bool moveRight = false;
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

    std::unique_ptr<Game> game = nullptr;

    GameState state = GameState::TITLE;

    TitleScreen* title_screen = new TitleScreen(font, window, state);

    std::unique_ptr<Player> player = std::make_unique <Player>(sf::Vector2f(WIDTH / 2, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH);

    std::unique_ptr <InputField> username_field = std::make_unique<InputField>(sf::Vector2f(400, 300), sf::Vector2f(250, 75), font1, player.get());

    title_screen->updateHighScore(highScoreManager);

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
                if (username_field->is_field_active() && sf::Event::TextEntered && state == GameState::TITLE)
                {
                    username_field->handle_event(event);
                }

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
                    if(player!=nullptr){
                        player->jump();
                    }
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
                player->shoot(WeaponType::SINGLE);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
            {
                player->shoot(WeaponType::MACHINEGUN);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
            {
                player->shoot(WeaponType::TRIPLE);
            }
        }

        if (moveLeft && state==GameState::SINGLEPLAYER)
        {
            player->move(-1.0f, 0.0f);
        }
        if (moveRight && state==GameState::SINGLEPLAYER)
        {
            player->move(1.0f, 0.0f);
        }

        window.clear();

        window.draw(sf::Sprite(background_texture));

        switch (state)
        {
        case GameState::TITLE:
            if(game == nullptr){
                game  = std::make_unique <Game> (0, 350, 0, WIDTH);
                game->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                game->create_enemy();
                score.setPosition(sf::Vector2f(100, 40));
            }
            if(player == nullptr){
                player = std::make_unique<Player>(sf::Vector2f(WIDTH / 2, HEIGHT - 150), sf::Vector2f(50, 50), 0, WIDTH);
                player->setName(name);
                username_field->updatePointer(player.get());
            }
            

            title_screen->update(window);
            title_screen->draw(window);

            username_field->draw(window);

            username_field->update(window);

            break;

        case GameState::SINGLEPLAYER:

            game->check_collision(*player);

            game->update(dt, window, *player);
            player->update(dt, window);
            
            game->draw(window);
            player->draw(window);

            score.setString("Your Score: " + std::to_string(static_cast<int>(game->get_score())));
            window.draw(score);

            if (game->get_game_state())
            {
                // std::cout << game.get_game_state() << std::endl;
                state = GameState::GAMEOVER;
                highScoreManager.addHighScore(HighScore(player->getName(), game->get_score()));
            }

            break;

        case GameState::MULTIPLAYER:
            break;
        case GameState::GAMEOVER:
            window.draw(score);
            score.setPosition(WIDTH / 2.0f, 150);
            title_screen->drawGameOver(window, highScoreManager);
            if(game != nullptr){
                game.reset();
                name = player->getName();
                player.reset();
            }

            break;
        }

        window.display();
    }

    return 0;
}
