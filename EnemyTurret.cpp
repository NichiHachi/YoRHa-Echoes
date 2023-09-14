#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"
#include "EnemyTurret.hpp"

EnemyTurret::EnemyTurret(float x, float y) {
    this->x = x;
    this->y = y;
    this->angle = 0;
    this->targetAngle = 0;
    this->hp = 7;
    this->shootTimer = 0;
}

void EnemyTurret::update(std::vector<Bullet> &bullets, float timePassed) {
    angle += (targetAngle - angle) * 0.25;

    // Shoot every 0.25 seconde
    shootTimer += timePassed;
    if (shootTimer >= 0.5) {
        bullets.emplace_back(x, y, angle, 2, false, true);
        bullets.emplace_back(x, y, angle + M_PI / 2, 2, false, false);
        bullets.emplace_back(x, y, angle + M_PI, 2, false, true);
        bullets.emplace_back(x, y, angle + M_PI + M_PI / 2, 2, false, false);
        targetAngle += M_PI / 6;
        shootTimer = 0;
    }
}

void EnemyTurret::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_part(sf::Triangles, 3);
    int radius = 30;
    enemy_part[0].color = enemiesColor;
    enemy_part[1].color = enemiesColor;
    enemy_part[2].color = enemiesColor;
    for (int i = 0; i < 8; i++) {
        enemy_part[0].position = sf::Vector2f(x, y);
        enemy_part[1].position = sf::Vector2f(x + radius * cos((i + 1) * M_PI / 4 - angle), y + radius * sin((i + 1) * M_PI / 4 - angle));
        enemy_part[2].position = sf::Vector2f(x + radius * cos((i + 2) * M_PI / 4 - angle), y + radius * sin((i + 2) * M_PI / 4 - angle));
        window.draw(enemy_part);
    }
}

bool EnemyTurret::getShot(Bullet bullet) {
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

float EnemyTurret::getX() { return x; }

float EnemyTurret::getY() { return y; }

float EnemyTurret::getAngle() { return angle; }

int EnemyTurret::getHP() { return hp; }
