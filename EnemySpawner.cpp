#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Bullet.hpp"
#include "EnemySpawner.hpp"
#include "Wall.hpp"

// Enemy Seeker
EnemySeeker::EnemySeeker(float x, float y, float angle, int level) {
    this->x = x;
    this->y = y;
    this->level = level;
    this->speed = 3.75;
    this->hp = 4;
    this->angle = angle;
}

void EnemySeeker::update(float targetAngle, std::vector<Wall> walls) {
    move(targetAngle, walls);
}

void EnemySeeker::move(float targetAngle, std::vector<Wall> walls) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2*M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2*M_PI;
    }

    angle += angleDiff*0.05;

    if (angle > M_PI) {
        angle -= 2*M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2*M_PI;
    }

    bool xInWall = false;
    bool yInWall = false;
    if (walls.size() != 0) {
        for (Wall wall : walls) {
        if (wall.isInWall(x + cos(angle) * speed, y)) {
            xInWall = true;
        }
        if (wall.isInWall(x, y - sin(angle) * speed)) {
            yInWall = true;
        }
        }
    }
    if (!xInWall) {
        x += cos(angle) * speed;
    }
    if (!yInWall) {
        y -= sin(angle) * speed;
    }

    if (x < 15) {
        x = 15;
    } 
    else if (x > 985) {
        x = 985;
    }

    if (y < 15) {
        y = 15;
    } 
    else if (y > 985) {
        y = 985;
    }
}

void EnemySeeker::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy(sf::Triangles, 3);

    enemy[0].position = sf::Vector2f(x + cos(angle) * 15, y - sin(angle) * 15);
    enemy[1].position = sf::Vector2f(x + cos(angle + M_PI * 2 / 3) * 15,
                                    y - sin(angle + M_PI * 2 / 3) * 15);
    enemy[2].position = sf::Vector2f(x + cos(angle - M_PI * 2 / 3) * 15,
                                    y - sin(angle - M_PI * 2 / 3) * 15);

    enemy[0].color = enemiesColor;
    enemy[1].color = enemiesColor;
    enemy[2].color = enemiesColor;

    window.draw(enemy);
}

bool EnemySeeker::getShot(Bullet bullet) {
    int diffX = x - bullet.getX();
    int diffY = y - bullet.getY();
    // If the border of the bullet touch the enemy (15+19 -> bullet radius + hit
    // box enemy)
    if (std::sqrt(diffX * diffX + diffY * diffY) < 34) {
        hp--;
        return true;
    }
    return false;
}

float EnemySeeker::getX() { return x; }

float EnemySeeker::getY() { return y; }

int EnemySeeker::getHP() { return hp; }

// Enemy Spawner
EnemySpawner::EnemySpawner(float x, float y, int level) {
    this->x = x;
    this->y = y;
    this->hp = 10+4;
    this->shootTimer = 0;
    this->level = level;
}

void EnemySpawner::update(std::vector<EnemySeeker> &enemiesSeeker, float timePassed) {
    shootTimer += timePassed;
    if (shootTimer >= 5) {
        for (int i = 0; i < 3; i++) {
            enemiesSeeker.emplace_back(x, y, M_PI*2*i/3, level);
        }
        shootTimer = 0;
    }
}

void EnemySpawner::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray quad(sf::Quads, 4);

    quad[0].color = enemiesColor;
    quad[1].color = enemiesColor;
    quad[2].color = enemiesColor;
    quad[3].color = enemiesColor;

    quad[0].position = sf::Vector2f(x + 25, y + 7);
    quad[1].position = sf::Vector2f(x - 25, y + 7);
    quad[2].position = sf::Vector2f(x - 25, y - 7);
    quad[3].position = sf::Vector2f(x + 25, y - 7);

    window.draw(quad);

    quad[0].position = sf::Vector2f(x + 7, y + 25);
    quad[1].position = sf::Vector2f(x - 7, y + 25);
    quad[2].position = sf::Vector2f(x - 7, y - 25);
    quad[3].position = sf::Vector2f(x + 7, y - 25);

    window.draw(quad);
    }

    bool EnemySpawner::getShot(Bullet bullet) {
    int diffX = x - bullet.getX();
    int diffY = y - bullet.getY();
    // If the border of the bullet touch the enemy (15+19 -> bullet radius + hitbox enemy)
    if (std::sqrt(diffX * diffX + diffY * diffY) < 34) {
        hp--;
        return true;
    }
    return false;
}

float EnemySpawner::getX() { return x; }

float EnemySpawner::getY() { return y; }

int EnemySpawner::getHP() { return hp; }