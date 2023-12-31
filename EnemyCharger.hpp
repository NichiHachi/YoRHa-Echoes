#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"
#include "Wall.hpp"

class EnemyCharger{
    public:
        EnemyCharger(float x, float y, int level);
        void update(float timePassed, float targetAngle, std::vector<Bullet>&bullets, std::vector<Wall> walls);
        void move(std::vector<Wall> walls,std::vector<Bullet>&bullets);
        void drawWarningZone(sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        float getAngle();
        int getHP();

    private:
        float x,y,speed,angle,shootTimer;
        int hp, level;
};