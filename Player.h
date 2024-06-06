#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Weapon.h"
#include "Enemy.h"

class Player : public sf::RectangleShape
{
private:
    float gravity = 500;
    float jump_force = 500;

    sf::Vector2f velocity;
    bool is_on_ground = false;

    float speed = 5;
    float air_speed = 10;

    float friction = 0.9;
    float air_friction = 0.9;

    int game_left_bound;
    int game_right_bound;

    Weapon weapon;

    int max_health = 100;

    int health_points = max_health;

    std::string name = "Player";

    bool is_invincible = false;
    float invincible_timer = 0;
    bool massacre_mode = false;
    float massacre_timer = 0;

    sf::Sprite shield;
    sf::Texture shield_texture;

    sf::Text health_text;

    sf::Font font;

public:
    Player(sf::Vector2f pos, sf::Vector2f s, int left_bound, int right_bound) : sf::RectangleShape(), weapon(pos, WeaponType::SINGLE, left_bound, right_bound)
    {
        setPosition(pos);
        setSize(s);

        game_left_bound = left_bound;
        game_right_bound = right_bound;

        if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
        {
            std::cout << "Failed to load font" << std::endl;
        }

        health_text = createText(std::to_string(health_points) + "/" + std::to_string(max_health), font, 40, sf::Color::White, pos);

        health_text.setOutlineColor(sf::Color::Black);
        health_text.setOutlineThickness(3);

        shield_texture.loadFromFile("assets/powerup/shield.png");
        shield.setTexture(shield_texture);
    }

    void update(float dt, sf::RenderWindow &window)
    {
        if (invincible_timer > 0)
        {
            invincible_timer -= dt;
            if (invincible_timer <= 0)
            {
                is_invincible = false;
            }
        }
        if (massacre_timer > 0)
        {
            massacre_timer -= dt;
            if (massacre_timer <= 0)
            {
                massacre_mode = false;
            }
        }

        if (is_on_ground)
        {
            velocity.y = 0;
            velocity.x *= friction;
        }
        else
        {
            velocity.y += gravity * dt;
            velocity.x *= air_friction;
        }

        sf::Vector2f pos = getPosition();
        pos.x += velocity.x * dt;
        pos.y += velocity.y * dt;
        setPosition(pos);
        if (is_invincible)
        {
            shield.setPosition(getPosition().x + getGlobalBounds().width - shield.getGlobalBounds().width, getPosition().y + getGlobalBounds().height - shield.getGlobalBounds().height);
        }

        weapon.update(dt, getPosition() + sf::Vector2f(getSize().x / 2, 0), window);

        health_text.setPosition(getPosition() + sf::Vector2f(getGlobalBounds().width / 2, -20));
        health_text.setString(std::to_string(health_points) + "/" + std::to_string(max_health));
    }

    void move(float dx, float dy)
    {
        sf::Vector2f pos = getPosition();
        pos.x += dx * speed;
        pos.y += dy;

        if (pos.x < game_left_bound)
        {
            pos.x = game_left_bound;
            std::cout << "Left bound" << std::endl;
        }
        if (pos.x + getGlobalBounds().width > game_right_bound)
        {
            pos.x = game_right_bound - getGlobalBounds().width;

            std::cout << "Right bound" << std::endl;
        }

        setPosition(pos);
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(*this);

        weapon.draw(window);

        window.draw(health_text);

        if (is_invincible)
        {
            window.draw(shield);
        }
    }

    void jump()
    {
        if (is_on_ground)
        {
            is_on_ground = false;
            velocity.y = -jump_force;
        }
    }

    sf::Vector2f get_velocity()
    {
        return velocity;
    }

    void set_ground(bool ground)
    {
        is_on_ground = ground;
    }

    void shoot(WeaponType type)
    {
        weapon.shoot(type, 0);
    }

    void check_projeciltile_collision(std::vector<Enemy> &enemies)
    {
        for (Enemy &enemy : enemies)
        {
            sf::FloatRect bounds = enemy.getGlobalBounds();

            int result = weapon.check_collision(bounds);

            if (result != -1)
            {
                // std::cout << "Collision" << std::endl;

                std::cout << "Collision with enemy at index: " << &enemy - &enemies[0] << std::endl;

                enemy.update_health(result);
            }
        }
    }

    void update_health(int damage)
    {
        if (!is_invincible && !massacre_mode)
            health_points -= damage;
        // std::cout << "Health: " << health_points << std::endl;
    }
    void restoreHealth(int health)
    {
        if (health_points + health > max_health)
        {
            health_points = max_health;
        }
        else
        {
            health_points += health;
        }
    }
    int get_health()
    {
        return health_points;
    }

    sf::Text createText(const std::string &text, const sf::Font &font, int size, const sf::Color &color, const sf::Vector2f &position)
    {
        sf::Text sfText(text, font, size);
        sfText.setFillColor(color);

        sf::FloatRect textRect = sfText.getLocalBounds();
        sfText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        sfText.setPosition(position);

        return sfText;
    }

    void set_invincible(int invincible)
    {
        is_invincible = true;
        invincible_timer = (float)invincible;
        shield.setPosition(getPosition().x + getGlobalBounds().width - shield.getGlobalBounds().width, getPosition().y + getGlobalBounds().height - shield.getGlobalBounds().height);
    }
    void boostJump(int boost)
    {
        velocity.y = boost;
        set_ground(false);
    }
    void set_massacre(int massacre)
    {
        massacre_mode = true;
        massacre_timer = (float)massacre;
    }
    bool get_massacre()
    {
        return massacre_mode;
    }

    void setName(std::string n)
    {
        name = n;

        std::cout << "Name set to: " << name << std::endl;
    }

    std::string getName()
    {
        return name;
    }
};