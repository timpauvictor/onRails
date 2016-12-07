/*
Computer Graphics 3GC3 Assignment 3: 3D Modeller (Group project)

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Date: December 1, 2016

Description - Hitbox class
*/

class Target{
public:
	Target(float x, float y,float z, float targetRadius, float targetScale);	//constructor

	void draw(float camX, float camZ);			//draw the target

	bool alive;

	float x,y,z;			//the center of the hitbox
	float radius;			//radius of target
	float scale;			//
	float rotY;
	float rotX;
	float rotZ;

//private:
	//float privateVar;
};
