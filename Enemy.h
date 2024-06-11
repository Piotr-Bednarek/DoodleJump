#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

#include "AnimatedSprite.h"
#include "Projectile.h"
#include "Weapon.h"

enum class EnemyType
{
    DRAGON,
    BEE,
    KAMIKAZE,
};

class Enemy : public AnimatedSprite
{
private:
    float speed;
    int directionX;
    int directionY = 1;

    int max_health = 30;
    int health_points = max_health;

    std::vector<sf::Texture> enemy_texture;

    Weapon weapon;
    bool is_weapon_active = true;

    int game_left_bound;
    int game_right_bound;

    float scaleX;
    float scaleY;

    sf::Vector2f player_position;

    ProjectileType projectile_type;

    EnemyType enemy_type;

    int window_height;

    int kamikaze_damage = 25;

    sf::Clock timer;
    bool timer_active = false;

public:
    Enemy(sf::Vector2f pos, float &s, int &dir, std::vector<sf::Texture> texture, int &left_bound, int &right_bound, float sX, float sY, ProjectileType proj_type, EnemyType e_type) : AnimatedSprite(pos, 10), weapon(pos, WeaponType::SINGLE, left_bound, right_bound, proj_type), speed(s), directionX(dir), enemy_texture(texture), scaleX(sX), scaleY(sY), game_left_bound(left_bound), game_right_bound(right_bound), projectile_type(proj_type), enemy_type(e_type)
    {
        if (enemy_type == EnemyType::KAMIKAZE)
        {
            is_weapon_active = false;
            max_health = 25;
        }

        if (enemy_type == EnemyType::DRAGON)
        {
            max_health = 100;
        }
        if (enemy_type == EnemyType::BEE)
        {
            max_health = 50;
        }

        health_points = max_health;

        if (directionX == 1)
        {
            setScale(-scaleX, scaleY);
        }
        else if (directionX == -1)
        {
            setScale(scaleX, scaleY);
        }

        setTexture(enemy_texture[0]);
    }

    sf::Sprite &getSprite()
    {
        return *this;
    }

    void update(float &dt, sf::RenderWindow &window, sf::Vector2f player_pos)
    {

        if (timer.getElapsedTime() > sf::milliseconds(100) && timer_active)
        {
            setColor(sf::Color::White);
        }

        window_height = window.getSize().y;

        player_position = player_pos;

        if (is_weapon_active)
        {
            int width = getGlobalBounds().width;
            int height = getGlobalBounds().height;

            sf::Vector2f weaponPos = getPosition();
            weaponPos.x += getGlobalBounds().width / 2.0f;
            weaponPos.y += getGlobalBounds().height / 2.0f;
            weapon.update(dt, getPosition() + sf::Vector2f(0, height / 2), window);
        }
        step();
        draw(window);
    }

    void move(float dt, float dy)
    {
        sf::Vector2f pos = getPosition();

        pos.y += dy;
        setPosition(pos);

        // DRAGON BEHAVIOR
        if (enemy_type == EnemyType::DRAGON)
        {
            pos.x += speed * directionX * dt;
            setPosition(pos);

            if (pos.x < game_left_bound || pos.x > game_right_bound)
            {
                directionX *= -1;
                bounceX();
            }

            if (rand() % 100 < 2)
            {
                shoot(WeaponType::SINGLE);
            }

            weapon.move(0, dy);
        }

        // BEE BEHAVIOR
        else if (enemy_type == EnemyType::BEE)
        {
            pos.x += speed * directionX * dt;
            pos.y += speed * directionY * dt;
            setPosition(pos);

            if (pos.x < game_left_bound || pos.x > game_right_bound)
            {
                directionX *= -1;
                bounceX();
            }

            if ((pos.y < 0 && directionY == -1) || (pos.y > window_height / 2.0f && directionY == 1))
            {
                directionY *= -1;
            }

            if (rand() % 100 < 5)
            {
                shoot(WeaponType::SINGLE);
            }

            weapon.move(0, dy);
        }

        // KAMIKAZE BEHAVIOR
        else if (enemy_type == EnemyType::KAMIKAZE)
        {
            sf::Vector2f direction = player_position - getPosition();

            float max_val = std::max(std::abs(direction.x), std::abs(direction.y));
            direction.x /= max_val;
            direction.y /= max_val;

            if (direction.x < 0)
            {
                setScale(abs(scaleX), scaleY);
            }
            else if (direction.x > 0)
            {
                setScale(-abs(scaleX), scaleY);
            }

            pos.x += direction.x * speed * dt;
            pos.y += direction.y * speed * dt;
            setPosition(pos);
        }
    }

    void bounceX()
    {
        float width = getGlobalBounds().width;

        sf::Vector2f position = getPosition();

        if (directionX == 1)
        {
            setScale(-scaleX, scaleY);
            setPosition(position.x + width, position.y);

            if (is_weapon_active)
            {
                weapon.move(-width);
            }
        }
        else if (directionX == -1)
        {
            setScale(scaleX, scaleY);
            setPosition(position.x - width, position.y);

            if (is_weapon_active)
            {
                weapon.move(width);
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {

        if (is_weapon_active)
        {
            weapon.draw(window);
        }
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
            weapon.shoot(type, angle);
        }
    }

    void update_health(int damage)
    {
        health_points -= damage;
        setColor(sf::Color::Red);

        timer.restart();
        timer_active = true;
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

    int check_kamikaze_collision(sf::FloatRect bounds, bool is_massacre_active)
    {
        if (is_massacre_active)
        {
            return -1;
        }

        if (getGlobalBounds().intersects(bounds))
        {
            return kamikaze_damage;
        }

        return -1;
    }

    EnemyType get_enemy_type()
    {
        return enemy_type;
    }
    void setWeaponActive(bool active)
    {
        is_weapon_active = active;
    }
    void set_Texture(AnimationType type)
    {
        setTexture(enemy_texture[static_cast<int>(type)]);
    }
};
