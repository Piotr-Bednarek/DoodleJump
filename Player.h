#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "Weapon.h"
#include "Enemy.h"

enum class PlayerState
{
    IDLE,
    JUMP,
    RUN
};

class Player : public sf::Sprite
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

    std::vector<sf::Texture> player_idle_textures;
    std::vector<sf::Texture> player_jump_textures;
    std::vector<sf::Texture> player_run_textures;

    PlayerState state = PlayerState::IDLE;

    int current_frame = 0;
    float frame_duration = 0.1f;
    float frame_timer = 0.0f;

    float scaleX = 2;
    float scaleY = 2;

public:
    Player(sf::Vector2f pos, sf::Vector2f s, int left_bound, int &right_bound) : sf::Sprite(), weapon(pos, WeaponType::SINGLE, left_bound, right_bound, ProjectileType::SHURIKEN), game_left_bound(left_bound), game_right_bound(right_bound)
    {
        if (!font.loadFromFile("assets/fonts/Jacquard12.ttf"))
        {
            std::cout << "Failed to load font" << std::endl;
        }

        for (int i = 0; i < 4; i++)
        {
            sf::Texture texture;
            texture.loadFromFile("assets/player/idle/adventurer-idle-0" + std::to_string(i) + ".png", sf::IntRect(10, 5, 25, 32));
            player_idle_textures.push_back(texture);
        }

        for (int i = 0; i < 4; i++)
        {
            sf::Texture texture;
            texture.loadFromFile("assets/player/jump/adventurer-jump-0" + std::to_string(i) + ".png", sf::IntRect(10, 5, 25, 32));
            player_jump_textures.push_back(texture);
        }
        for (int i = 0; i < 6; i++)
        {
            sf::Texture texture;
            texture.loadFromFile("assets/player/run/adventurer-run-0" + std::to_string(i) + ".png", sf::IntRect(10, 5, 25, 32));
            player_run_textures.push_back(texture);
        }

        setPosition(pos);
        setScale(scaleX, scaleY);

        health_text = createText(std::to_string(health_points) + "/" + std::to_string(max_health), font, 40, sf::Color::White, pos);

        health_text.setOutlineColor(sf::Color::Black);
        health_text.setOutlineThickness(3);

        shield_texture.loadFromFile("assets/powerup/shield.png");
        shield.setTexture(shield_texture);
    }

    void update(float &dt, sf::RenderWindow &window)
    {
        check_state();

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

        if (velocity.x != 0)
        {
            if (velocity.x < 1 && velocity.x > -1)
            {
                velocity.x = 0;
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
            shield.setPosition(getPosition().x - shield.getGlobalBounds().width, getPosition().y + getGlobalBounds().height - shield.getGlobalBounds().height);
        }

        weapon.update(dt, getPosition() + sf::Vector2f(getGlobalBounds().width / 2, 0), window);

        health_text.setPosition(getPosition() + sf::Vector2f(getGlobalBounds().width / 2, -20));
        health_text.setString(std::to_string(health_points) + "/" + std::to_string(max_health));

        animate(dt);
    }

    void check_state()
    {
        // std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
        if (velocity.y < 0)
        {
            setPlayerState(PlayerState::JUMP);
        }
        else if (velocity.x != 0)
        {
            setPlayerState(PlayerState::RUN);
        }
        else
        {
            setPlayerState(PlayerState::IDLE);
        }
    }

    void animate(float &dt)
    {
        frame_timer += dt;

        if (frame_timer >= frame_duration)
        {
            frame_timer = 0.0f;
            current_frame++;

            switch (state)
            {
            case PlayerState::IDLE:
                if (current_frame >= player_idle_textures.size())
                {
                    current_frame = 0;
                }
                setTexture(player_idle_textures[current_frame]);
                break;

            case PlayerState::JUMP:
                if (current_frame >= player_jump_textures.size())
                {
                    current_frame = player_jump_textures.size() - 2;
                }
                else if (current_frame == 3)
                {
                    current_frame = 2;
                }
                else if (current_frame == 2)
                {
                    current_frame = 3;
                }
                setTexture(player_jump_textures[current_frame]);
                break;

            case PlayerState::RUN:
                if (current_frame >= player_run_textures.size())
                {
                    current_frame = 0;
                }
                setTexture(player_run_textures[current_frame]);
                break;
            }
        }
    }

    void setPlayerState(PlayerState newState)
    {
        if (state == newState)
        {
            return;
        }

        state = newState;

        current_frame = 0;
        frame_timer = 0.0f;

        switch (state)
        {
        case PlayerState::IDLE:
            setTexture(player_idle_textures[current_frame]);
            // std::cout << "IDLE" << std::endl;
            break;

        case PlayerState::JUMP:
            setTexture(player_jump_textures[current_frame]);

            // std::cout << "JUMP" << std::endl;
            break;

        case PlayerState::RUN:
            setTexture(player_run_textures[current_frame]);
            // std::cout << "RUN" << std::endl;
            break;
        }
    }

    void move(float dx, float dy)
    {
        sf::Vector2f pos = getPosition();
        velocity.x = dx * speed;
        pos.x += velocity.x;
        pos.y += dy;

        if (velocity.x < 0)
        {
            setOrigin(this->getTexture()->getSize().x / 2 + getGlobalBounds().width / 4, 0);
            setScale(-scaleX, scaleY);
        }
        else if (velocity.x > 0)
        {
            setOrigin(this->getTexture()->getSize().x / 2 - getGlobalBounds().width / 4, 0);
            setScale(scaleX, scaleY);
        }

        if (pos.x < game_left_bound)
        {
            pos.x = game_left_bound;
        }
        if (pos.x + getGlobalBounds().width > game_right_bound)
        {
            pos.x = game_right_bound - getGlobalBounds().width;
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

        // sf::RectangleShape hitbox(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
        // hitbox.setPosition(getPosition());
        // hitbox.setFillColor(sf::Color::Transparent);
        // hitbox.setOutlineColor(sf::Color::Red);
        // hitbox.setOutlineThickness(2);
        // window.draw(hitbox);
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

    void update_health(int &damage)
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
        shield.setPosition(getPosition().x + getGlobalBounds().width * 2, getPosition().y + getGlobalBounds().height - shield.getGlobalBounds().height);
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

    void setName(std::string &n)
    {
        name = n;

        //  std::cout << "Name set to: " << name << std::endl;
    }

    std::string getName()
    {
        return name;
    }
};