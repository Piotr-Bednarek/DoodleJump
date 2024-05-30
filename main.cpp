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
#include "TittleScreen.h"

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

    sf::Texture projectile_texture;

    sf::Font font;

    if (!enemy_flying_texture.loadFromFile("assets/enemy/FLYING.png"))
    {
        return 1;
    }

    if (!background_texture.loadFromFile("assets/background.png"))
    {
        return 1;
    }

    if (!platform_grass_texture.loadFromFile("assets/platform_grass.png"))
    {
        return 1;
    }

    if (!platform_stone_texture.loadFromFile("assets/platform_stone.png"))
    {
        return 1;
    }

    if (!projectile_texture.loadFromFile("assets/projectile.png"))
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

    // std::vector<sf::Texture> platform_textures = {platform_grass_texture, platform_stone_texture};

    // Platform platform1(sf::Vector2f(100, 100), sf::Vector2f(100, 20));
    // Platform platform2(sf::Vector2f(200, 200), sf::Vector2f(100, 20));

    // ----------------------------------------------

    Game game(0, 350, 0, WIDTH, projectile_texture);

    GameState state = GameState::TITLE;

    TittleScreen tittle_screen(font);

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
            window.draw(title);
            window.draw(info);

            tittle_screen.update(window);
            tittle_screen.draw(window);

            break;
        case GameState::SINGLEPLAYER:
            game.draw(window);
            game.update(dt, window);

            // window.draw(platform1);
            // window.draw(platform2);

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
