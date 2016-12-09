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

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <iostream>
#include <random>

#include "Bullet.h"
#include "basicMathLibrary.h"

#define PI 3.14159265   //used for hit detection calculations


Bullet::Bullet(){
  x = 0;
  y = 0;
  z = 0;
}

Bullet::Bullet(point3D startingPoint, point3D targetPoint){  //constructor
  
  origin = startingPoint;
  x = origin.x;
  y = origin.y;
  z = origin.z;
  //targetPoint.y +=5;
  target = targetPoint;
  active = true;
  shouldReset = false;
  direction =  direction.createVector(origin,target);
  realoadTime = 5;
  t = 0;
}

void Bullet::resetBullet(point3D startingPoint, point3D targetPoint){  //constructor
  
  origin = startingPoint;
  x = origin.x;
  y = origin.y;
  z = origin.z;
  //targetPoint.y += 5;
  target = targetPoint;
  active = true;
  shouldReset = false;
  direction =  direction.createVector(origin,target);
  t = 0;


  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_int_distribution<> distr(1, 7); // define the range

  int randomTime;
  /* initialize random seed: */
  //srand (time(NULL));
  //randomTime = rand() % 7 + 1;  // generate random number betwee 7 and 1
  randomTime = distr(eng);
  //printf("randomTime: %i \n", randomTime);
  realoadTime = randomTime;
}

void Bullet::setTarget(point3D p){
  target = p;
  direction =  direction.createVector(origin,target);
  active = true;
  t = 0;
  shouldReset = false;
}

void Bullet::draw(){
  //float camPos0, float camPos2
    //safety percaution, this solves a bug where t goes negative sometimes
    if (t < 0){
      t = 0;
    }

    t += 0.05;                    //how fast the bullet will travel per frame
    x = origin.x + t*direction.x;
    y = origin.y + t*direction.y;
    z = origin.z + t*direction.z;

    //printf("t: %f\n", t);

    if (t > realoadTime){
      shouldReset = true;
      active = false;

    }else { 

      //Draw Bullet

      glPushMatrix();
          glTranslated(x,y,z);
          //glRotatef(45,0,1,0);
          glScalef(0.8,0.8,0.8);
          glColor3f(1,0,0);
          glScalef(0.5,0.2,0.5);
          glutSolidSphere(1,50,10);
      glPopMatrix();

    }

    //update x, y,z?
}

