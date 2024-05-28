#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <stdlib.h>

class Platform : public sf::RectangleShape
{
public:
    Platform(sf::Vector2f pos, sf::Vector2f s) : sf::RectangleShape()
    {
        setPosition(pos);
        setSize(s);
        setFillColor(sf::Color::Green);
    }
};

int main()
{
    const int WIDTH = 1000;
    const int HEIGHT = 1000;

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Doodle Jump!");

    std::vector<Platform> platforms;

    int offset = 100;

    int platform_width = 100;

    for (int i=0; i < HEIGHT; i=i+offset)
    {
        int platform_y = HEIGHT - i;
        int platform_x = rand() % (WIDTH-platform_width);


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


        for (const Platform& i : platforms)
        {
            window.draw(i);
        }

        window.display();
    }

    return 0;
}
