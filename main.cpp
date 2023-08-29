#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <chrono>

const int displayX = 1000;
const int displayY = 1000;
const int hauteur = 24;
const int rayon = 19;
const int bulletRadius = 15;
const double PI = std::acos(-1);
sf::Color enemiesColor(200,200,200);
sf::Color enemiesBulletInvinsibleColor(255,102,51);
sf::Color enemiesBulletDestructibleColor(155,155,155);

double calcul_angle(int startX, int startY, int endX, int endY){
    return atan2(startY-endY,endX-startX);
}

class Player{
    public:
        float x = 0;
        float y = 0;
        float speed = 0.35;
        int hp = 10;
        double angle;
        std::chrono::time_point<std::chrono::steady_clock> shootTime;
};

class Bullet{
    public:
        Bullet(float x, float y, float angle, float speed, bool Ally, bool Destructible) : x(x), y(y), speed(speed), angle(angle), Ally(Ally), Destructible(Destructible){}

        void update(){
            x+=speed*cos(angle);
            y-=speed*sin(angle);
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

class EnemyShooter{
    public:
        EnemyShooter(float x, float y) : x(x), y(y), angle(PI+PI/2), speed(0.03), hp(5), shootTimer(0), timeBetweenShot(2000) {}

        void update(Player player,float targetAngle,std::vector<Bullet>& bullets, int timePassed){
            angle = targetAngle;

            x+=speed*cos(angle);
            y-=speed*sin(angle);

            //Shoot every N seconde
            shootTimer+=timePassed;
            if(shootTimer>=timeBetweenShot){
                bullets.emplace_back(x,y,angle,0.25,false,true);
                shootTimer = 0;
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

        int getHP() const{
            return hp;
        }

    private:
        float x,y,speed,angle;
        int hp,timeBetweenShot;
        double shootTimer;
};

class EnemyTurret{
    public:
        EnemyTurret(float x, float y) : x(x), y(y), angle(0), targetAngle(0), hp(7), shootTimer(0), timeBetweenShot(250) {}

        void update(std::vector<Bullet>& bullets, int timePassed){
            angle+=(targetAngle-angle)*0.02;

            //Shoot every 0.25 seconde
            shootTimer+=timePassed;
            if(shootTimer>=timeBetweenShot){
                bullets.emplace_back(x,y,angle,0.2,false,true);
                bullets.emplace_back(x,y,angle+PI/2,0.2,false,false);
                bullets.emplace_back(x,y,angle+PI,0.2,false,true);
                bullets.emplace_back(x,y,angle+PI+PI/2,0.2,false,false);
                targetAngle += PI/6;
                shootTimer = 0;
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

        int getHP() const{
            return hp;
        }

    private:
        float x,y,angle,targetAngle;
        int hp,timeBetweenShot;
        double shootTimer;
};

void playerShoot(Player& player, std::vector<Bullet>& bullets){
    int timePassed = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - player.shootTime)).count();
    if(timePassed >= 250){
            bullets.emplace_back(player.x,player.y,player.angle,0.75,true,false);
            player.shootTime = std::chrono::steady_clock::now();
    }
}

void drawPlayer(Player player, sf::RenderWindow& window){
    sf::VertexArray player_left_part(sf::Triangles,3);
    sf::VertexArray player_right_part(sf::Triangles,3);
    float angle_point_triangle_1 = atan2(hauteur,rayon);
    float angle_point_triangle_2 = atan2(hauteur,-rayon);
    float distance_point_triangle = sqrt(hauteur*hauteur+rayon*rayon);

    ////Define the coordonate of the player's point that are aline with the mouse
    float player_sprite_down_x = player.x-hauteur*0.6*cos(player.angle);
    float player_sprite_down_y = player.y+hauteur*0.6*sin(player.angle);

    float player_sprite_up_x = player.x+hauteur*cos(player.angle);
    float player_sprite_up_y = player.y-hauteur*sin(player.angle);

    float angle_triangle_left = angle_point_triangle_1+player.angle+PI/2;
    float player_sprite_left_x = player.x+distance_point_triangle*cos(angle_triangle_left);
    float player_sprite_left_y = player.y-distance_point_triangle*sin(angle_triangle_left);

    float angle_triangle_right = angle_point_triangle_2+player.angle+PI/2;
    float player_sprite_right_x = player.x+distance_point_triangle*cos(angle_triangle_right);
    float player_sprite_right_y = player.y-distance_point_triangle*sin(angle_triangle_right);

    ////Define the position of the left part points
    player_left_part[0].position = sf::Vector2f(player_sprite_left_x,player_sprite_left_y);
    player_left_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
    player_left_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);
        
    ////Define the color of the left part
    player_left_part[0].color = sf::Color::White;
    player_left_part[1].color = sf::Color::White;
    player_left_part[2].color = sf::Color::White;

    ////Define the position of right part points
    player_right_part[0].position = sf::Vector2f(player_sprite_right_x,player_sprite_right_y);
    player_right_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
    player_right_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);
        
    ////Define the color of the right part
    player_right_part[0].color = sf::Color::White;
    player_right_part[1].color = sf::Color::White;
    player_right_part[2].color = sf::Color::White;

    window.draw(player_left_part);
    window.draw(player_right_part);
}

void drawHealth(Player player, sf::RenderWindow& window){
    sf::CircleShape circle;
    circle.setRadius(15);
    circle.setFillColor(sf::Color::Red);
    for(int i=0;i<player.hp;i++){
        circle.setPosition(15+35*i,15);
        window.draw(circle);
    }
}

void drawEnemyShooter(EnemyShooter enemy, sf::RenderWindow& window){
    sf::VertexArray enemy_left_part(sf::Triangles,3);
    sf::VertexArray enemy_right_part(sf::Triangles,3);
    float angle_point_triangle_1 = atan2(hauteur,rayon);
    float angle_point_triangle_2 = atan2(hauteur,-rayon);
    float distance_point_triangle = sqrt(hauteur*hauteur+rayon*rayon);
    int x = enemy.getX();
    int y = enemy.getY();

    ////Define the coordonate of the enemy's point that are aline with the mouse
    float enemy_sprite_down_x = enemy.getX()-hauteur*0.6*cos(enemy.getAngle());
    float enemy_sprite_down_y = enemy.getY()+hauteur*0.6*sin(enemy.getAngle());

    float enemy_sprite_up_x = enemy.getX()+hauteur*cos(enemy.getAngle());
    float enemy_sprite_up_y = enemy.getY()-hauteur*sin(enemy.getAngle());

    float angle_triangle_left = angle_point_triangle_1+enemy.getAngle()+PI/2;
    float enemy_sprite_left_x = enemy.getX()+distance_point_triangle*cos(angle_triangle_left);
    float enemy_sprite_left_y = enemy.getY()-distance_point_triangle*sin(angle_triangle_left);

    float angle_triangle_right = angle_point_triangle_2+enemy.getAngle()+PI/2;
    float enemy_sprite_right_x = enemy.getX()+distance_point_triangle*cos(angle_triangle_right);
    float enemy_sprite_right_y = enemy.getY()-distance_point_triangle*sin(angle_triangle_right);

    ////Define the position of the left part points
    enemy_left_part[0].position = sf::Vector2f(enemy_sprite_left_x,enemy_sprite_left_y);
    enemy_left_part[1].position = sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_left_part[2].position = sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);
        
    ////Define the color of the left part
    enemy_left_part[0].color = enemiesColor; 
    enemy_left_part[1].color = enemiesColor;
    enemy_left_part[2].color = enemiesColor;

    ////Define the position of right part points
    enemy_right_part[0].position = sf::Vector2f(enemy_sprite_right_x,enemy_sprite_right_y);
    enemy_right_part[1].position = sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_right_part[2].position = sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);
        
