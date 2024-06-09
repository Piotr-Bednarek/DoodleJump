#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

    sf::SoundBuffer soundBuffer;
    sf::Sound sound;

public:
    PowerUp(sf::Texture &texture, PowerUpType p_type, int amount) : sf::Sprite()
    {
        setTexture(texture);
        type = p_type;
        effect_amount = amount;

        switch (p_type)
        {
        case PowerUpType::JUMPBOOST:
            if (!soundBuffer.loadFromFile("assets/sounds/jump_powerup.wav"))
            {
                std::cout << "Failed to load sound from assets/sounds/jump_powerup.wav" << std::endl;
            }

            sound.setBuffer(soundBuffer);
            sound.setVolume(1);
            break;
        case PowerUpType::HEAL:
            if (!soundBuffer.loadFromFile("assets/sounds/health_powerup.wav"))
            {
                std::cout << "Failed to load sound from assets/sounds/health_powerup.wav" << std::endl;
            }

            sound.setBuffer(soundBuffer);
            sound.setVolume(2);
            break;
        case PowerUpType::INVINCIBILITY:
            if (!soundBuffer.loadFromFile("assets/sounds/shield_powerup.wav"))
            {
                std::cout << "Failed to load sound from assets/sounds/shield_powerup.wav" << std::endl;
            }

            sound.setBuffer(soundBuffer);
            sound.setVolume(5);
            break;
        case PowerUpType::MASSACRE:
            if (!soundBuffer.loadFromFile("assets/sounds/massacre_powerup.wav"))
            {
                std::cout << "Failed to load sound from assets/sounds/massacre_powerup.wav" << std::endl;
            }

            sound.setBuffer(soundBuffer);
            sound.setVolume(5);
            break;
        }
    }

    PowerUpType getType()
    {
        return type;
    }

    int getEffectAmount()
    {
        sound.play();

        return effect_amount;
    }
};