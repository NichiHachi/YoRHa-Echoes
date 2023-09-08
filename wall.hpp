#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#ifndef WALL_HPP
#define WALL_HPP
class Wall{
    public:
        Wall(float x, float y) : x(x), y(y), size(50){}

        void draw(sf::RenderWindow& window){
            sf::VertexArray quad(sf::Quads, 4);
            quad[0].color = sf::Color::White;
            quad[1].color = sf::Color::White;
            quad[2].color = sf::Color::White;
            quad[3].color = sf::Color::White;

            quad[0].position = sf::Vector2f(x+size/2,y+size/2);
            quad[1].position = sf::Vector2f(x-size/2,y+size/2);
            quad[2].position = sf::Vector2f(x-size/2,y-size/2);
            quad[3].position = sf::Vector2f(x+size/2,y-size/2);
            window.draw(quad);
        }

        bool isInWall(float objectX, float objectY){
            return(objectX<x+size/2 && objectX>x-size/2 && objectY<y+size/2 && objectY>y-size/2);
        }

        float getX() const{
            return x;
        }

        float getY() const{
            return y;
        }

        float getSize() const{
            return size;
        }


    private:
        float x,y,size;
};
#endif