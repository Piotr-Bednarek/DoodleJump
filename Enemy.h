#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "AnimatedSprite.h"
#include "Projectile.h"
#include "Weapon.h"

class Enemy : public AnimatedSprite
{
private:
    float speed;
    int direction;

    int max_health = 30;

    int health_points = max_health;

    sf::Texture enemy_texture;

    Weapon weapon;

    int game_left_bound;
    int game_right_bound;

    float scaleX;
    float scaleY;

    sf::Vector2f player_position;

    // sf::Text health_text;
    // sf::Font font;

public:
    Enemy(sf::Vector2f pos, float s, int dir, sf::Texture &texture, int left_bound, int right_bound, float sX, float sY) : AnimatedSprite(pos, 10), weapon(pos, WeaponType::SINGLE, left_bound, right_bound)
    {
        // if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
        // {
        //     std::cout << "Failed to load font" << std::endl;
        // }

        // health_text = createText(std::to_string(health_points) + "/" + std::to_string(max_health), font, 40, sf::Color::White, pos);

        // health_text.setOutlineColor(sf::Color::Black);
        // health_text.setOutlineThickness(3);

        speed = s;
        direction = dir;

        scaleX = sX;
        scaleY = sY;

        if (direction == 1)
        {
            setScale(-scaleX, scaleY);
        }
        else if (direction == -1)
        {
            setScale(scaleX, scaleY);
        }

        enemy_texture = texture;

        game_left_bound = left_bound;
        game_right_bound = right_bound;

        setTexture(texture);
        step();

        // try
        // {
        //     std::string healthStr = std::to_string(health_points);
        //     std::string maxHealthStr = std::to_string(max_health);
        //     health_text = createText(healthStr + "/" + maxHealthStr, font, 20, sf::Color::Black, sf::Vector2f(0, 0));
        // }
        // catch (const std::exception &e)
        // {
        //     std::cout << "Exception caught: " << e.what() << std::endl;
        // }
    }

    sf::Sprite &getSprite()
    {
        return *this;
    }

    void update(float dt, sf::RenderWindow &window, sf::Vector2f player_pos)
    {
        sf::Vector2f weaponPos = getPosition();
        weaponPos.x += getGlobalBounds().width / 2.0f;
        weaponPos.y += getGlobalBounds().height / 2.0f;
        weapon.update(dt, weaponPos, window);

        int width = getGlobalBounds().width;
        int height = getGlobalBounds().height;

        step();
        draw(window);

        weapon.update(dt, getPosition() + sf::Vector2f(0, height / 2), window);

        player_position = player_pos;

        // health_text.setPosition(getPosition() + sf::Vector2f(getGlobalBounds().width / 2, -20));
        // health_text.setString(std::to_string(health_points) + "/" + std::to_string(max_health));
    }

    void move(float dt, float dy)
    {
        sf::Vector2f pos = getPosition();

        pos.x += speed * direction * dt;
        pos.y += dy;
        setPosition(pos);

        if (pos.x < 0 || pos.x > 800)
        {
            direction *= -1;
            bounce(weapon);
        }

        if (rand() % 100 < 5)
        {
            shoot(WeaponType::SINGLE);
        }

        weapon.move(0, dy);
    }

    void bounce(Weapon &weapon)
    {
        float width = getGlobalBounds().width;

        sf::Vector2f position = getPosition();

        if (direction == 1)
        {
            setScale(-scaleX, scaleY);
            setPosition(position.x + width, position.y);

            weapon.move(-width);

            // std::cout << "Bounce right" << std::endl;
        }
        else if (direction == -1)
        {
            setScale(scaleX, scaleY);
            setPosition(position.x - width, position.y);

            weapon.move(width);

            // std::cout << "Bounce left" << std::endl;
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);
        weapon.draw(window);
        sf::RectangleShape hitbox;
        hitbox.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
        hitbox.setPosition(getPosition().x - getGlobalBounds().width / 2, getPosition().y - getGlobalBounds().height / 2);
        hitbox.setOutlineColor(sf::Color::Red);
        hitbox.setOutlineThickness(1.0f);
        hitbox.setFillColor(sf::Color::Transparent);
        window.draw(hitbox);
    }

    void shoot(WeaponType type)
    {
        const double PI = 3.14159265358979323846;
        int threshold = 90;

        sf::Vector2f enemy_pos = getPosition();

        float dx = player_position.x - enemy_pos.x;
        float dy = player_position.y - enemy_pos.y;
        float angle = atan2(dy, dx) * 180 / PI + 90;

        if (std::abs(angle) >= threshold)
        {
            std::cout << "Angle: " << angle << std::endl;
            weapon.shoot(type, angle);
        }
    }

    void update_health(int damage)
    {
        health_points -= damage;
    }

    int get_health()
    {
        return health_points;
    }

    int check_projeciltile_collision(sf::FloatRect bounds)
    {
        int result = weapon.check_collision(bounds);

        return result;
    }

    // sf::Text createText(const std::string &text, const sf::Font &font, int size, const sf::Color &color, const sf::Vector2f &position)
    // {
    //     sf::Text sfText(text, font, size);
    //     sfText.setFillColor(color);

    //     sf::FloatRect textRect = sfText.getLocalBounds();
    //     sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    //     sfText.setPosition(position);

    //     return sfText;
    // }
};
