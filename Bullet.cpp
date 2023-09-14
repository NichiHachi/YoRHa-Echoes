#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"

Bullet::Bullet(float x, float y, float angle, float speed, bool ally, bool destructible) {
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->angle = angle;
    this->ally = ally;
    this->destructible = destructible;
}

void Bullet::update() {
    x += speed * cos(angle);
    y -= speed * sin(angle);
}

void Bullet::draw(sf::RenderWindow &window) {
    if (ally) {
        sf::VertexArray quad(sf::Quads, 4);
        quad[0].color = sf::Color::White;
        quad[1].color = sf::Color::White;
        quad[2].color = sf::Color::White;
        quad[3].color = sf::Color::White;

        quad[0].position = sf::Vector2f(20 * std::cos(angle) + 10 * std::sin(angle) + x,
                                    -20 * std::sin(angle) + 10 * std::cos(angle) + y);
        quad[1].position = sf::Vector2f(-20 * std::cos(angle) + 10 * std::sin(angle) + x,
                                    20 * std::sin(angle) + 10 * std::cos(angle) + y);
        quad[2].position = sf::Vector2f(-20 * std::cos(angle) - 10 * std::sin(angle) + x,
                                    20 * std::sin(angle) - 10 * std::cos(angle) + y);
        quad[3].position = sf::Vector2f(20 * std::cos(angle) - 10 * std::sin(angle) + x,
                                    -20 * std::sin(angle) - 10 * std::cos(angle) + y);
        window.draw(quad);
    } 
    else {
        int bulletRadius = 15;
        sf::Color enemiesBulletInvinsibleColor(255, 102, 51);
        sf::Color enemiesBulletDestructibleColor(70, 11, 102);
        sf::CircleShape circle;
        circle.setRadius(bulletRadius);
        if (destructible) {
            circle.setFillColor(enemiesBulletDestructibleColor);
        } 
        else {
            circle.setFillColor(enemiesBulletInvinsibleColor);
        }
        circle.setPosition(x - bulletRadius, y - bulletRadius);
        window.draw(circle);
    }
}

float Bullet::getX() { return x; }

float Bullet::getY() { return y; }

float Bullet::getAngle() { return angle; }

bool Bullet::isAlly() { return ally; }

bool Bullet::isDestructible() { return destructible; }