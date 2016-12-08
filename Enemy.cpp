/*
Computer Graphics 3GC3 Assignment 3: 3D Modeller (Group project)

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Date: December 1, 2016

Description - Hitbox class
*/

#include <stdio.h>    //printf command
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h> 
#include "Enemy.h"
//#include "Bullet.h"

#define PI 3.14159265   //used for hit detection calculations


Enemy::Enemy(float xcord, float ycord, float zcord,float rad, float theScale, float initTime,float popDir, float speed){	//constructor
	//store the center of the cube
	x = xcord;
	y = ycord;
	z = zcord;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	active = false;           //enemies start off in active
  moved = false;          //for when the enemy has finished poping up

	scale = theScale;
  radius = 5.8*rad*scale;

  point3D p(x,y,z);
  point3D p1(x+1,y,z);
  Bullet bullet(p,p1);

  waitTime = initTime;
  printf("wait Time: %f \n",waitTime );
  activeTime = 0;
  moveTime = 1.0;
  moveDir = popDir;       // 0 = up, 1, right, 2 down, 3 left, 4 "backward", 5 forward
  moveSpeed = speed;

  //printf(" radius: %f \n", radius );
}


void Enemy::shoot(float camPos0, float camPos1, float camPos2){

  if (bullet.shouldReset){
    point3D origin(x,y,z);
    point3D p(camPos0,camPos1,camPos2);
    bullet.resetBullet(origin ,p);
    bullet.setTarget(p);
  }

  if (bullet.active){
    bullet.draw();
    //printf("bullet x: %f , y: %f , z: %f \n",bullet.x,bullet.y,bullet.z );
  }else {
    //printf("bullet Is not Active \n");
    bullet.active = true;
  }
  
}

void Enemy::waitActivate(){
  activeTime += 0.05;
  //printf("active Time: %f \n", activeTime);
  float totalTime = waitTime +moveTime; 
  //printf("wait + moveUpTime: %f \n", totalTime );

  if (activeTime > waitTime){
    //need to start moving up
    active = true;
  }

  if (active == true){
    //move robot up until you reach waitTime + moveUpTime
    if (moveDir == 0){
      //printf("move y++ \n");
      y += moveSpeed;
    }else if (moveDir == 1){
      //printf("moveR x++ \n");
      x += moveSpeed;
    }else if (moveDir == 2){
      //printf("move y-- \n");
      y -= moveSpeed;
    }else if (moveDir == 3){
      //printf("move x -- \n");
      x -= moveSpeed;
    }else if (moveDir == 4) {
      //printf("move back z++ \n");
      z += moveSpeed;
    }else if (moveDir == 5) {
      //printf("move closer z-- \n");
      z -= moveSpeed;
    }
    
    if (activeTime > totalTime){
      moved = true;
    }

  }
}

void Enemy::draw(float camPos0, float camPos1, float camPos2){

  if ((active == true) && (moved == true)){
    shoot(camPos0,camPos1,camPos2);    
    //printf("shoot \n");
  }else {
    //else keep waiting to be activated
    waitActivate();
  }
  
  //float camPos0, float camPos2
    glPushMatrix();
       glTranslated(x,y,z);

        float a = camPos0 - x;
        float o;
        o = camPos2 - z;
        ////
        /*
        Fix BUG
        if camera.z >= enemy.z then it stops working

        */
        float angle = (atan(a/o)*180)/PI;
        //atan(0/h) * 180 / PI

        //printf("angle: %f , tX: %f , tZ: %f camX: %f , camZ: %f \n", angle, x,z, camPos0, camPos2);
       rotY = angle;

        glRotatef(rotY,0,1,0);
        glRotatef(rotX, 1, 0.0, 0.0);
        glRotatef(rotZ,0,0,1);

        //hit Box
        glColor3f(0,1,0);
        glutWireSphere(radius,50,10);

        glPushMatrix();
        //Torus
          glScalef(0.7,0.7,0.7);
          glColor3f(0,0,1);
          glutSolidTorus(0.3,0.8,10,50);
        glPopMatrix();
        
        //Body
        glPushMatrix();
          //glTranslatef(0,0,0);

          glColor3f(0,0.5,0.5);
          glutSolidSphere(1.2, 10, 50);
          //glutSolidTorus(0.3,0.8,10,50);
        glPopMatrix();
        
        //Center Shooting Part
        glPushMatrix();
          glTranslatef(0,0,0.3);
          glColor3f(1,0,0);
          glutSolidSphere(0.7,10,100);
        glPopMatrix();
        
        //left shoulder Shooter
        glPushMatrix();
          glTranslatef(0.9,0.7,1);
          glScalef(0.5,0.5,2);
          glColor3f(0,1,0);
          glutSolidCube(1);
        glPopMatrix();

        //right shoulder Shooter
        glPushMatrix();
          glTranslatef(-0.9,0.7,1);
          glScalef(0.5,0.5,2);
          glColor3f(0,1,0);
          glutSolidCube(1);
        glPopMatrix();  
        
    glPopMatrix();
        

    glutPostRedisplay();
}
