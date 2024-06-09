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
    int HEIGHT = 793;
    int FPS = 60;

    std::vector<std::pair<bool,bool>> move = {{false, false}, {false, false}};
    std::vector<std::vector<sf::Keyboard::Key>> keys = {{sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::C, sf::Keyboard::V, sf::Keyboard::B}, {sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Numpad1, sf::Keyboard::Numpad2, sf::Keyboard::Numpad3}};

    std::vector<std::string> name = {"Player1", "Player2"};
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
    sf::Sprite background(background_texture, sf::IntRect(0, 0, 1600, HEIGHT));
   
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

    sf::RectangleShape blackBar(sf::Vector2f(2, window.getSize().y));
    blackBar.setFillColor(sf::Color::Black);
    blackBar.setPosition(window.getSize().x + 2, 0);
    std::vector<sf::View> view{sf::View(sf::FloatRect(0, 0, 800, HEIGHT)), sf::View(sf::FloatRect(800, 0, 1600, HEIGHT))};
    view[0].setViewport(sf::FloatRect(0, 0, 0.5, 1));
    view[1].setViewport(sf::FloatRect(0.5, 0, 1, 1));


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
    titleScreens.emplace_back(std::make_unique<TitleScreen>(font, 0, WIDTH, HEIGHT, gameStates[0], sf::Keyboard::W));
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
                if(event.key.code == sf::Keyboard::W && gameStates[0] == GameState::TITLE){
                    gameStates[0] = GameState::GAME;
                }
                if(event.key.code == sf::Keyboard::Up && gameStates.size()>1 && gameStates[1] == GameState::TITLE){
                    gameStates[1] = GameState::GAME;
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
        
        for(int i = 0; i < players.size(); i++){
            if( players[i]!=nullptr && gameStates[i] == GameState::GAME){
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
        window.draw(background);

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
                    inputFields[i] = std::make_unique<InputField>(sf::Vector2f(games[i]->getLeftBound() + (games[i]->getRightBound()-games[i]->getLeftBound())/2 , 300), sf::Vector2f(250, 75), font1, players[i].get());
                    inputFields[i]->setText(name[i]);
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2), HEIGHT, gameStates[i], keys[i][2]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }
                else if (games[i] == nullptr){
                    games[i] = std::make_unique <Game> (0, 350, 0, WIDTH);
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH);
                    games[i]->create_enemy();
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-games[i]->getLeftBound()-50) / 2 + games[i]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), 0 , WIDTH);
                    players[i]->setName(name[i]);
                    inputFields[i] = std::make_unique<InputField>(sf::Vector2f(games[i]->getLeftBound() + (games[i]->getRightBound()-games[i]->getLeftBound())/2 , 300), sf::Vector2f(250, 75), font1, players[i].get());
                    inputFields[i]->setText(name[i]);
                    titleScreens[i].reset();
                    titleScreens[i] = std::make_unique<TitleScreen>(font, 0, WIDTH, HEIGHT, gameStates[i], keys[i][2]);
                    titleScreens[i]->updateHighScore(*highScoreManager);
                }
                titleScreens[i]->update(window);
                titleScreens[i]->draw(window);
                inputFields[i]->update(window);
                inputFields[i]->draw(window);
                break;

            case GameState::GAME:
                if(gameStates.size()>1){
                    window.setView(view[i]);
                }
                window.draw(background);
                games[i]->check_collision(*players[i]);
                games[i]->update(dt, window, *players[i]);
                players[i]->update(dt, window);
                games[i]->draw(window);
                players[i]->draw(window);

                score[i].setString("Your Score: " + std::to_string(static_cast<int>(games[i]->get_score())));
                window.draw(score[i]);

                if (games[i]->get_game_state())
                {
                    gameStates[i] = GameState::GAMEOVER;
                    highScoreManager->loadHighScores();
                    highScoreManager->addHighScore(HighScore(players[i]->getName(), games[i]->get_score()));
                    highScoreManager->saveHighScores();
                }
                window.setView(window.getDefaultView());
                break;
            case GameState::SINGLEPLAYER:
                if(gameStates.size()>1){
                    WIDTH = 800;
                    gameStates.pop_back();
                    games.pop_back();
                    players.pop_back();
                    inputFields.pop_back();
                    window.create(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);
                    window.setFramerateLimit(FPS);
                    window.setVerticalSyncEnabled(true);
                    blackBar.setPosition(window.getSize().x + 10 , 0);
                    titleScreens.pop_back();
                }
                if(games[i] != nullptr){
                    titleScreens[i]->draw(window);
                    inputFields[i]->draw(window);
                    games[i].reset();
                    name[i] = players[i]->getName();
                    players[i].reset();
                    inputFields[i].reset();
                    gameStates[i] = GameState::TITLE;
                    titleScreens[i].reset();
                }
                break;
            case GameState::MULTIPLAYER:
                if(i == 0 && gameStates.size() == 1){
                    WIDTH = 1600;
                    games[i].reset();
                    name[i] = players[i]->getName();
                    players[i].reset();
                    titleScreens[i].reset();
                    inputFields[i].reset();
                    games.emplace_back(nullptr);
                    players.emplace_back(nullptr);
                    titleScreens.emplace_back(nullptr);
                    inputFields.emplace_back(nullptr);
                    gameStates.emplace_back(GameState::MULTIPLAYER);
                }
                if(games[i] == nullptr){
                    games[i] = std::make_unique <Game> (0, 350, 0+i*(WIDTH/2), WIDTH/2 + i*(WIDTH/2));
                    players[i] = std::make_unique<Player>(sf::Vector2f((games[i]->getRightBound()-games[i]->getLeftBound()-50) / 2 + games[i]->getLeftBound(), HEIGHT - 100), sf::Vector2f(50, 50), 0 + i*WIDTH/2, WIDTH/2 + i*(WIDTH/2));
                    titleScreens[i] = std::make_unique<TitleScreen>(font, games[i]->getLeftBound(), games[i]->getRightBound(), HEIGHT, gameStates[i], keys[i][2]);
                    inputFields[i] = std::make_unique<InputField>(sf::Vector2f(games[i]->getLeftBound() + (games[i]->getRightBound()-games[i]->getLeftBound())/2 , 300), sf::Vector2f(250, 75), font1, players[i].get());
                    inputFields[i]->setText(name[i]);
                    games[i]->create_platforms(50, 78, 35, HEIGHT, WIDTH/2);
                    games[i]->create_enemy();
                    players[i]->setName(name[i]);
                    titleScreens[i]->updateHighScore(*highScoreManager);                    
                    if(i == 1){
                        score.emplace_back(createText("Your Score: 0", font, 50, sf::Color::Black, sf::Vector2f(games[i]->getLeftBound()+130, 40)));
                        window.create(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);
                        window.setFramerateLimit(FPS);
                        window.setVerticalSyncEnabled(true);
                        blackBar.setPosition(window.getSize().x / 2 - blackBar.getSize().x / 2, 0);
                    }
                    score[i].setPosition(sf::Vector2f(130 + games[i]->getLeftBound(), 40));
                }
                titleScreens[i]->draw(window);
                inputFields[i]->draw(window);
                gameStates[i] = GameState::TITLE;
                break;
            case GameState::GAMEOVER:
                if(gameStates.size()>1){
                    window.setView(view[i]);
                }
                window.draw(background);
                window.draw(score[i]);
                titleScreens[i]->drawGameOver(window, *highScoreManager);
                if(games[i] != nullptr){
                    score[i].setPosition((games[i]->getRightBound() - games[i]->getLeftBound())/ 2.0f + games[i]->getLeftBound() , 150);
                    games[i].reset();
                    name[i] = players[i]->getName();
                    players[i].reset();
                }
                window.setView(window.getDefaultView());
                break;
            }
        }
        window.draw(blackBar);
        window.display();
    }
    return 0;
}
