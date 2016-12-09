/*
Computer Graphics 3GC3 Final Project: Target Smash

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Victor Timpau - 001414243
Jin Lee - 001417622
Date: December 9, 2016

Description - On rails shooter game created with c++ and openGL. This is the main file
*/
#include "basicMathLibrary.h"

class Bullet{
public:
	Bullet(point3D origin, point3D target);	//constructor, this one is going to be used
	Bullet();								//back up constructor		
	bool active;
	bool shouldReset;						//used to reload, ie knowing when to reset the bullet

	void draw();							//draw the target
	void setTarget(point3D newTargetPoint);	//the target that the bullet is aiming for
	void resetBullet(point3D sPoint, point3D tPoint);	//reset the bullet, Used when you relaod and shoot
	
	//current x,y,z coordinates
	float x;
	float y;
	float z;

	point3D origin;				//where the bullet comes from
	point3D target;				//where the bullet is aiming for

	float t;					//time
	float realoadTime;			//how long it takes to "reload"

	vec3D direction;			//direction of the bullet
};
