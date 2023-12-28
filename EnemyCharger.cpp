#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"
#include "EnemyCharger.hpp"
#include "Wall.hpp"

EnemyCharger::EnemyCharger(float x, float y, int level=0) {
    this->x = x;
    this->y = y;
    this->level = level;
    this->angle = M_PI*3/2;
    this->speed = 13;
    this->hp = 20;
    this->shootTimer = 0;
}

void EnemyCharger::update(float timePassed, float targetAngle, std::vector<Bullet>&bullets, std::vector<Wall> walls) {
    shootTimer += timePassed;
    if (shootTimer > 10) {
        move(walls,bullets);
    } 
    else {
        angle = targetAngle;
    }
}

void EnemyCharger::move(std::vector<Wall> walls, std::vector<Bullet>&bullets) {
    if(level>0){
        if(shootTimer-10>0.25){
            bullets.emplace_back(x,y,angle+M_PI/2,3,false,false);
            bullets.emplace_back(x,y,angle-M_PI/2,3,false,false);
            shootTimer -= 0.25;
        }
    }
    bool xInWall = false;
    bool yInWall = false;
    if (walls.size() != 0) {
        for (Wall wall : walls) {
        if (wall.isInWall(x + cos(angle) * speed, y)) {
            xInWall = true;
            shootTimer = 0;
        }
        if (wall.isInWall(x, y - sin(angle) * speed)) {
            yInWall = true;
            shootTimer = 0;
        }
        }
    }
    if (!xInWall) {
        x += cos(angle) * speed;
    }
    if (!yInWall) {
        y -= sin(angle) * speed;
    }

    if (x < 30) {
        x = 30;
        shootTimer = 0;
    } 
    else if (x > 970) {
        x = 970;
        shootTimer = 0;
    }

    if (y < 30) {
        y = 30;
        shootTimer = 0;
    } 
    else if (y > 970) {
        y = 970;
        shootTimer = 0;
    }
}

void EnemyCharger::drawWarningZone(sf::RenderWindow &window) {
    if (shootTimer > 7) {
        float ratioLength = 1500 * (shootTimer - 7) / 3;
        sf::VertexArray warningZone(sf::Quads, 4);

        warningZone[0].color = sf::Color::Red;
        warningZone[1].color = sf::Color::Red;
        warningZone[2].color = sf::Color::Red;
        warningZone[3].color = sf::Color::Red;

        warningZone[0].position =
            sf::Vector2f(9 * std::cos(angle) + 28 * std::sin(angle) + x,
                        -9 * std::sin(angle) + 28 * std::cos(angle) + y);
        warningZone[1].position =
            sf::Vector2f(ratioLength * std::cos(angle) + 28 * std::sin(angle) + x,
                        -ratioLength * std::sin(angle) + 28 * std::cos(angle) + y);
        warningZone[2].position =
            sf::Vector2f(ratioLength * std::cos(angle) - 28 * std::sin(angle) + x,
                        -ratioLength * std::sin(angle) - 28 * std::cos(angle) + y);
        warningZone[3].position =
            sf::Vector2f(9 * std::cos(angle) - 28 * std::sin(angle) + x,
                        -9 * std::sin(angle) - 28 * std::cos(angle) + y);

        window.draw(warningZone);
    }
}

void EnemyCharger::draw(sf::RenderWindow &window) {
    int rayon = 30;

    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray side(sf::Triangles, 3);

    side[0].color = enemiesColor;
    side[1].color = enemiesColor;
    side[2].color = enemiesColor;

    side[0].position = sf::Vector2f(x, y);

    for (int i = 0; i < 5; i++) {
        side[1].position = sf::Vector2f(x + rayon * cos(angle + i * 2 * M_PI / 5),
                                        y + -rayon * sin(angle + i * 2 * M_PI / 5));
        side[2].position =sf::Vector2f(x + rayon * cos(angle + (i + 1) * 2 * M_PI / 5),
                                      y - rayon * sin(angle + (i + 1) * 2 * M_PI / 5));
        window.draw(side);
    }
    }

    bool EnemyCharger::getShot(Bullet bullet) {
    int diffX = x - bullet.getX();
    int diffY = y - bullet.getY();
    // If the border of the bullet touch the enemy (15+25 -> bullet radius + hitbox enemy)
    if (std::sqrt(diffX * diffX + diffY * diffY) < 40) {
        hp--;
        return true;
    }
    return false;
}

float EnemyCharger::getX() { return x; }

float EnemyCharger::getY() { return y; }

float EnemyCharger::getAngle() { return angle; }

int EnemyCharger::getHP() { return hp; }
