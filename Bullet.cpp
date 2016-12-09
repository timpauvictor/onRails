/*
Computer Graphics 3GC3 Final Project: Target Smash

Cesar Antonio Santana Penner - 001411598
Juan Carlos Santana Penner - 001411625
Victor Timpau - 001414243
Jin Lee - 001417622
Date: December 9, 2016

Description - On rails shooter game created with c++ and openGL. This is the main file
*/
// Bullet Class
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

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <random>

#include "Bullet.h"
#include "basicMathLibrary.h"

//Class Empty Constructor
Bullet::Bullet(){
  x = 0;
  y = 0;
  z = 0;
}

//Class Constructor, All bullets will be made with this
Bullet::Bullet(point3D startingPoint, point3D targetPoint){  //constructor
  
  target = targetPoint;
  origin = startingPoint;   
  x = origin.x;
  y = origin.y;
  z = origin.z;

  active = true;
  shouldReset = false;
  direction =  direction.createVector(origin,target);
  realoadTime = 5;                       //how long it takes to reaload and shoot
  t = 0;                                 //time starts at 0
}

//Once Enemy is ready to shoot again it will reset the buller
void Bullet::resetBullet(point3D startingPoint, point3D targetPoint){  //constructor
  
  target = targetPoint;
  origin = startingPoint;
  x = origin.x;
  y = origin.y;
  z = origin.z;
  
  active = true;
  shouldReset = false;
  direction =  direction.createVector(origin,target);
  t = 0;

  //Generate a random number
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_int_distribution<> distr(1, 7); // define the range

  int randomTime;
  randomTime = distr(eng);
  //printf("randomTime: %i \n", randomTime);
  realoadTime = randomTime;
}

//Setting the target of where the bullet is supposed to go
void Bullet::setTarget(point3D p){
  target = p;       
  direction =  direction.createVector(origin,target);
  active = true;
  t = 0;
  shouldReset = false;
}

//Drawing the bullet
void Bullet::draw(){
  //float camPos0, float camPos2
    //safety percaution, this solves a bug where t goes negative sometimes
    if (t < 0){
      t = 0;
    }

    //Moving along the vector, where t represents time
    t += 0.05;                    //how fast the bullet will travel per frame
    x = origin.x + t*direction.x;
    y = origin.y + t*direction.y;
    z = origin.z + t*direction.z;

    //printf("t: %f\n", t);

    // if its time to reload 
    if (t > realoadTime){
      shouldReset = true;
      active = false;
    }else { 

      //Brass Meterial, Link can be found in readMe
      float m_amb[] = {0.329412, 0.223529, 0.027451, 1.0};
      float m_dif[] = {0.780392,0.568627, 0.113725, 1.0};
      float m_spec[] = {0.992157, 0.941176, 0.807843, 1.0};
      float shiny = 27.8974;

      //Apply material to buller
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
      glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

      //Draw Bullet
      glPushMatrix();
          glTranslated(x,y,z);
          glScalef(0.8,0.8,0.8);
          glScalef(1,0.2,1);
          glutSolidSphere(0.2,50,10);
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
    }
}

