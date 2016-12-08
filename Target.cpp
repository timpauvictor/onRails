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
#include "Target.h"

#define PI 3.14159265   //used for hit detection calculations


Target::Target(float xcord, float ycord, float zcord,float rad, float theScale){	//constructor
	//store the center of the cube
	x = xcord;
	y = ycord;
	z = zcord;
	rotX = 0;
	rotY = 0;
	rotZ = 0;
	alive = true;
	scale = theScale;
  radius = rad;
  //printf(" radius: %f \n", radius );
}

void Target::draw(float camPos0, float camPos2){
  //float camPos0, float camPos2
    glPushMatrix();
        glTranslated(x,y,z);

        glRotatef(rotY,0,1,0);
        glRotatef(rotX, 1, 0.0, 0.0);
       	glRotatef(rotZ,0,0,1);

        float h = camPos0 - x;
        float o = camPos2 - z;

        float angle = (atan(h/o)*180)/PI;
          //asin(0/h) * 180 / PI

        //printf("angle: %f , tX: %f , tZ: %f camX: %f , camZ: %f \n", angle, x,z, camPos0, camPos2);

         rotY = angle;


		    //glScalef(scale,scale,scale);

       	//glColor3f(0,1,0);
       //	glutWireSphere(radius,50,10);

        glColor3f(1,1,1);
        float m_amb[] = {0, 0.05, 0.05, 1.0};
        float m_dif[] = {0.4, 0.5, 0.5, 1.0};
        float m_spec[] = {0.04, 0.7, 0.7, 1.0};
        float shiny = 0.078125 *128;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
        glutSolidTorus(0.8*radius/4, 0.8*radius/4  +0.6*radius, 10, 50);
        
        glColor3f(0,0,1);
        
        m_amb[0] = 0;
        m_amb[1] = 0;
        m_amb[2] = 0;
        m_dif[0] = 0.55;
        m_dif[1] = 0.55;
        m_dif[2]= 0.55;
        m_spec[0] = 0.7;
        m_spec[1] = 0.7;
        m_spec[2]= 0.7; 
        shiny = 0.078125*128;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
        glutSolidTorus(1*radius/4,0.7*radius/4 + 0.4*radius,10,50);
        
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
        glutSolidSphere(radius/2.7,50,10);
        
        m_amb[0] = 0.1;
        m_amb[1] = 0.1;
        m_amb[2] = 0.1;
        m_dif[0] = 0.5;
        m_dif[1] = 0.5;
        m_dif[2]= 0.5;
        m_spec[0] = 0.99;
        m_spec[1] = 0.91;
        m_spec[2]= 0.81; 
        shiny = 10;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
        /*
       	glColor3f(1,1,1);
        glutSolidTorus(0.8*scale, 3*scale, 10, 50);
        glColor3f(0,0,1);
        glutSolidTorus(1*scale,2*scale,10,50);
        glColor3f(1,0,0);
        glutSolidTorus(0.8*scale,0.8*scale,10,50);
        */
    glPopMatrix();

}



