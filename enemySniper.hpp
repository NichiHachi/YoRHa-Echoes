#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"


class EnemySniper{
    public:
        EnemySniper(float x, float y) : x(x), y(y), angle(M_PI+M_PI/2), speed(0.4), hp(4), shootTimer(0){}

        void update(std::vector<Bullet>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls){
            move(targetAngle, walls);

            //Shoot every 3.5 secondes
            shootTimer+=timePassed;
            if(shootTimer>=3.5){
                bullets.emplace_back(x+cos(angle)*15,y+sin(angle)*15,angle,12,false,false);
                shootTimer = 0;
            }
        }

        void move(float targetAngle, std::vector<Wall> walls){
            float angleDiff = targetAngle - angle;
            if(angleDiff>M_PI){
                angleDiff-=2*M_PI;
            }
            else if(angleDiff<-M_PI){
                angleDiff+=2*M_PI;
            }

            angle += angleDiff*0.1;

            if(angle>M_PI){
                angle-=2*M_PI;
            }
            else if(angle<-M_PI){
                angle+=2*M_PI;
            }

            bool xInWall = false;
            bool yInWall = false;
            if(walls.size()!=0){
                for(Wall wall : walls){
                    if(wall.isInWall(x-cos(angle)*speed,y)){
                        xInWall = true;
                    }
                    if(wall.isInWall(x,y+sin(angle)*speed)){
                        yInWall = true;
                    }
                }
            }
            if(!xInWall){
                x-=cos(angle)*speed;
            }
            if(!yInWall){
                y+=sin(angle)*speed;
            }

            if(x<19){
                x=19;
            }
            else if(x>981){
                x=981;
            }
            if(y<19){
                y=19;
            }
            else if(y>981){
                y=981;
            }
        }
        
        void draw(sf::RenderWindow& window){
            int hauteur = 50;
            int rayon = 19;
            sf::Color enemiesColor(200,200,200);
            sf::VertexArray enemyUp(sf::Triangles,3);
            sf::VertexArray enemyDown(sf::Triangles,3);

            enemyUp[0].position = sf::Vector2f(hauteur*cos(angle)+x,-hauteur*sin(angle)+y);
            enemyUp[1].position = sf::Vector2f(rayon*cos(angle+M_PI/2)+x,-rayon*sin(angle+M_PI/2)+y);
            enemyUp[2].position = sf::Vector2f(rayon*cos(angle-M_PI/2)+x,-rayon*sin(angle-M_PI/2)+y);

            enemyUp[0].color = enemiesColor;
            enemyUp[1].color = enemiesColor;
            enemyUp[2].color = enemiesColor;

            enemyDown[0].position = sf::Vector2f(-hauteur*cos(angle)/3+x,hauteur*sin(angle)/3+y);
            enemyDown[1].position = sf::Vector2f(rayon*cos(angle+M_PI/2)+x,-rayon*sin(angle+M_PI/2)+y);
            enemyDown[2].position = sf::Vector2f(rayon*cos(angle-M_PI/2)+x,-rayon*sin(angle-M_PI/2)+y);

            enemyDown[0].color = enemiesColor;
            enemyDown[1].color = enemiesColor;
            enemyDown[2].color = enemiesColor;

            window.draw(enemyUp);
            window.draw(enemyDown);
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
        float x,y,speed,angle,shootTimer;
        int hp;
};