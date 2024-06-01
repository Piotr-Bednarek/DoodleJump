#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include <iostream>

class AnimatedSprite : public sf::Sprite
{
private:
    std::vector<sf::IntRect> animation_frames;
    sf::Clock clock;
    int animation_fps;
    int current_frame;

public:
    AnimatedSprite(sf::Vector2f pos, int fps) : sf::Sprite()
    {
        setPosition(pos);
        animation_fps = fps;
        current_frame = 0;
    }

    void add_animation_frame(sf::IntRect frame_rect)
    {
        animation_frames.emplace_back(frame_rect);

        if (animation_frames.size() == 1)
        {
            setTextureRect(animation_frames[0]);
        }
    }

    void step()
    {
        // std::cout << animation_frames.size() << std::endl;
        if (clock.getElapsedTime().asSeconds() >= 1.0f / animation_fps)
        {
            current_frame = (current_frame + 1) % animation_frames.size();
            setTextureRect(animation_frames[current_frame]);
            clock.restart();

            // std::cout << current_frame << std::endl;

            // std::cout << "Stepping" << std::endl;
        }
    }
};
