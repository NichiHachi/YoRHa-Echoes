#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <set>

#include "Bullet.hpp"
#include "Player.hpp"
#include "Wall.hpp"
#include "EnemyShooter.hpp"
#include "EnemyTurret.hpp"
#include "EnemySpawner.hpp"
#include "EnemySniper.hpp"
#include "EnemyCharger.hpp"

const int displayX = 1000;
const int displayY = 1000;
const int bulletRadius = 15;
const sf::Color backgroundColor(0,0,0);

double calcul_angle(int startX, int startY, int endX, int endY){
    return atan2(startY-endY,endX-startX);
}

double angle_shot_predict(Player player,float objectX, float objectY){
    float diffX = player.getX()-objectX;
    float diffY = player.getY()-objectY;
    float distance = sqrt(diffX*diffX+diffY*diffY);
    float timeBulletTravel = distance/12*1.5;
    float playerNewX, playerNewY;
    float angleNew = -1;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = -M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = -M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        angleNew = M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        angleNew = -M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = 0;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = M_PI;
    }
            
    if(angleNew==-1){
        playerNewX = player.getX();
        playerNewY = player.getY();
    }
    else{
        playerNewX = player.getX()+cos(angleNew)*player.getSpeed()*timeBulletTravel;
        playerNewY = player.getY()-sin(angleNew)*player.getSpeed()*timeBulletTravel;
    }

    return calcul_angle(objectX,objectY,playerNewX,playerNewY);
}

