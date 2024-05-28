#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "Platform.h"
#include "Enemy.h"

enum class GameState
{
    TITLE,
    GAME,
    MULTIPLAYER,
    GAMEOVER

};

int main()
{
    srand(time(NULL));

    const int WIDTH = 800;
    const int HEIGHT = 800;
    const int FPS = 60;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!", sf::Style::Close | sf::Style::Titlebar);

    window.setFramerateLimit(FPS);

    //----------------------------------------------

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

    sf::Text title("Doodle Jump!", font, 100);
    sf::Text info("Press SPACE to start", font, 50);

    sf::FloatRect textRect = title.getLocalBounds();
    title.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    title.setPosition(sf::Vector2f(WIDTH / 2.0f, 75));

    textRect = info.getLocalBounds();
    info.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    info.setPosition(sf::Vector2f(WIDTH / 2.0f, 150));

    GameState state = GameState::TITLE;

    Enemy enemy1(sf::Vector2f(100, 100), 200, 1.0);

    enemy1.setTexture(enemy_flying_texture);

    for (int i = 0; i < 4; ++i)
    {
        enemy1.add_animation_frame(sf::IntRect(81 * i, 0, 71, 81));
    }

    std::vector<Platform> platforms;

    int offset = 50;
    int platform_width = 100;

    // kod do kamery
    //  sf::View view1(sf::FloatRect(200.f, 200.f, 300.f, 200.f));
    //  window.setView(view1);

    for (int i = 0; i < HEIGHT; i = i + offset)
    {
        int platform_y = HEIGHT - i;
        int platform_x = rand() % (WIDTH - platform_width);

        Platform platforma(sf::Vector2f(platform_x, platform_y), sf::Vector2f(100.0, 20.0));

        platforms.emplace_back(platforma);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                state = GameState::GAME;
            }
        }

        window.clear();

        window.draw(sf::Sprite(background_texture));

        if (state == GameState::TITLE)
        {
            window.draw(title);
            window.draw(info);
        }
        else if (state == GameState::GAME)
        {
            for (const Platform &i : platforms)
            {
                window.draw(i);
            }

            window.draw(enemy1);
            enemy1.step();
            enemy1.move(1.0 / FPS);
        }

        window.display();
    }

    return 0;
}
