#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"
#include "Wall.hpp"

class Player{
    public:
        Player(float x, float y);
        void update(std::vector<Bullet>& bullets,sf::RenderWindow& window,float timePassed, std::vector<Wall> walls);
        void move(sf::RenderWindow& window, std::vector<Wall> walls);
        void shoot(std::vector<Bullet>& bullets);
        bool getHit(int objectX, int objectY);
        void draw(sf::RenderWindow& window);
        void drawHealth(sf::RenderWindow& window);
        float getX();
        float getY();
        double getAngle();
        float getSpeed();
        int getHP();

    private:
        float x,y,speed,shootTimer,hitTimer;
        int hp;
        double angle;
};