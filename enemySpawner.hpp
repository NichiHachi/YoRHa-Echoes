#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"

class EnemySpawner{
    public :
        EnemySpawner(float x, float y) : x(x), y(y), hp(10), shootTimer(0){}

        int getX() const{
            return x;
        }

        int getY() const{
            return y;
        }

        int getHP() const{
            return hp;
        }

        void getShot(){
            hp--;
        }

    private:
        float x,y;
        int hp, shootTimer;
};

class EnemySeeking{

};