#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#ifndef WALL_HPP
#define WALL_HPP
class Wall{
    public:
        Wall(float x, float y);
        void draw(sf::RenderWindow& window);
        bool isInWall(float objectX, float objectY);
        float getX();
        float getY();
        float getSize();

    private:
        float x,y,size;
};
#endif