#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum class PowerUpType
{
    HEAL,
    INVINCIBILITY,
    JUMPBOOST,
    MASSACRE
};

class PowerUp : public sf::Sprite
{
private:
    PowerUpType type;
    int effect_amount;

public:
    PowerUp(sf::Texture &texture, PowerUpType p_type, int amount) : sf::Sprite()
    {
        setTexture(texture);
        type = p_type;
        effect_amount = amount;
    }

    PowerUpType getType()
    {
        return type;
    }

    int getEffectAmount()
    {
        return effect_amount;
    }
};