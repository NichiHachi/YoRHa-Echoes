#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <chrono>

#include "bullet.hpp"
#include "player.hpp"
#include "enemyShooter.hpp"
#include "enemyTurret.hpp"

const int displayX = 1000;
const int displayY = 1000;
const int bulletRadius = 15;

double calcul_angle(int startX, int startY, int endX, int endY){
    return atan2(startY-endY,endX-startX);
}

int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    //Init Player
    Player player(0,0);

    //Init EnemiesShooter Array
    std::vector<EnemyShooter> enemiesShooter;
    enemiesShooter.emplace_back(0,0);
    enemiesShooter.emplace_back(700,700);
    enemiesShooter.emplace_back(205,90);
    enemiesShooter.emplace_back(60,800);

    //Init EnemyTurret Array
    std::vector<EnemyTurret> enemiesTurret;
    enemiesTurret.emplace_back(900,900);
    enemiesTurret.emplace_back(100,100);

    //Init Bullet Array
    std::vector<Bullet> bullets;

    //Time track and Framerate
    int timePassed;
    sf::Clock clock;
    window.setFramerateLimit(60);

    int idEnemy; 
    int idBullet;
    while(window.isOpen()){
        //Compute the frame rate
        float currentTime = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        //PLAYER
        player.update(bullets,window,currentTime);
        player.draw(window);

        //BULLETS 
        idBullet = 0;
        for(Bullet &bullet : bullets){
            bullet.update();
            bullet.draw(window);

            //If the enemy bullet touch the player HitBox: -1HP and destroy the bullet
            if(!bullet.isAlly()){
                if(player.getShot(bullet)){
                            bullets.erase(bullets.begin()+idBullet);
                }
            }
            else{
                int idBullet2=0;
                //If the ally bullet touch a enemy bullet that is destructible -> destroy both bullet
                for(Bullet &bullet_collision : bullets){
                    if(!bullet_collision.isAlly() && bullet_collision.isDestructible()){
                        int diffX = bullet_collision.getX()-bullet.getX();
                        int diffY = bullet_collision.getY()-bullet.getY();
                        if(diffX>-bulletRadius && diffX<bulletRadius && diffY>-bulletRadius && diffY<bulletRadius){
                            bullets.erase(bullets.begin()+idBullet2);
                            bullets.erase(bullets.begin()+idBullet);
                        }
                    }
                    idBullet2++;
                }

                //If the ally bullet touch the enemyShooter -> -1HP and look if he died
                idEnemy = 0;
                for(EnemyShooter &enemy: enemiesShooter){
                    if(enemy.getShot(bullet)){
                        bullets.erase(bullets.begin()+idBullet);
                        if(enemy.getHP()<=0){
                            enemiesShooter.erase(enemiesShooter.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                
                idEnemy=0;
                for(EnemyTurret &enemy: enemiesTurret){
                    if(enemy.getShot(bullet)){
                        bullets.erase(bullets.begin()+idBullet);
                        if(enemy.getHP()<=0){
                            enemiesTurret.erase(enemiesTurret.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
            }

            if(bullet.getX()<0|| bullet.getY()<0 || bullet.getX()>displayX|| bullet.getY()>displayY){
                bullets.erase(bullets.begin()+idBullet);
            }

            idBullet++;
        }

        //ENEMIES
        ////EnemyShooter update
        for(EnemyShooter &enemy : enemiesShooter){
            float angleEnemyToPlayer = calcul_angle(enemy.getX(),enemy.getY(),player.getX(),player.getY());
            enemy.update(bullets, currentTime, angleEnemyToPlayer);
            enemy.draw(window);
        }

        ////EnemyTurret update
        for(EnemyTurret &enemy : enemiesTurret){
            enemy.update(bullets,currentTime);
            enemy.draw(window);
        }

        player.drawHealth(window);

        window.display();
    }

    return 0;
}