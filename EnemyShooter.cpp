#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"
#include "EnemyShooter.hpp"
#include "Wall.hpp"

EnemyShooter::EnemyShooter(float x, float y, int level) {
    this->x = x;
    this->y = y;
    this-> level = level;
    this->angle = M_PI*3/2;
    this->speed = 1;
    this->hp = 6;
    this->shootTimer = 0;
}

void EnemyShooter::update(std::vector<Bullet> &bullets, float timePassed, float targetAngle, std::vector<Wall> walls) {
    move(targetAngle, walls);

    // Shoot every 2 secondes
    shootTimer += timePassed;
    shoot(bullets);
}

void EnemyShooter::shoot(std::vector<Bullet> &bullets){
    switch (level) {
        case 1 :
            if (shootTimer >= 1.8) {
                bullets.emplace_back(x,y,angle, 3.2, false, true);
                bullets.emplace_back(x,y,angle+M_PI/5, 3.3, false, true);
                bullets.emplace_back(x,y,angle-M_PI/5, 3.3, false, true);
                shootTimer = 0;
                break;
            }
        case 2 :
            if (shootTimer >= 1.4) {
                bullets.emplace_back(x,y,angle, 3.8, false, false);
                bullets.emplace_back(x,y,angle+M_PI/10, 3.8, false, true);
                bullets.emplace_back(x,y,angle+M_PI/5, 3.8, false, false);
                bullets.emplace_back(x,y,angle-M_PI/10, 3.8, false, true);
                bullets.emplace_back(x,y,angle-M_PI/5, 3.8, false, false);
                shootTimer = 0;
                break;
            }
        default :
            if (shootTimer >= 2) {
                bullets.emplace_back(x, y, angle, 3, false, true);
                shootTimer = 0;
            }
            break;
    }
}

void EnemyShooter::move(float targetAngle, std::vector<Wall> walls) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * 0.2;

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2 * M_PI;
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
}

void EnemyShooter::draw(sf::RenderWindow &window) {
    int hauteur = 24;
    int rayon = 19;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_left_part(sf::Triangles, 3);
    sf::VertexArray enemy_right_part(sf::Triangles, 3);
    float angle_point_triangle_1 = atan2(hauteur, rayon);
    float angle_point_triangle_2 = atan2(hauteur, -rayon);
    float distance_point_triangle = sqrt(hauteur * hauteur + rayon * rayon);

    ////Define the coordonate of the enemy's point that are aline with the mouse
    float enemy_sprite_down_x = x - hauteur * 0.6 * cos(angle);
    float enemy_sprite_down_y = y + hauteur * 0.6 * sin(angle);

    float enemy_sprite_up_x = x + hauteur * cos(angle);
    float enemy_sprite_up_y = y - hauteur * sin(angle);

    float angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    float enemy_sprite_left_x =
        x + distance_point_triangle * cos(angle_triangle_left);
    float enemy_sprite_left_y =
        y - distance_point_triangle * sin(angle_triangle_left);

    float angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    float enemy_sprite_right_x =
        x + distance_point_triangle * cos(angle_triangle_right);
    float enemy_sprite_right_y =
        y - distance_point_triangle * sin(angle_triangle_right);

    ////Define the position of the left part points
    enemy_left_part[0].position =
        sf::Vector2f(enemy_sprite_left_x, enemy_sprite_left_y);
    enemy_left_part[1].position =
        sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_left_part[2].position =
        sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);

    ////Define the color of the left part
    enemy_left_part[0].color = enemiesColor;
    enemy_left_part[1].color = enemiesColor;
    enemy_left_part[2].color = enemiesColor;

    ////Define the position of right part points
    enemy_right_part[0].position =
        sf::Vector2f(enemy_sprite_right_x, enemy_sprite_right_y);
    enemy_right_part[1].position =
        sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_right_part[2].position =
        sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);

    ////Define the color of the right part
    enemy_right_part[0].color = enemiesColor;
    enemy_right_part[1].color = enemiesColor;
    enemy_right_part[2].color = enemiesColor;

    window.draw(enemy_left_part);
    window.draw(enemy_right_part);
}

bool EnemyShooter::getShot(Bullet bullet) {
    int diffX = x - bullet.getX();
    int diffY = y - bullet.getY();
    // If the border of the bullet touch the enemy (15+19 -> bullet radius + hitbox enemy)
    if (std::sqrt(diffX * diffX + diffY * diffY) < 34) {
        hp--;
        return true;
    }
    return false;
}

float EnemyShooter::getX() { return x; }

float EnemyShooter::getY() { return y; }

float EnemyShooter::getAngle() { return angle; }

int EnemyShooter::getHP() { return hp; }