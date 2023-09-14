#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"
#include "Wall.hpp"

class EnemyShooter{
    public:
        EnemyShooter(float x, float y);
        void update(std::vector<Bullet>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls);
        void move(float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        float getAngle();
        int getHP();

    private:
        float x,y,speed,angle,shootTimer;
        int hp;
};