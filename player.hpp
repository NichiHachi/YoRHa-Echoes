#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "bullet.hpp"
#include "wall.hpp"

class Player{
    public:
        Player(float x, float y) : x(x),y(y),speed(5),hp(10),angle(0),shootTimer(0), hitTimer(2){}
        void update(std::vector<Bullet>& bullets,sf::RenderWindow& window,float timePassed, std::vector<Wall> walls){
            move(window,walls);

            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            angle = atan2(y-mousePosition.y,mousePosition.x-x);

            shootTimer+=timePassed;
            hitTimer+=timePassed;

            shoot(bullets);
        }

        void move(sf::RenderWindow& window, std::vector<Wall> walls){
            float angleMove = -1;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                angleMove = M_PI/4;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                angleMove = M_PI*3/4;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                angleMove = -M_PI*3/4;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                angleMove = -M_PI/4;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
                angleMove = M_PI/2;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
                angleMove = -M_PI/2;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
                angleMove = 0;
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                angleMove = M_PI;
            }

            if(angleMove!=-1){
                bool xInWall = false;
                bool yInWall = false;
                if(walls.size()!=0){
                    for(Wall wall : walls){
                        if(wall.isInWall(x+cos(angleMove)*speed,y)){
                            xInWall = true;
                        }
                        if(wall.isInWall(x,y-sin(angleMove)*speed)){
                            yInWall = true;
                        }
                    }
                }
                if(!xInWall){
                    x+=cos(angleMove)*speed;
                }
                if(!yInWall){
                    y-=sin(angleMove)*speed;
                }
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

        void shoot(std::vector<Bullet>& bullets){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                if(shootTimer>0.2){
                    bullets.emplace_back(x,y,angle,10,true,false);
                    shootTimer = 0;
                }
            }
        }

        bool getHit(int objectX, int objectY){
            int diffX = x-objectX;
            int diffY = y-objectY;
            //If the border of the object (enemy or bullet) touch the player (15+15 -> object hitbox + player hitbox)
            if(std::sqrt(diffX*diffX+diffY*diffY)<=30){
                if(hitTimer>2){
                    hp--;
                    hitTimer = 0;
                }
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
            circle.setRadius(10);
            circle.setFillColor(sf::Color::Red);

            sf::VertexArray quad(sf::Quads, 4);
            quad[0].color = sf::Color::Red;
            quad[1].color = sf::Color::Red;
            quad[2].color = sf::Color::Red;
            quad[3].color = sf::Color::Red;
            for(int i=0;i<hp;i++){
                circle.setPosition(10*cos(M_PI*3/4)+40*i+20,10*sin(M_PI*3/4)+10);
                window.draw(circle);
                circle.setPosition(10*cos(M_PI/4)+40*i+20,10*sin(M_PI/4)+10);
                window.draw(circle);

                quad[0].position = sf::Vector2f(40*i+30,21);
                quad[1].position = sf::Vector2f(40*i+44,35);
                quad[2].position = sf::Vector2f(40*i+30,49);
                quad[3].position = sf::Vector2f(40*i+16,35);
                window.draw(quad);
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

        float getSpeed() const{
            return speed;
        }

        int getHP() const{
            return hp;
        }

    private:
        float x,y,speed,shootTimer,hitTimer;
        int hp;
        double angle;
};