int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    //Init Player
    Player player(0,0);

    //Init EnemyShooter Array
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

    //Init EnemySpawner Array
    std::vector<EnemySpawner> enemiesSpawner;
    enemiesSpawner.emplace_back(400,400);

    //Init EnemySeeker Array
    std::vector<EnemySeeker> enemiesSeeker;

    //Init EnemySniper Array
    std::vector<EnemySniper> enemiesSniper;
    enemiesSniper.emplace_back(400,400);

    //Init EnemyCharger Array
    std::vector<EnemyCharger> enemiesCharger;
    enemiesCharger.emplace_back(450,300);

    //Init Wall Array
    std::vector<Wall> walls;
    walls.emplace_back(200,350);
    walls.emplace_back(200,300);
    walls.emplace_back(250,300);
    walls.emplace_back(300,300);

    //Time track and Framerate
    int timePassed;
    sf::Clock clock;
    window.setFramerateLimit(60);

    //Int values
    int idEnemy; 
    int idBullet;

    while(window.isOpen()){
        //Time passed between frames
        float currentTime = clock.restart().asSeconds();

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(backgroundColor);

        //Warning Zone from Charger Enemies (The last layer)
        for(EnemyCharger enemy : enemiesCharger){
            enemy.drawWarningZone(window);
        }

        //PLAYER
        player.update(bullets,window,currentTime,walls);
        player.draw(window);

        //BULLETS 
        idBullet = 0;
        ////We use a set because if a ally bullet touch a enemy bullet, they will be stored 2 times in the array (ie we prevent that to happen and having memory problem)
        std::set<int> bulletsToDelete;
        while(idBullet < bullets.size()){
            bullets[idBullet].update();
            bullets[idBullet].draw(window);

            bool bulletDestroyed = false;

            //ENEMY'S BULLET
            ////If the enemy bullet touch the player HitBox: -1HP and destroy the bullet
            if(!bullets[idBullet].isAlly()){
                if(player.getHit(bullets[idBullet].getX(),bullets[idBullet].getY())){
                    bulletsToDelete.insert(idBullet);
                    bulletDestroyed = true;
                }
            }

            //ALLY'S BULLET
            else{
                ////If the ally bullet touch a enemy bullet that is destructible -> destroy both bullets
                int idBulletEnemy = 0;
                while(idBulletEnemy < bullets.size() && !bulletDestroyed){
                    if(!bullets[idBulletEnemy].isAlly() && bullets[idBulletEnemy].isDestructible()){
                        int diffX = bullets[idBulletEnemy].getX()-bullets[idBullet].getX();
                        int diffY = bullets[idBulletEnemy].getY()-bullets[idBullet].getY();
                        if(diffX>-bulletRadius && diffX<bulletRadius && diffY>-bulletRadius && diffY<bulletRadius){
                            bulletsToDelete.insert(idBulletEnemy);
                            bulletsToDelete.insert(idBullet);
                            bulletDestroyed = true;
                        }
                    }
                    idBulletEnemy++;
                }

                ////If the ally bullet touch the enemy -> He loose 1 HP and if he has 0 HP (ie if he died) -> Erase him from his array
                //////Enemy Shooter
                idEnemy = 0;
                while(idEnemy < enemiesShooter.size() && !bulletDestroyed){
                    if(enemiesShooter[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesShooter[idEnemy].getHP()<=0){
                            enemiesShooter.erase(enemiesShooter.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                //////Enemy Shooter
                idEnemy=0;
                while(idEnemy < enemiesTurret.size() && !bulletDestroyed){
                    if(enemiesTurret[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesTurret[idEnemy].getHP()<=0){
                            enemiesTurret.erase(enemiesTurret.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                //////Enemy Turret
                idEnemy=0;
                while(idEnemy < enemiesSeeker.size() && !bulletDestroyed){
                    if(enemiesSeeker[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesSeeker[idEnemy].getHP()<=0){
                            enemiesSeeker.erase(enemiesSeeker.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                //////Enemy Spawner
                idEnemy=0;
                while(idEnemy < enemiesSpawner.size() && !bulletDestroyed){
                    if(enemiesSpawner[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesSpawner[idEnemy].getHP()<=0){
                            enemiesSpawner.erase(enemiesSpawner.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                //////Enemy Sniper
                idEnemy=0;
                while(idEnemy < enemiesSniper.size() && !bulletDestroyed){
                    if(enemiesSniper[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesSniper[idEnemy].getHP()<=0){
                            enemiesSniper.erase(enemiesSniper.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
                //////Enemy Charger
                idEnemy=0;
                while(idEnemy < enemiesCharger.size() && !bulletDestroyed){
                    if(enemiesCharger[idEnemy].getShot(bullets[idBullet])){
                        bulletsToDelete.insert(idBullet);
                        bulletDestroyed = true;
                        if(enemiesCharger[idEnemy].getHP()<=0){
                            enemiesCharger.erase(enemiesCharger.begin()+idEnemy);
                        }
                    }
                    idEnemy++;
                }
            }

            ////If any bullet get out of the window -> destroy it
            if(!bulletDestroyed && (bullets[idBullet].getX()<0 || bullets[idBullet].getY()<0 || bullets[idBullet].getX()>displayX || bullets[idBullet].getY()>displayY)){
                    bulletsToDelete.insert(idBullet);
            }

            ////If any bullet is in a wall -> destroy it
            int idWall = 0;
            while(idWall < walls.size()&& !bulletDestroyed){
                if(walls[idWall].isInWall(bullets[idBullet].getX()+bulletRadius,bullets[idBullet].getY()+bulletRadius)){
                    bulletsToDelete.insert(idBullet);
                    bulletDestroyed = true;
                }
                else if(walls[idWall].isInWall(bullets[idBullet].getX()-bulletRadius,bullets[idBullet].getY()+bulletRadius)){
                    bulletsToDelete.insert(idBullet);
                    bulletDestroyed = true;
                }
                else if(walls[idWall].isInWall(bullets[idBullet].getX()-bulletRadius,bullets[idBullet].getY()-bulletRadius)){
                    bulletsToDelete.insert(idBullet);
                    bulletDestroyed = true;
                }
                else if(walls[idWall].isInWall(bullets[idBullet].getX()+bulletRadius,bullets[idBullet].getY()-bulletRadius)){
                    bulletsToDelete.insert(idBullet);
                    bulletDestroyed = true;
                }
                idWall ++;
            }
            idBullet++;
        }

        //Erase the bullets
        std::vector<int> bulletsToDeleteArray(bulletsToDelete.begin(), bulletsToDelete.end());
        for (int i = bulletsToDeleteArray.size() - 1; i >= 0; i--) {
            bullets.erase(bullets.begin() + bulletsToDeleteArray[i]);
        }

        //WALL
        for(Wall &wall : walls){
            wall.draw(window);
        }

        //ENEMIES
        ////EnemyShooter update
        for(EnemyShooter &enemy : enemiesShooter){
            float angleEnemyToPlayer = calcul_angle(enemy.getX(),enemy.getY(),player.getX(),player.getY());
            enemy.update(bullets, currentTime, angleEnemyToPlayer, walls);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        ////EnemyTurret update
        for(EnemyTurret &enemy : enemiesTurret){
            enemy.update(bullets, currentTime);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        ////EnemySpawner update
        for(EnemySpawner &enemy : enemiesSpawner){
            enemy.update(enemiesSeeker, currentTime);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        ////EnemySeeker update
        for(EnemySeeker &enemy : enemiesSeeker){
            float angleEnemyToPlayer = calcul_angle(enemy.getX(),enemy.getY(),player.getX(),player.getY());
            enemy.update(angleEnemyToPlayer, walls);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        ////EnemySniper update
        for(EnemySniper &enemy : enemiesSniper){
            double angleEnemyToPlayer = angle_shot_predict(player,enemy.getX(),enemy.getY());
            enemy.update(bullets, currentTime, angleEnemyToPlayer,walls);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        //EnemyCharger update
        for(EnemyCharger &enemy : enemiesCharger){
            float angleEnemyToPlayer = calcul_angle(enemy.getX(),enemy.getY(),player.getX(),player.getY());
            enemy.update(currentTime,angleEnemyToPlayer, walls);
            enemy.draw(window);

            player.getHit(enemy.getX(),enemy.getY());
        }

        player.drawHealth(window);

        window.display();
    }

    return 0;
}