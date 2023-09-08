#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

class EnemySeeking{
    public:
        EnemySeeking(float x, float y, float angle): x(x), y(y), speed(5.2), angle(angle), hp(3){}
        void update(float targetAngle){
            float angleDiff = targetAngle - angle;
            if(angleDiff>M_PI){
                angleDiff-=2*M_PI;
            }
            else if(angleDiff<-M_PI){
                angleDiff+=2*M_PI;
            }

            angle += angleDiff*0.05;

            if(angle>M_PI){
                angle-=2*M_PI;
            }
            else if(angle<-M_PI){
                angle+=2*M_PI;
            }

            x+=speed*cos(angle);
            y-=speed*sin(angle);
        }

        void draw(sf::RenderWindow& window){
            sf::Color enemiesColor(200,200,200);
            sf::VertexArray enemy(sf::Triangles,3);

            enemy[0].position = sf::Vector2f(x+cos(angle)*15, y-sin(angle)*15);
            enemy[1].position = sf::Vector2f(x+cos(angle+M_PI*2/3)*15, y-sin(angle+M_PI*2/3)*15);
            enemy[2].position = sf::Vector2f(x+cos(angle-M_PI*2/3)*15, y-sin(angle-M_PI*2/3)*15);
        
            enemy[0].color = enemiesColor; 
            enemy[1].color = enemiesColor;
            enemy[2].color = enemiesColor;

            window.draw(enemy);
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

        int getHP() const{
            return hp;
        }

    private:
        float x,y,angle,speed;
        int hp;
};

class EnemySpawner{
    public :
        EnemySpawner(float x, float y) : x(x), y(y), hp(10), shootTimer(0){}
        void update(std::vector<EnemySeeking>& enemiesSeeking, float timePassed){
            shootTimer+=timePassed;
            if(shootTimer>=5){
                for(int i=0;i<3;i++){
                    enemiesSeeking.emplace_back(x,y,M_PI*2*i/3);
                }
                shootTimer=0;
            }
        }

        void draw(sf::RenderWindow& window){
            sf::Color enemiesColor(200,200,200);
            sf::VertexArray quad(sf::Quads, 4);
            
            quad[0].color = enemiesColor;
            quad[1].color = enemiesColor;
            quad[2].color = enemiesColor;
            quad[3].color = enemiesColor;

            quad[0].position = sf::Vector2f(x+20,y+10);
            quad[1].position = sf::Vector2f(x-20,y+10);
            quad[2].position = sf::Vector2f(x-20,y-10);
            quad[3].position = sf::Vector2f(x+20,y-10);

            window.draw(quad);

            quad[0].position = sf::Vector2f(x+10,y+20);
            quad[1].position = sf::Vector2f(x-10,y+20);
            quad[2].position = sf::Vector2f(x-10,y-20);
            quad[3].position = sf::Vector2f(x+10,y-20);

            window.draw(quad);
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

        int getHP() const{
            return hp;
        }

    private:
        float x,y,shootTimer;
        int hp;
};