    ////Define the color of the right part
    enemy_right_part[0].color = enemiesColor;
    enemy_right_part[1].color = enemiesColor;
    enemy_right_part[2].color = enemiesColor;

    window.draw(enemy_left_part);
    window.draw(enemy_right_part);
}

void drawEnemyTurret(EnemyTurret enemy, sf::RenderWindow& window){
    sf::VertexArray enemy_part(sf::Triangles,3);
    int radius = 30;
    enemy_part[0].color = enemiesColor;
    enemy_part[1].color = enemiesColor;
    enemy_part[2].color = enemiesColor;
    for(int i=0;i<8;i++){
        enemy_part[0].position = sf::Vector2f(enemy.getX(),enemy.getY());
        enemy_part[1].position = sf::Vector2f(enemy.getX()+radius*cos((i+1)*PI/4-enemy.getAngle()),enemy.getY()+radius*sin((i+1)*PI/4-enemy.getAngle()));
        enemy_part[2].position = sf::Vector2f(enemy.getX()+radius*cos((i+2)*PI/4-enemy.getAngle()),enemy.getY()+radius*sin((i+2)*PI/4-enemy.getAngle()));
        window.draw(enemy_part);
    }
}

void drawBullet(Bullet bullet, sf::RenderWindow& window){
    if(bullet.isAlly()){
        sf::VertexArray quad(sf::Quads, 4);
        quad[0].color = sf::Color::White;
        quad[1].color = sf::Color::White;
        quad[2].color = sf::Color::White;
        quad[2].color = sf::Color::White;

        quad[0].position = sf::Vector2f(bullet.getX()+20*cos(bullet.getAngle()-PI/8),bullet.getY()+20*sin(bullet.getAngle()-PI/8));
        quad[1].position = sf::Vector2f(bullet.getX()+20*cos(bullet.getAngle()+PI/8),bullet.getY()+20*sin(bullet.getAngle()+PI/8));
        quad[2].position = sf::Vector2f(bullet.getX()+20*cos(bullet.getAngle()+PI*7/8),bullet.getY()+20*sin(bullet.getAngle()+PI*7/8));
        quad[3].position = sf::Vector2f(bullet.getX()+20*cos(bullet.getAngle()+PI*9/8),bullet.getY()+20*sin(bullet.getAngle()+PI*9/8));
        window.draw(quad);
    }
    else{
        sf::CircleShape circle;
        circle.setRadius(bulletRadius);
        if(bullet.isDestructible()){
            circle.setFillColor(enemiesBulletDestructibleColor);
            }
        else{
            circle.setFillColor(enemiesBulletInvinsibleColor);
        }
        circle.setPosition(bullet.getX()-bulletRadius,bullet.getY()-bulletRadius);
        window.draw(circle);
    }
}


