#ifndef BULLET_HPP
#define BULLET_HPP
class Bullet{
    public:
        Bullet(float x, float y, float angle, float speed, bool ally, bool destructible);
        void update();
        void draw(sf::RenderWindow& window);
        float getX();
        float getY();
        float getAngle();
        bool isAlly();
        bool isDestructible();
        
    private:
        float x,y,angle,speed;
        bool ally, destructible;
};
#endif