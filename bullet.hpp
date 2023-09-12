#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#ifndef BULLET_HPP
#define BULLET_HPP
class Bullet{
    public:
        Bullet(float x, float y, float angle, float speed, bool Ally, bool Destructible) : x(x), y(y), speed(speed), angle(angle), Ally(Ally), Destructible(Destructible){}

        void update(){
            x+=speed*cos(angle);
            y-=speed*sin(angle);
        }

    void draw(sf::RenderWindow& window){
        if(Ally){
            sf::VertexArray quad(sf::Quads, 4);
            quad[0].color = sf::Color::White;
            quad[1].color = sf::Color::White;
            quad[2].color = sf::Color::White;
            quad[3].color = sf::Color::White;

            quad[0].position = sf::Vector2f(20*std::cos(angle)+10*std::sin(angle)+ x,-20*std::sin(angle)+10*std::cos(angle)+ y);
            quad[1].position = sf::Vector2f(-20*std::cos(angle)+10*std::sin(angle)+ x,20*std::sin(angle)+10*std::cos(angle)+ y);
            quad[2].position = sf::Vector2f(-20*std::cos(angle)-10*std::sin(angle)+ x,20*std::sin(angle)-10*std::cos(angle)+ y);
            quad[3].position = sf::Vector2f(20*std::cos(angle)-10*std::sin(angle)+ x,-20*std::sin(angle)-10*std::cos(angle)+ y);
            window.draw(quad);
        }
        else{
            int bulletRadius = 15;
            sf::Color enemiesBulletInvinsibleColor(255,102,51);
            sf::Color enemiesBulletDestructibleColor(70,11,102);
            sf::CircleShape circle;
            circle.setRadius(bulletRadius);
            if(Destructible){
                circle.setFillColor(enemiesBulletDestructibleColor);
            }
            else{
                circle.setFillColor(enemiesBulletInvinsibleColor);
            }
            circle.setPosition(x-bulletRadius,y-bulletRadius);
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

        bool isAlly() const{
            return Ally;
        }

        bool isDestructible() const{
            return Destructible;
        }

    private:
        float x,y,angle,speed;
        bool Ally, Destructible;
};
#endif