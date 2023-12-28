#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"
#include "Wall.hpp"

class EnemySniper{
    public:
        EnemySniper(float x, float y, int level);
        void update(std::vector<Bullet>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls);
        void shoot(std::vector<Bullet> &bullets);
        void move(float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();        
        float getY();
        float getBulletSpeed();
        float getAngle();
        int getHP();

    private:
        float x,y,speed,angle,shootTimer, bulletSpeed;
        int hp, level;
};