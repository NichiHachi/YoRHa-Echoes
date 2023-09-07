#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"

class EnemyTurret{
    public:
        EnemyTurret(float x, float y) : x(x), y(y), angle(0), targetAngle(0), hp(7), shootTimer(0) {}

        void update(std::vector<Bullet>& bullets, float timePassed){
            angle+=(targetAngle-angle)*0.25;

            //Shoot every 0.25 seconde
            shootTimer+=timePassed;
            if(shootTimer>=0.5){
                bullets.emplace_back(x,y,angle,2,false,true);
                bullets.emplace_back(x,y,angle+M_PI/2,2,false,false);
                bullets.emplace_back(x,y,angle+M_PI,2,false,true);
                bullets.emplace_back(x,y,angle+M_PI+M_PI/2,2,false,false);
                targetAngle += M_PI/6;
                shootTimer = 0;
            }
        }


        void draw(sf::RenderWindow& window){
            sf::Color enemiesColor(200,200,200);
            sf::VertexArray enemy_part(sf::Triangles,3);
            int radius = 30;
            enemy_part[0].color = enemiesColor;
            enemy_part[1].color = enemiesColor;
            enemy_part[2].color = enemiesColor;
            for(int i=0;i<8;i++){
                enemy_part[0].position = sf::Vector2f(x,y);
                enemy_part[1].position = sf::Vector2f(x+radius*cos((i+1)*M_PI/4-angle),y+radius*sin((i+1)*M_PI/4-angle));
                enemy_part[2].position = sf::Vector2f(x+radius*cos((i+2)*M_PI/4-angle),y+radius*sin((i+2)*M_PI/4-angle));
                window.draw(enemy_part);
            }
        }

        bool getShot(Bullet bullet){
            int diffX = x-bullet.getX();
            int diffY = y-bullet.getY();
            //If the border of the bullet touch the enemy (15+19 -> bullet radius + hit box enemy)
            if(std::sqrt(diffX*diffX+diffY*diffY)<34){
                hp--;
                return true;
            }
            return false;
        }


        int getX() const{
            return x;
        }

        int getY() const{
            return y;
        }

        float getAngle() const{
            return angle;
        }

        int getHP() const{
            return hp;
        }

    private:
        float x,y,angle,shootTimer,targetAngle,timeBetweenShot;
        int hp;
};