int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    Player player;
    player.shootTime = std::chrono::steady_clock::now();

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


    //Timer
    auto currentTime = std::chrono::steady_clock::now();
    int timePassed;
    while(window.isOpen()){

        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        //Time passed for each loop
        timePassed = (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - currentTime)).count();
        currentTime = std::chrono::steady_clock::now();

        //Player interaction with the game
        ////Look out if a key is pressed, if so move the triangle
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
            player.y -= player.speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            player.y += player.speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            player.x += player.speed;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
            player.x -= player.speed;
        }

        ////Define le angle between the mouse and the player
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        player.angle = calcul_angle(player.x,player.y,mousePosition.x,mousePosition.y);
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            playerShoot(player,bullets);
        }

        //DRAW THE SPRITE OF THE PLAYER
        drawPlayer(player,window);

        //DRAW HEALTH OF THE PLAYER
        drawHealth(player,window);

        ////Verify the angle of the player
        sf::VertexArray line(sf::Lines, 2);
        sf::Vector2f startPoint(mousePosition.x,mousePosition.y);
        sf::Vector2f endPoint(player.x,player.y);
        line[0].position = startPoint;
        line[1].position = endPoint;
        line[1].color = sf::Color::Red;
        window.draw(line);


        //BULLETS INTERACTION
        int iteration = 0;
        for(Bullet &bullet : bullets){
            bullet.update();

            //DRAW BULLETS
            drawBullet(bullet,window);

            //If the bullet touch the player HitBox: -1HP and destroy the bullet
            if(!bullet.isAlly()){
                int distance_between_bullet_player = sqrt((player.x-bullet.getX())*(player.x-bullet.getX()) + (player.y-bullet.getY())*(player.y-bullet.getY()));
                if(distance_between_bullet_player <= rayon/2+bulletRadius){
                    player.hp -= 1;
                    bullets.erase(bullets.begin()+iteration);
                }
            }
            else{
                int number_bullet=0;
                for(Bullet &bullet_collision : bullets){
                    if(!bullet_collision.isAlly() && bullet_collision.isDestructible()){
                        int diffX = bullet_collision.getX()-bullet.getX();
                        int diffY = bullet_collision.getY()-bullet.getY();
                        if(diffX>-bulletRadius && diffX<bulletRadius && diffY>-bulletRadius && diffY<bulletRadius){
                            bullets.erase(bullets.begin()+number_bullet);
                            bullets.erase(bullets.begin()+iteration);
                            iteration--;
                        }
                    }
                    number_bullet++;
                }
            }

            if(bullet.getX()<0|| bullet.getY()<0 || bullet.getX()>displayX-bulletRadius || bullet.getY()>displayY-bulletRadius){
                bullets.erase(bullets.begin()+iteration);
            }
            iteration++;
        }

        //ENEMIES INTERACTION
        ////EnemyShooter update
        for(EnemyShooter &enemy : enemiesShooter){
            float angleEnemyToPlayer = calcul_angle(enemy.getX(),enemy.getY(),player.x,player.y);
            enemy.update(player,angleEnemyToPlayer,bullets,timePassed);

            //DRAW ENEMY    
            drawEnemyShooter(enemy,window);

            ////Verify the angle between the enemy and the player
            /*sf::VertexArray line(sf::Lines, 2);
            sf::Vector2f startPoint(enemy.getX(),enemy.getY());
            sf::Vector2f endPoint(player.x,player.y);
            line[0].position = startPoint;
            line[1].position = endPoint;
            line[0].color = sf::Color::Green;
            window.draw(line);*/
        }

        ////EnemyTurret update
        for(EnemyTurret &enemy : enemiesTurret){
            enemy.update(bullets,timePassed);

            //DRAW ENEMY    
            drawEnemyTurret(enemy,window);
        }

        window.display();
    }

    return 0;
}