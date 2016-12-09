/*
Computer Graphics 3GC3 Final Project: Target Smash

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Victor Timpau - 001414243
Jin Lee - 001417622
Date: December 9, 2016

Description - On rails shooter game created with c++ and openGL. This is the main file
*/
#include "Bullet.h"
//Enemy Class

class Enemy{
public:
	Enemy(float x, float y,float z, float enemyRadius, float enemyScale, float initTime,float popDir,float moveSpeed);	//constructor

	void draw(float camX, float camY, float camZ);				//draw the target
	void shoot(float camPos0, float camPos1, float camPos2);	//when the enemy shoots
	void waitActivate();										//enemy waiting to be activated

	float x,y,z;			//the center of the hitbox, enemy coordinates
	float radius;			//radius of target
	float scale;			//scaling the target
	float rotY;				//enemy rotation, about the y axis
	float rotX;				//enemy rotation, about the x axis
	float rotZ;				//enemy rotation, about the z axis
	Bullet bullet;			//Every enemy has a bullet

	bool active;			//Whether the enemy is active
	float waitTime;			//How long it takes for the enemy to pop out
	float activeTime;		//the current time of the enemy
	float moveTime;			//for how long the enemy will move for
	bool moved;				//whether the enemy has moved yet
	//MoveDir // 0 = y++, 1 x++, 2 y--, 3 = x--, 4 = z++, 5 = z--
	float moveDir;			//the direction in which the enemy will move once it is activated
	float moveSpeed;		//how fast the enemy will move
};
