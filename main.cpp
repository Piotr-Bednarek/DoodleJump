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

    int WIDTH = 1600;
    int HEIGHT = 800;
    int FPS = 60;

    std::vector<std::pair<bool,bool>> move = {{false, false}, {false, false}};
    std::vector<std::vector<sf::Keyboard::Key>> keys = {{sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::Q, sf::Keyboard::E, sf::Keyboard::R}, {sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::M, sf::Keyboard::Comma, sf::Keyboard::Period}};

    std::vector<std::string> name = {"Player", "Player"};
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
    background_texture.setRepeated(true);
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

    std::vector<sf::Text> score;
    score.emplace_back(createText("Your Score: 0", font, 50, sf::Color::Black, sf::Vector2f(130, 40)));
    std::unique_ptr<HighScoreManager> highScoreManager = std::make_unique<HighScoreManager>("highscores.txt");
    std::unique_ptr<Game> game = nullptr;
    std::vector<std::unique_ptr<Game>> games;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<InputField>> inputFields;
    std::vector<std::unique_ptr<TitleScreen>> titleScreens;
    std::vector<GameState> gameStates;
    games.emplace_back(nullptr);
    players.emplace_back(std::make_unique<Player>(sf::Vector2f((WIDTH-50) / 2, HEIGHT - 100), sf::Vector2f(50, 50), 0, WIDTH));
    inputFields.emplace_back(std::make_unique<InputField>(sf::Vector2f(WIDTH/2, 300), sf::Vector2f(250, 75), font1, players[0].get()));
    gameStates.emplace_back(GameState::TITLE);
    titleScreens.emplace_back(std::make_unique<TitleScreen>(font, 0, WIDTH, HEIGHT, gameStates[0]));
    titleScreens[0]->updateHighScore(*highScoreManager);
    
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
                for(int i = 0; i < inputFields.size(); i++){
                    if(inputFields[i]->is_field_active() && sf::Event::TextEntered && gameStates[i] == GameState::TITLE){
                        inputFields[i]->handle_event(event);
                    }
                }

                for(int i = 0; i < players.size(); i++){
                    if(players[i]!=nullptr){
                        if (event.key.code == keys[i][0])
                        {
                            move[i].first = true;
                        }
                        if (event.key.code == keys[i][1])
                        {
                            move[i].second = true;
                        }
                        if (event.key.code == keys[i][2])
                        {
                            if(players[i]!=nullptr){
                                players[i]->jump();
                            }
                        }
                        
                        if (event.key.code == keys[i][3])
                        {
                            players[i]->shoot(WeaponType::SINGLE);
                        }

                        if (event.key.code == keys[i][4])
                        {
                            players[i]->shoot(WeaponType::MACHINEGUN);
                        }

                        if (event.key.code == keys[i][5])
                        {
                            players[i]->shoot(WeaponType::TRIPLE);
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                for(int i = 0; i < players.size(); i++){
                    if(players[i]!=nullptr){
                        if (event.key.code == keys[i][0])
                        {
                            move[i].first = false;
                        }
                        if (event.key.code == keys[i][1])
                        {
                            move[i].second = false;
                        }
                    }
                }
            }
        }
        for(int i = 0; i < players.size(); i++){
            if( players[i]!=nullptr && gameStates[i] == GameState::SINGLEPLAYER){
                if (move[i].first)
                {
                    players[i]->move(-1.0f, 0.0f);
                }
                if (move[i].second)
                {
                    players[i]->move(1.0f, 0.0f);
                }
            }
        }


        window.clear();

        window.draw(sf::Sprite(background_texture, sf::IntRect(0, 0, WIDTH, HEIGHT)));
        for(int i = 0; i < gameStates.size(); i++){
            switch (gameStates[i])
            {
            case GameState::TITLE:
                if(games[i] == nullptr && gameStates.size()>1){
                    games[i] = std::make_unique <Game> (0, 350, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2));
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH/2 + i*(WIDTH/2));
                    games[i]->create_enemy();
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-games[i]->getLeftBound()-50) / 2 + games[i]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), 0 + i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2));
                    players[i]->setName(name[i]);
                    inputFields[i]->updatePointer(players[i].get());
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2), HEIGHT, gameStates[i]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }
                else if (games[i] == nullptr){
                    std::cout << 'a' << std::endl;
                    games[i] = std::make_unique <Game> (0, 350, 0, WIDTH);
                    std::cout << 'b' << std::endl;
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                    std::cout << 'c' << std::endl;
                    games[i]->create_enemy();
                    std::cout << 'd' << std::endl;
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                    std::cout << 'e' << std::endl;
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-games[i]->getLeftBound()-50) / 2 + games[i]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), 0 , WIDTH);
                    std::cout << 'f' << std::endl;
                    players[i]->setName(name[i]);
                    std::cout << 'g' << std::endl;
                    inputFields[i]->updatePointer(players[i].get());
                    std::cout << 'h' << std::endl;
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0, WIDTH, HEIGHT, gameStates[i]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }

                titleScreens[i]->update(window);
                std::cout << 'i' << std::endl;
                titleScreens[i]->draw(window);
                std::cout << 'j' << std::endl;
                inputFields[i]->update(window);
                std::cout << 'k' << std::endl;
                inputFields[i]->draw(window);
                std::cout << "l " << (int)gameStates[i] << std::endl;
                if(gameStates[i] == GameState::SINGLEPLAYER){
                    std::cout <<"wtf" <<std::endl;
                }
                break;

            case GameState::SINGLEPLAYER:
                std::cout << 1 << std::endl;
                games[i]->check_collision(*players[i]);
                std::cout << 2 << std::endl;
                games[i]->update(dt, window, *players[i]);
                std::cout << 3 << std::endl;
                players[i]->update(dt, window);
                std::cout << 4 << std::endl;
                games[i]->draw(window);
                std::cout << 5 << std::endl;
                players[i]->draw(window);
                std::cout << 6 << std::endl;

                score[i].setString("Your Score: " + std::to_string(static_cast<int>(games[i]->get_score())));
                std::cout << 7 << std::endl;
                window.draw(score[i]);
                std::cout << 8 << std::endl;

                if (games[i]->get_game_state())
                {
                    std::cout << 9 << std::endl;
                    gameStates[i] = GameState::GAMEOVER;
                    highScoreManager->loadHighScores();
                    highScoreManager->addHighScore(HighScore(players[i]->getName(), games[i]->get_score()));
                    highScoreManager->saveHighScores();
                    std::cout << 10 << std::endl;
                }
                std::cout << 11 << std::endl;
                break;

            case GameState::MULTIPLAYER:
                if(gameStates.size() < 2){
                    games.emplace_back(std::make_unique <Game> (0, 350, WIDTH/2, WIDTH));
                    players.emplace_back(std::make_unique<Player>(sf::Vector2f((WIDTH-50) / 2, HEIGHT - 100), sf::Vector2f(50, 50), 0, WIDTH));
                    gameStates.emplace_back(GameState::TITLE);
                    score.emplace_back(createText("Your Score: 0", font, 50, sf::Color::Black, sf::Vector2f(games[i+1]->getLeftBound()+130, 40)));
                    inputFields.emplace_back(std::make_unique<InputField>(sf::Vector2f(WIDTH/2 + (games[i+1]->getRightBound()-games[i+1]->getLeftBound())/2 , 300), sf::Vector2f(250, 75), font1, players[i+1].get()));
                    titleScreens.emplace_back(std::make_unique<TitleScreen>(font, WIDTH/2, WIDTH, HEIGHT, gameStates[i+1]));
                    titleScreens[i+1]->updateHighScore(*highScoreManager);
                    games[i+1] = std::make_unique <Game> (0, 350, WIDTH/2, WIDTH);
                    games[i+1]->create_platforms(50, 78, 35, HEIGHT, WIDTH/2);
                    games[i+1]->create_enemy();
                    players[i+1] = std::make_unique<Player>(sf::Vector2f((WIDTH/2-50) / 2 + games[i+1]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), WIDTH/2, WIDTH);
                    players[i+1]->setName(name[i+1]);
                    inputFields[i+1]->updatePointer(players[i+1].get());

                    
                    games[i].reset();
                    name[i] = players[i]->getName();
                    players[i].reset();
                    games[i] = std::make_unique <Game> (0, 350, 0, WIDTH/2);
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH/2);
                    games[i]->create_enemy();
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-50) / 2, HEIGHT - 100), sf::Vector2f(50, 50), 0, WIDTH/2);
                    players[i]->setName(name[i]);
                    inputFields[i].reset();
                    inputFields[i] = std::make_unique<InputField>(sf::Vector2f(WIDTH/4 , 300), sf::Vector2f(250, 75), font1, players[i].get());
                    gameStates[i] = GameState::TITLE;
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0, WIDTH/2, HEIGHT, gameStates[i]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }
                if(games[i] == nullptr){
                    games[i] = std::make_unique <Game> (0, 350, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2));
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                    games[i]->create_enemy();
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-games[i]->getLeftBound()-50) / 2 + games[i]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), 0 + i*WIDTH/2, WIDTH);
                    players[i]->setName(name[i]);
                    inputFields[i]->updatePointer(players[i].get());
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2), HEIGHT, gameStates[i]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }
                break;
            case GameState::GAMEOVER:
                window.draw(score[i]);
                titleScreens[i]->drawGameOver(window, *highScoreManager);
                if(games[i] != nullptr){
                    score[i].setPosition((games[i]->getRightBound() - games[i]->getLeftBound())/ 2.0f + games[i]->getLeftBound() , 150);
                    games[i].reset();
                    name[i] = players[i]->getName();
                    players[i].reset();
                }
                break;
            }
        }
        window.display();
    }

    return 0;
}
