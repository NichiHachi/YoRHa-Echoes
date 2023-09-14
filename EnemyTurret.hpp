#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"

class EnemyTurret{
    public:
        EnemyTurret(float x, float y);
        void update(std::vector<Bullet>& bullets, float timePassed);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        float getAngle();
        int getHP();

    private:
        float x,y,angle,shootTimer,targetAngle;
        int hp;
};