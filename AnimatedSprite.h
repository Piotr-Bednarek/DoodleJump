#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

enum class AnimationType
{
    FLY,
    HIT,
    DEATH,
};

class AnimatedSprite : public sf::Sprite
{
private:
    std::vector<std::vector<sf::IntRect>> animation_frames;
    sf::Clock clock;
    int animation_fps;
    int current_frame;
    AnimationType current_animation = AnimationType::FLY;

public:
    AnimatedSprite(sf::Vector2f &pos, int fps) : sf::Sprite(), animation_fps(fps), current_frame(0)
    {
        animation_frames.resize(3);
        setPosition(pos);
    }

    void add_animation_frame(sf::IntRect frame_rect, AnimationType type)
    {
        animation_frames[static_cast<int>(type)].emplace_back(frame_rect);

        if (animation_frames[0].size() == 1)
        {
            setTextureRect(animation_frames[static_cast<int>(type)][0]);
        }
    }

    void step()
    {
        if (clock.getElapsedTime().asSeconds() >= 1.0f / animation_fps)
        {
            current_frame = (current_frame + 1) % animation_frames[static_cast<int>(current_animation)].size();
            setTextureRect(animation_frames[static_cast<int>(current_animation)][current_frame]);
            clock.restart();
        }
    }
    void set_animation(AnimationType type)
    {
        current_animation = type;
        current_frame = 0;
        setTextureRect(animation_frames[static_cast<int>(current_animation)][current_frame]);
    }
    AnimationType get_animation()
    {
        return current_animation;
    }
    bool isEndOfAnimation()
    {
        return current_frame == animation_frames[static_cast<int>(current_animation)].size()-1;
    }
};
