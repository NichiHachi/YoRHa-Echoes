#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.hpp"
#include "Wall.hpp"
#include "EnemyTurret.hpp"

EnemyTurret::EnemyTurret(float x, float y, bool moving, int level, int patern) {
    this->x = x;
    this->y = y;
    this-> moving = moving;
    this-> level = level;
    this-> patern = patern;
    this-> xMoving = 1;
    this-> yMoving = -1;
    this->shootAngle = 0;
    this->hp = 10;
    this->shootTimer = 0;
}

void EnemyTurret::update(std::vector<Bullet> &bullets, std::vector<Wall> walls, float timePassed) {
    if(moving){
        move(walls);
    }
    shootTimer += timePassed;
    shoot(bullets);
}

void EnemyTurret::shoot(std::vector<Bullet> &bullets){
    switch (patern) {
        case 1 :
            if(shootTimer >= 1.5){
                for(int i=0;i<20;i++){
                    bullets.emplace_back(x,y,M_PI*i/10,3.5,false,true);
                }
                shootTimer = 0;
            }
            break;
        
        case 2 :
            if(shootTimer >= 1){
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,i*M_PI/2,3.5,false,true);
                }
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,i*M_PI/2+M_PI/4,3.5*0.7,false,true);
                }
                shootTimer = 0;
            }
            break;

        case 3 :
            if(shootTimer >= 0.2){
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,shootAngle+M_PI*i/2,3,false, i%2==0);
                }
                shootAngle += M_PI/24;
                shootTimer = 0;
            }
            break;

        case 4 :
            if(shootTimer >= 0.4){
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,shootAngle+M_PI*i/2+M_PI/5,3, false, true);
                }
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,-shootAngle+M_PI*i/2-M_PI/5,3,false, false);
                }
                shootAngle += M_PI/16;
                shootTimer = 0;
            }
            break;

        default :
            if(shootTimer >= 0.5){
                for(int i=0;i<4;i++){
                    bullets.emplace_back(x,y,shootAngle+M_PI*i/2,3,false, true);
                }
                shootAngle += M_PI/6;
                shootTimer = 0;
            }
    }
}

void EnemyTurret::move(std::vector<Wall> walls){
    bool xInWall = false;
    bool yInWall = false;
    if (walls.size() != 0) {
        for (Wall wall : walls) {
            if (wall.isInWall(x+xMoving, y)) {
                xInWall = true;
            }
            if (wall.isInWall(x, y+yMoving)) {
                yInWall = true;
            }
        }
    }
    if(xInWall){ xMoving = -xMoving; }
    if(yInWall){ yMoving = -yMoving; }

    if (x < 30) {
        xMoving = -xMoving;
    } 
    else if (x > 970) {
        xMoving = -xMoving;
    }

    if (y < 30) {
        yMoving = -yMoving;
    } 
    else if (y > 970) {
        yMoving = -yMoving;
    }

    x += xMoving;
    y += yMoving;
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
        enemy_part[1].position = sf::Vector2f(x + radius * cos((i + 1) * M_PI / 4 - shootAngle), y + radius * sin((i + 1) * M_PI / 4 - shootAngle));
        enemy_part[2].position = sf::Vector2f(x + radius * cos((i + 2) * M_PI / 4 - shootAngle), y + radius * sin((i + 2) * M_PI / 4 - shootAngle));
        window.draw(enemy_part);
    }
}

bool EnemyTurret::getShot(Bullet bullet) {
    int diffX = x - bullet.getX();
    int diffY = y - bullet.getY();
    // If the border of the bullet touch the enemy (15+19 -> bullet radius + hitbox enemy)
    if (std::sqrt(diffX * diffX + diffY * diffY) < 34) {
        hp--;
        return true;
    }
    return false;
}

float EnemyTurret::getX() { return x; }

float EnemyTurret::getY() { return y; }

int EnemyTurret::getHP() { return hp; }
