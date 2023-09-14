#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.hpp"
#include "Wall.hpp"

class EnemySeeker{
    public:
        EnemySeeker(float x, float y, float angle);
        void update(float targetAngle, std::vector<Wall> walls);
        void move(float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        int getHP();
  

    private:
        float x,y,angle,speed;
        int hp;
};

class EnemySpawner{
    public :
        EnemySpawner(float x, float y);
        void update(std::vector<EnemySeeker>& enemiesSeeker, float timePassed);
        void draw(sf::RenderWindow& window);
        bool getShot(Bullet bullet);
        float getX();
        float getY();
        int getHP();

    private:
        float x,y,shootTimer;
        int hp;
};