/*
Computer Graphics 3GC3 Final Project: Target Smash

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Victor Timpau - 001414243
Jin Lee - 001417622
Date: December 9, 2016

Description - On rails shooter game created with c++ and openGL. This is the main file
*/

class Target{
public:
	Target(float x, float y,float z, float targetRadius, float targetScale);	//constructor

	void draw(float camX, float camZ);			//draw the target

	bool alive;

	float x,y,z;			//the center of the hitbox
	float radius;			//radius of target
	float scale;			//scale of the target
	//Target Rotation
	float rotY;			
	float rotX;
	float rotZ;
};
