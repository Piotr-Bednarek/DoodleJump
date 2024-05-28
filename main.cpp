#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "Platform.h"
#include "Enemy.h"

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

    if (!enemy_flying_texture.loadFromFile("assets/enemy/FLYING.png"))
    {
        return 1;
    }

    if (!background_texture.loadFromFile("assets/background.png"))
    {
        return 1;
    }

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
                window.close();
        }

        window.clear();
        window.draw(sf::Sprite(background_texture));

        for (const Platform &i : platforms)
        {
            window.draw(i);
        }

        window.draw(enemy1);
        enemy1.step();
        enemy1.move(1.0 / FPS);

        window.display();
    }

    return 0;
}
