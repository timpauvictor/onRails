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
  //printf("wait Time: %f \n",waitTime );
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

      /*
        Fixed BUG !
        if camera.z >= enemy.z then it stops working

      */
      if (camPos2 <= z){
        rotY += 180;
      }

      glRotatef(rotY,0,1,0);
      glRotatef(rotX, 1, 0.0, 0.0);
      glRotatef(rotZ,0,0,1);

      //hit Box
      //glColor3f(0,1,0);
      //glutWireSphere(radius,50,10);

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
        //glTranslatef(0,0,0);
        glutSolidSphere(1.2, 10, 50);
        //glutSolidTorus(0.3,0.8,10,50);
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
