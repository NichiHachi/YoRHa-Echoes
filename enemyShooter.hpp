#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"

class EnemyShooter{
    public:
        EnemyShooter(float x, float y) : x(x), y(y), angle(M_PI+M_PI/2), speed(1), hp(5), shootTimer(0){}

        void update(std::vector<Bullet>& bullets, float timePassed, float targetAngle){
            angle = targetAngle;

            x+=speed*cos(angle);
            y-=speed*sin(angle);

            //Shoot every N seconde
            shootTimer+=timePassed;
            if(shootTimer>=2){
                bullets.emplace_back(x,y,angle,3,false,true);
                shootTimer = 0;
            }
        }
        
        void draw(sf::RenderWindow& window){
            int hauteur = 24;
            int rayon = 19;
            sf::Color enemiesColor(200,200,200);
            sf::VertexArray enemy_left_part(sf::Triangles,3);
            sf::VertexArray enemy_right_part(sf::Triangles,3);
            float angle_point_triangle_1 = atan2(hauteur,rayon);
            float angle_point_triangle_2 = atan2(hauteur,-rayon);
            float distance_point_triangle = sqrt(hauteur*hauteur+rayon*rayon);

            ////Define the coordonate of the enemy's point that are aline with the mouse
            float enemy_sprite_down_x = x-hauteur*0.6*cos(angle);
            float enemy_sprite_down_y = y+hauteur*0.6*sin(angle);

            float enemy_sprite_up_x = x+hauteur*cos(angle);
            float enemy_sprite_up_y = y-hauteur*sin(angle);

            float angle_triangle_left = angle_point_triangle_1+angle+M_PI/2;
            float enemy_sprite_left_x = x+distance_point_triangle*cos(angle_triangle_left);
            float enemy_sprite_left_y = y-distance_point_triangle*sin(angle_triangle_left);

            float angle_triangle_right = angle_point_triangle_2+angle+M_PI/2;
            float enemy_sprite_right_x = x+distance_point_triangle*cos(angle_triangle_right);
            float enemy_sprite_right_y = y-distance_point_triangle*sin(angle_triangle_right);

            ////Define the position of the left part points
            enemy_left_part[0].position = sf::Vector2f(enemy_sprite_left_x,enemy_sprite_left_y);
            enemy_left_part[1].position = sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
            enemy_left_part[2].position = sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);
        
            ////Define the color of the left part
            enemy_left_part[0].color = enemiesColor; 
            enemy_left_part[1].color = enemiesColor;
            enemy_left_part[2].color = enemiesColor;

            ////Define the position of right part points
            enemy_right_part[0].position = sf::Vector2f(enemy_sprite_right_x,enemy_sprite_right_y);
            enemy_right_part[1].position = sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
            enemy_right_part[2].position = sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);
        
            ////Define the color of the right part
            enemy_right_part[0].color = enemiesColor;
            enemy_right_part[1].color = enemiesColor;
            enemy_right_part[2].color = enemiesColor;

            window.draw(enemy_left_part);
            window.draw(enemy_right_part);
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
        float x,y,speed,angle,shootTimer,timeBetweenShot;
        int hp;
};
