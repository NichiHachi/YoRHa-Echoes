all: main

main: main.cpp Bullet.o Player.o EnemyCharger.o EnemyShooter.o EnemySniper.o EnemySpawner.o EnemyTurret.o Wall.o
	g++ main.cpp Bullet.o Player.o EnemyCharger.o EnemyShooter.o EnemySniper.o EnemySpawner.o EnemyTurret.o Wall.o -o main -lsfml-graphics -lsfml-window -lsfml-system

Bullet.o: Bullet.cpp Bullet.hpp
	g++ -c Bullet.cpp

Wall.o: Wall.cpp Wall.hpp
	g++ -c Wall.cpp

Player.o: Player.cpp Player.hpp
	g++ -c Player.cpp

EnemyCharger.o: EnemyCharger.cpp EnemyCharger.hpp
	g++ -c EnemyCharger.cpp

EnemyShooter.o: EnemyShooter.cpp EnemyShooter.hpp
	g++ -c EnemyShooter.cpp

EnemySniper.o: EnemySniper.cpp EnemySniper.hpp
	g++ -c EnemySniper.cpp

EnemySpawner.o: EnemySpawner.cpp EnemySpawner.hpp
	g++ -c EnemySpawner.cpp

EnemyTurret.o: EnemyTurret.cpp EnemyTurret.hpp
	g++ -c EnemyTurret.cpp

clean:
	rm main
	rm *.o