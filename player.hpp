#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"


class Player{
    public:
        Player(float x, float y) : x(x),y(y),speed(5),hp(10),angle(0),shootTimer(0){}
        void update(std::vector<Bullet>& bullets,sf::RenderWindow& window,float timePassed){
            move(window);

            shootTimer+=timePassed;
            shoot(bullets);
        }

        void move(sf::RenderWindow& window){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                y -= speed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                y += speed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                x += speed;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                x -= speed;
            }
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            angle = atan2(y-mousePosition.y,mousePosition.x-x);
        }

        void shoot(std::vector<Bullet>& bullets){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                if(shootTimer>0.2){
                    bullets.emplace_back(x,y,angle,10,true,false);
                    shootTimer = 0;
                }
            }
        }

        bool getShot(Bullet bullet){
            int diffX = x-bullet.getX();
            int diffY = y-bullet.getY();
            //If the border of the bullet touch the player (15+20/2 -> bullet radius + hit box player)
            if(std::sqrt(diffX*diffX+diffY*diffY)<25){
                hp--;
                return true;
            }
            return false;
        }
        
        void draw(sf::RenderWindow& window){
            int hauteur = 24;
            int rayon = 19;
            sf::VertexArray player_left_part(sf::Triangles,3);
            sf::VertexArray player_right_part(sf::Triangles,3);
            float angle_point_triangle_1 = atan2(hauteur,rayon);
            float angle_point_triangle_2 = atan2(hauteur,-rayon);
            float distance_point_triangle = sqrt(hauteur*hauteur+rayon*rayon);

            ////Define the coordonate of the player's point that are aline with the mouse
            float player_sprite_down_x = x-hauteur*0.6*cos(angle);
            float player_sprite_down_y = y+hauteur*0.6*sin(angle);

            float player_sprite_up_x = x+hauteur*cos(angle);
            float player_sprite_up_y = y-hauteur*sin(angle);

            float angle_triangle_left = angle_point_triangle_1+angle+M_PI/2;
            float player_sprite_left_x = x+distance_point_triangle*cos(angle_triangle_left);
            float player_sprite_left_y = y-distance_point_triangle*sin(angle_triangle_left);

            float angle_triangle_right = angle_point_triangle_2+angle+M_PI/2;
            float player_sprite_right_x = x+distance_point_triangle*cos(angle_triangle_right);
            float player_sprite_right_y = y-distance_point_triangle*sin(angle_triangle_right);

            ////Define the position of the left part points
            player_left_part[0].position = sf::Vector2f(player_sprite_left_x,player_sprite_left_y);
            player_left_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
            player_left_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);
        
            ////Define the color of the left part
            player_left_part[0].color = sf::Color::White;
            player_left_part[1].color = sf::Color::White;
            player_left_part[2].color = sf::Color::White;

            ////Define the position of right part points
            player_right_part[0].position = sf::Vector2f(player_sprite_right_x,player_sprite_right_y);
            player_right_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
            player_right_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);
        
            ////Define the color of the right part
            player_right_part[0].color = sf::Color::White;
            player_right_part[1].color = sf::Color::White;
            player_right_part[2].color = sf::Color::White;

            window.draw(player_left_part);
            window.draw(player_right_part);
        }

        void drawHealth(sf::RenderWindow& window){
            sf::CircleShape circle;
            circle.setRadius(15);
            circle.setFillColor(sf::Color::Red);
            for(int i=0;i<hp;i++){
                circle.setPosition(15+35*i,15);
                window.draw(circle);
        }
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

        void getShot(){
            hp--;
        }

    private:
        float x,y,speed,shootTimer;
        int hp;
        double angle;
};