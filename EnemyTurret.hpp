#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"
#include "Wall.hpp"

class EnemyTurret{
    public:
        EnemyTurret(float x, float y, bool moving, int level, int patern);
        void update(std::vector<Bullet>& bullets, std::vector<Wall> walls, float timePassed);
        void move(std::vector<Wall> walls);
        void shoot(std::vector<Bullet> &bullets);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        int getHP();

    private:
        float x,y,shootTimer,shootAngle,xMoving,yMoving;
        int hp,level,patern;
        bool moving;
};