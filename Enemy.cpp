/*
Computer Graphics 3GC3 Final Project: Target Smash

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Victor Timpau - 001414243
Jin Lee - 001417622
Date: December 9, 2016

Description - On rails shooter game created with c++ and openGL. This is the main file
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

/*
Enemy Constructor
x,y,z, enemy radius, scale (** Keep at 0.2), waitTime, popup Direction, movement speed
*/
Enemy::Enemy(float xcord, float ycord, float zcord,float rad, float theScale, float initTime,float popDir, float speed){	//constructor
	//store the center of the cube
	x = xcord;
	y = ycord;
	z = zcord;
  //enemy rotation
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	active = false;           //enemies start off in active
  moved = false;            //for when the enemy has finished poping up

	scale = theScale;
  radius = 5.8*rad*scale;   //the new radius

  point3D p(x,y,z);
  point3D p1(x+1,y,z);
  Bullet bullet(p,p1);      //make buller

  waitTime = initTime;
  //printf("wait Time: %f \n",waitTime );
  activeTime = 0;
  moveTime = 1.0;
  moveDir = popDir;       // 0 = up, 1, right, 2 down, 3 left, 4 "backward", 5 forward
  moveSpeed = speed;
}

//When the eneym shoots
void Enemy::shoot(float camPos0, float camPos1, float camPos2){

  //When its time to shoot again
  if (bullet.shouldReset){
    point3D origin(x,y,z);
    point3D p(camPos0,camPos1,camPos2);
    bullet.resetBullet(origin ,p);
    bullet.setTarget(p);
  }

  //if the bullet is active
  if (bullet.active){
    bullet.draw();
    //printf("bullet x: %f , y: %f , z: %f \n",bullet.x,bullet.y,bullet.z );
  }else {
    //printf("bullet Is not Active \n");
    bullet.active = true;
  }
  
}

//When the enemy is waiting to be activated
void Enemy::waitActivate(){

  activeTime += 0.05;         //increase time
  float totalTime = waitTime +moveTime; 

  //Waiting to be activated, once the time > waitTime the enemy will be activated
  if (activeTime > waitTime){
    //need to start moving up
    active = true;
  }

  //Once it is activated it will move in one of the directions
  if (active == true){
    //move robot up until you reach waitTime + moveUpTime
    if (moveDir == 0){
      y += moveSpeed;
    }else if (moveDir == 1){
      x += moveSpeed;
    }else if (moveDir == 2){
      y -= moveSpeed;
    }else if (moveDir == 3){
      x -= moveSpeed;
    }else if (moveDir == 4) {;
      z += moveSpeed;
    }else if (moveDir == 5) {
      z -= moveSpeed;
    }
    
    //Keep moving up until you have reach the waitTime +moveTime
    if (activeTime > totalTime){
      moved = true;
    }
  }
}

void Enemy::draw(float camPos0, float camPos1, float camPos2){

  //Once the enemy has already been activated and finished moving
  if ((active == true) && (moved == true)){
    shoot(camPos0,camPos1,camPos2);    
  }else {
    //else keep waiting to be activated
    waitActivate();
  }
  
  //Draw Enemy
  glPushMatrix();
     glTranslated(x,y,z);

      //Make it so that the enemy is always facing you, in terms of 
      float a = camPos0 - x;
      float o;
      o = camPos2 - z;
      float angle = (atan(a/o)*180)/PI;   //variable a should be var o, and o should be a , sorry :P
      //atan(a/0) * 180 / PI
      rotY = angle;
      /*
        Fixed BUG !?
        if camera.z >= enemy.z then it stops working */
      if (camPos2 <= z){
        rotY += 180;
      }

      //Rotate enemy
      glRotatef(rotY,0,1,0);
      glRotatef(rotX, 1, 0.0, 0.0);
      glRotatef(rotZ,0,0,1);

      //Used for testing      
      //hit Box
      //glColor3f(0,1,0);
      //glutWireSphere(radius,50,10);
      
      //Materials      
      float m_amb[] = {0.2, 0.2, 0.2, 1.0};
      float m_dif[] = {0.0, 0.0, 5, 1.0};
      float m_spec[] = {0.04, 0.7, 0.7, 1.0};
      float shiny = 0.078125 *128;

      //Obsidian Material
      m_amb[0] = 0.05375;
      m_amb[1] = 0.05;
      m_amb[2] = 0.06625;
      m_amb[3] = 0.82;
      m_dif[0] = 0.18275;
      m_dif[1] = 0.17;
      m_dif[2]= 0.22525;
      m_dif[3] = 0.82;
      m_spec[0] = 0.332741;
      m_spec[1] = 0.328634;
      m_spec[2]= 0.508273; 
      m_spec[3] = 0.346435;
      shiny = 38.4;
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

      //Main Gun
      glPushMatrix();
      //Torus
        glTranslatef(0,0,1);
        glutSolidTorus(0.2,0.4,50,50);
      glPopMatrix();
      
      //Chrome Material
      m_amb[0] = 0.25;
      m_amb[1] = 0.25;
      m_amb[2] = 0.25;
      m_dif[0] = 0.4;
      m_dif[1] = 0.4;
      m_dif[2]= 0.4;
      m_spec[0] = 0.774597;
      m_spec[1] = 0.774597;
      m_spec[2]= 0.774597; 
      shiny = 76.8;
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

      //Body
      glPushMatrix();
        glutSolidSphere(1.2, 10, 50);
      glPopMatrix();

      //RED Material
      m_amb[0] = 0.1745;
      m_amb[1] = 0.01175;
      m_amb[2] = 0.01175;
      m_dif[0] = 0.61424;
      m_dif[1] = 0.04316;
      m_dif[2]= 0.04136;
      m_spec[0] = 0.727811;
      m_spec[1] = 0.626959;
      m_spec[2]= 0.626959; 
      shiny = 0.6 * 128;
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

      //Center Shooting Part
      glPushMatrix();
        glTranslatef(0,0,1);
        glColor3f(1,0,0);
        glutSolidSphere(0.3,10,100);
      glPopMatrix();
      
      //Obsidian Material
      m_amb[0] = 0.05375;
      m_amb[1] = 0.05;
      m_amb[2] = 0.06625;
      m_amb[3] = 0.82;
      m_dif[0] = 0.18275;
      m_dif[1] = 0.17;
      m_dif[2]= 0.22525;
      m_dif[3] = 0.82;
      m_spec[0] = 0.332741;
      m_spec[1] = 0.328634;
      m_spec[2]= 0.508273; 
      m_spec[3] = 0.346435;
      shiny = 38.4;
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

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
  
      //reset Material
      m_amb[0] = 0.1;
      m_amb[1] = 0.1;
      m_amb[2] = 0.1;
      m_amb[3] = 1;
      m_dif[0] = 0.5;
      m_dif[1] = 0.5;
      m_dif[2]= 0.5;
      m_dif[3] = 1;
      m_spec[0] = 0.99;
      m_spec[1] = 0.91;
      m_spec[2]= 0.81; 
      m_spec[3] = 1;
      shiny = 10;
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

  glPopMatrix();
      
  glutPostRedisplay();
}
