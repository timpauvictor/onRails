/*
Computer Graphics 3GC3 Assignment 3: 3D Modeller (Group project)

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Date: December 1, 2016

Description - Hitbox class
*/
#include "Bullet.h"
//#include "basicMathLibrary.h"

class Enemy{
public:
	Enemy(float x, float y,float z, float enemyRadius, float enemyScale, float initTime,float popDir,float moveSpeed);	//constructor

	void draw(float camX, float camY, float camZ);			//draw the target
	void shoot(float camPos0, float camPos1, float camPos2);
	void waitActivate();

	float x,y,z;			//the center of the hitbox
	float radius;			//radius of target
	float scale;			//
	float rotY;
	float rotX;
	float rotZ;
	Bullet bullet;

	bool active;
	float waitTime;
	float activeTime;
	float moveTime;
	bool moved;
	float moveDir;
	float moveSpeed;

//private:
	//float privateVar;
};
