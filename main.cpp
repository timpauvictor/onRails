#include <stdio.h>
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
//our includes
#include "basicMathLibrary.h" 		//for 3D points and Vectors
#include "basicShapeLibrary.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include "Target.h"						//Target Objects
#include "Enemy.h"
#include "objLoad.h";
#include <math.h> 	

using namespace std; 

#define PI 3.14159265		//used for hit detection calculations

//vars to save mouse x/y coord
int mouseX = 0, mouseY = 0;

Object deer("car.obj");

float light_pos[] = {5,10,5,1};
/*** CAMERA VARIABLES***/
vector<point3D*> *cameraPos = new vector<point3D*>;
vector<point3D*> *lookAtPos = new vector<point3D*>;
vector<int> *stages = new vector<int>;
int stageNumber = 0;
int frameCounter = 0;
float cameraHeight = 2.5;
bool first = true;
int lookAtIndex = 0;
int cameraIndex = 0;
int cameraPosSize = 0;

/*** SCORE AND TIMER VARIABELS ***/
int health = 3;
int score = 0;
int elapsedTime;
int timeAtReset = 0;
int timeToReset = 0;
int numOfResets = 1;
bool firstTime = true;
int time1 = 0;
int timeIncr =0;
int oldTime=0;
bool gameOver = false;

/*** Clearing stage variable***/
bool isLevelCleared = false;


float pos[] = {0,1,0};
float camPos[] = {5, 5, 10};
float angle = 0.0f;

/*** AMMO VARIABLES***/
int ammo = 6;
string reloading = "Reloading...";
int startTime;
int endTime;
bool isTimed = true;
bool isReloading = false;
int timeReloadCounter = 50;

/*** HUD Images***/
GLubyte *healthBar_image;
int height = 0;
int width = 0;
int max2 = 0;
GLubyte *ammo_image;
int height2 = 0;
int width2 = 0;
int max3 = 0;
GLubyte *restart_image;
int height3 = 0;
int width3 = 0;
int max4 = 0;
GLubyte *images[12];

/*** Textures ***/
GLubyte *crate_tex;
int height5 = 0;
int width5 = 0;
int max6 = 0;
GLubyte *stone_tex;
int height4 = 0;
int width4 = 0;
int max5 = 0;
GLubyte *steel_tex;
int height6 = 0;
int width6 = 0;
int max7 = 0;
GLubyte *flameCrate_tex;
int height7 = 0;
int width7 = 0;
int max8 = 0;
GLubyte *steelCrate_tex;
int height8 = 0;
int width8 = 0;
int max9 = 0;
GLubyte *modernCrate_tex;
int height9 = 0;
int width9 = 0;
int max10 = 0;
GLuint textures[6];
Shape s;

/*** FOG ***/
float density = 0.1; //fog density
float fogColor[4] = {0.5, 0.5, 0.5, 1};

//FLOOR
int mapSize=100;
vector<vector<float> > myfloor(mapSize,vector<float>(mapSize));

//Crouching
string crouching = "CROUCHING";

int side = 0;
int up = 0;
float ang = 0.0f;

float playerX;
float playerY;
float playerZ;
float playerRadius = 2;

/*** ENEMIES AND TARGET LISTS ***/
vector<Target> targetList;
vector<vector<Target>> targetInfo;

vector<Enemy> enemyList;
vector<vector<Enemy>> enemyInfo;

//Carlos will comment
/*
void drawPlayer(){
	glPushMatrix();
		glTranslatef(playerX,playerY,playerZ);
	  	glColor3f(0,1,0);
        glutWireSphere(playerRadius,50,10);
    glPopMatrix();
}
*/

//Splits string by a delimiter
void split(const string &s, char delim, vector<string> &elems){
	stringstream ss;
	ss.str(s);
	string item;
	while(getline(ss, item, delim)){
		elems.push_back(item);
	}
}

//splits the string and returns as a vector
vector<string> split(const string &s, char delim){
	vector<string> elems;
	split(s,delim, elems);
	return elems;
}

//Carlos will comment
void checkPlayerHit(){
	//Iterate through all bullets to see if the player got hit

	//Array Of all enemies
	for (int j = 0; j < enemyList.size(); j++){
		float x = enemyList[j].bullet.x;
		float y = enemyList[j].bullet.y;
		float z = enemyList[j].bullet.z;

		playerX = cameraPos->at(cameraIndex)->x;
		playerY = cameraPos->at(cameraIndex)->y;
		playerZ = cameraPos->at(cameraIndex)->z;

			
		if (x < playerX + 2 && x > playerX -2){
			if (z < playerZ + 2 && z > playerZ -2){
				if (y < playerY + 2 && y > playerY -2){
					
					if (cameraHeight == 2.5){
						// /printf("Hit PLAYER x: %f, y: %f , z: %f \n", x,y,z );
						point3D origin(enemyList[j].x,enemyList[j].y,enemyList[j].z);
	    				point3D p(cameraPos->at(cameraIndex)->x,cameraPos->at(cameraIndex)->y+1,cameraPos->at(cameraIndex)->z);
						enemyList[j].bullet.resetBullet(origin, p);

						health--;
					}
				}
			}
		}	
	}
}



//Initialize Target, and target positions
void createTargetList(){
	//printf("  %f %f %f %f %f \n",targetInfo[i][j].x,targetInfo[i][j].y,targetInfo[i][j].z,targetInfo[i][j].radius,targetInfo[i][j].scale );
	printf("Number of Targets in this stage: %i \n",targetInfo[stageNumber].size());
	for (int j = 0; j < targetInfo[stageNumber].size(); j++){
		printf("  %f %f %f %f %f \n",targetInfo[stageNumber][j].x,targetInfo[stageNumber][j].y,targetInfo[stageNumber][j].z,targetInfo[stageNumber][j].radius,targetInfo[stageNumber][j].scale );
		Target t(targetInfo[stageNumber][j].x,targetInfo[stageNumber][j].y,targetInfo[stageNumber][j].z,targetInfo[stageNumber][j].radius,targetInfo[stageNumber][j].scale);
		targetList.push_back(t);
	}
}

//Initialize Target, and target positions
void createEnemyList(){
	//printf("  %f %f %f %f %f \n",targetInfo[i][j].x,targetInfo[i][j].y,targetInfo[i][j].z,targetInfo[i][j].radius,targetInfo[i][j].scale );
	//printf("Number of Targets in this stage: %i \n",targetInfo[stageNumber].size());
	for (int j = 0; j < enemyInfo[stageNumber].size(); j++){
		//printf("  %f %f %f %f %f \n",targetInfo[stageNumber][j].x,targetInfo[stageNumber][j].y,targetInfo[stageNumber][j].z,targetInfo[stageNumber][j].radius,targetInfo[stageNumber][j].scale );
		Enemy e(enemyInfo[stageNumber][j].x,enemyInfo[stageNumber][j].y,enemyInfo[stageNumber][j].z,enemyInfo[stageNumber][j].radius,enemyInfo[stageNumber][j].scale,enemyInfo[stageNumber][j].waitTime,enemyInfo[stageNumber][j].moveDir,enemyInfo[stageNumber][j].moveSpeed);
		enemyList.push_back(e);
	}
}

//insert a point into the cameraPos vector
void insertPoint3D(point3D *p){
	cameraPos->push_back(p);
}

/*Insert a point into the lookAt vector
  For each stage, there is a different point where the camera will be looking at	
*/
void insertPoint3DLookAt(point3D *p){
	lookAtPos->push_back(p);
}


vector<vec3D> getRay(){
	//construct Ray
	GLdouble R0[3], R1[3], Rd[3];
	GLdouble modelMat[16], projMat[16];
	GLint viewMat[4];

	//populate mpv matricies
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMat);
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	glGetIntegerv(GL_VIEWPORT, viewMat);

	//calculate near point
	gluUnProject(mouseX, mouseY, 0.0, modelMat, projMat, viewMat, &R0[0], &R0[1], &R0[2]);
	//calculate far point
	gluUnProject(mouseX, mouseY, 1.0, modelMat, projMat, viewMat, &R1[0], &R1[1], &R1[2]);

	//calcualte our ray from R0 and R1
	Rd[0] = R1[0] - R0[0];
	Rd[1] = R1[1] - R0[1];
	Rd[2] = R1[2] - R0[2];

	//turn ray Rd into unit ray 
	GLdouble m = sqrt(Rd[0]*Rd[0] + Rd[1]*Rd[1] + Rd[2]*Rd[2]);
	Rd[0] /= m;
	Rd[1] /= m;
	Rd[2] /= m;

	//printf("R0: %f, %f, %f | \n", R0[0], R0[1], R0[2]);
	//printf("R1: %f, %f, %f | \n", R1[0], R1[1], R1[2]);
	//printf("Rd: %f, %f, %f | \n", Rd[0], Rd[1], Rd[2]);

	vec3D vecRD(Rd[0],Rd[1],Rd[2]);		//the returning vector, vetor Rd
	vec3D vecR0(R0[0],R0[1],R0[2]);		//the returning vector, vetor Rd

	vector<vec3D> vector(2);
	vector[0] = vecR0;
	vector[1] = vecRD;

	return vector;
}

//Sphere Intersections
bool targetTest(vec3D Rd, vec3D R0, Target t){

	//At^2 + Bt + C = 0
	//A = Rd dot Rd
	//B = 2( (R0 - Pc) dot (Rd) )
	//C = ( (R0 -Pc) dot (R0 - Pc)) - r^2
	vec3D Pc(t.x,t.y,t.z);

	float A = Rd.dot(Rd);
	vec3D temp = (R0.subtract(Pc));
	float B = 2* (temp.dot(Rd));
	float C = (temp.dot(temp)) - (t.radius*t.radius);

	//check discriminant(ie d), discriminant = b^2 - 4ac
	float d = B*B - 4*A*C;

	//if d < 0 then no instersection.
	//if d = 0 ray is tangent to sphere
	//if d > 0 ray intersects sphere in two points

	if (d < 0 ){
		printf("You missed the sphere! \n");
		return false;
	}else if (d > 0){

		//if intersections, to find point of intersection
		//t = quadratic formula 
		// x = R0.x + t*Rd.x
		// y = R0.y + t*Rd.y
		// x = R0.z + t*Rd.z

		float t0 = (-1*B  + sqrt(d))/(2*A);
		//float t1 = (-1*B  - sqrt(d))/(2*A);

		//Point P
		float P[3];
		P[0] = R0.x + t0*Rd.x;
		P[1] = R0.y + t0*Rd.y;
		P[2] = R0.z + t0*Rd.z;

		//printf("Target hit! x: %f, y: %f , z: %f \n", t.x,t.y,t.z );
		printf("Hit at: x: %f , y: %f , z: %f \n", P[0], P[1], P[2]);
		return true;
	}
	return false;

}



//Checks Intersections with all Target Objects
void targetIntersections(vec3D Rd, vec3D R0){
	
	vector<int> targetHits;			//the int will be like the id of the targets that are hit
									//the id is the index in the TargetList Vector Array
	for (int i = 0; i < targetList.size(); i++){
		bool hit = targetTest(Rd,R0,targetList[i]);
		if (hit == true){
			score+=5;
			targetHits.push_back(i);
		}
	}

	//delete hit objects
	for (int i = 0 ; i<targetHits.size(); i++){
		//printf("Target id: %i \n", targetHits[i]);
		targetList.erase( targetList.begin() + targetHits[i], targetList.begin() + targetHits[i]+1  );
		printf("Hit & Deleted Target id: %i \n", targetHits[i]);
	}

}

bool enemyHitTest(vec3D Rd, vec3D R0, Enemy e){
	//At^2 + Bt + C = 0
	//A = Rd dot Rd
	//B = 2( (R0 - Pc) dot (Rd) )
	//C = ( (R0 -Pc) dot (R0 - Pc)) - r^2
	vec3D Pc(e.x,e.y,e.z);

	float A = Rd.dot(Rd);
	vec3D temp = (R0.subtract(Pc));
	float B = 2* (temp.dot(Rd));
	float C = (temp.dot(temp)) - (e.radius*e.radius);

	//check discriminant(ie d), discriminant = b^2 - 4ac
	float d = B*B - 4*A*C;

	//if d < 0 then no instersection.
	//if d = 0 ray is tangent to sphere
	//if d > 0 ray intersects sphere in two points

	if (d < 0 ){
		//printf("You missed the sphere! \n");
		return false;
	}else if (d > 0){

		//if intersections, to find point of intersection
		//t = quadratic formula 
		// x = R0.x + t*Rd.x
		// y = R0.y + t*Rd.y
		// x = R0.z + t*Rd.z

		float t0 = (-1*B  + sqrt(d))/(2*A);
		//float t1 = (-1*B  - sqrt(d))/(2*A);

		//Point P
		float P[3];
		P[0] = R0.x + t0*Rd.x;
		P[1] = R0.y + t0*Rd.y;
		P[2] = R0.z + t0*Rd.z;

		//printf("Target hit! x: %f, y: %f , z: %f \n", t.x,t.y,t.z );
		//printf("Hit at: x: %f , y: %f , z: %f \n", P[0], P[1], P[2]);
		return true;
	}
	return false;
}


//Checks Intersections with all Target Objects
void enemyIntersections(vec3D Rd, vec3D R0){
	
	vector<int> enemyHits;			//the int will be like the id of the targets that are hit
									//the id is the index in the TargetList Vector Array
	for (int i = 0; i < enemyList.size(); i++){
		bool hit = enemyHitTest(Rd,R0,enemyList[i]);
		if (hit == true){
			enemyHits.push_back(i);
		}
	}

	//delete hit objects
	for (int i = 0 ; i<enemyHits.size(); i++){
		//printf("Target id: %i \n", targetHits[i]);
		enemyList.erase( enemyList.begin() + enemyHits[i], enemyList.begin() + enemyHits[i]+1  );
		printf("Hit & Deleted enemy id: %i \n", enemyHits[i]);
	}

}

//Draw all the text onto the screen such as Score and time
void DrawText(){
	glPushMatrix();
		glLoadIdentity();
		//calculate time
		glDisable(GL_LIGHTING);
		time1 =120-((elapsedTime)/250) + timeToReset;
		if(time1+timeIncr == 0){
			gameOver = true;
		}
		if(gameOver == false){
			if(isLevelCleared == false){
				string str;
				if((time1+timeIncr) > 0){
					str = to_string(time1+timeIncr);
				}else{
					str = "0";
				}

				
				glColor3f(1,1,1);

				//Draw Time
				glTranslatef(370,730,0);
				glScalef(0.5,0.5,1);
				glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());

				//Draw Score
				glLoadIdentity();
				str = "Score: " + to_string(score);
				glTranslatef(10,775,0);
				glScalef(0.20,0.20,1);
				glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());

				if(isReloading == true){
					//Draw Reloading
					glLoadIdentity();
					glTranslatef(315,400,0);
					glScalef(0.25,0.20,1);
					glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)reloading.c_str());
				}

				if(cameraHeight == 1.5){
					//Draw Crouhcing status
					glLoadIdentity();
					glTranslatef(315,25,0);
					glScalef(0.25,0.20,1);
					glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)crouching.c_str());
				}
			}else{
				if(firstTime == true){
					oldTime = time1+timeIncr;
					//timeAtReset = glutGet(GLUT_ELAPSED_TIME);
					firstTime = false;
				}


				string str;
				str = "Score: " + to_string(score);
				glColor3f(1,1,1);
				glTranslatef(320,500,0);
				glScalef(0.35,0.25,1);
				glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());

				glLoadIdentity();
				int bonus = (oldTime-10) *2;
				str = "Time Bonus: " + to_string((oldTime)) + " x 2" ;
				glColor3f(1,1,1);
				glTranslatef(220,450,0);
				glScalef(0.35,0.25,1);
				glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());

				glLoadIdentity();
				str = "Total Score: " + to_string(score+bonus) ;
				glColor3f(1,1,1);
				glTranslatef(250,400,0);
				glScalef(0.35,0.25,1);
				glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());
			
			}
	}else{
		string str;
		glLoadIdentity();
		str = "GAME OVER";
		glColor3f(1,1,1);
		glTranslatef(250,400,0);
		glScalef(0.35,0.25,1);
		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str.c_str());
	}

	glEnable(GL_LIGHTING);
	glPopMatrix();

}

//Draw the HUD
void DrawHUD(){
	glMatrixMode(GL_PROJECTION);
	glDisable(GL_FOG); 
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 800);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(800,800-height);
	glPixelZoom(-1, 1);
	glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, healthBar_image);

	glLoadIdentity();
	glRasterPos2i(800,800-height-height2);
	glPixelZoom(-1, 1);
	glDrawPixels(width2,height2,GL_RGB, GL_UNSIGNED_BYTE, ammo_image);


	if(isLevelCleared == true || gameOver == true){
		glLoadIdentity();
		glRasterPos2i(500,300);
		glPixelZoom(-1, 1);
		glDrawPixels(width3,height3,GL_RGB, GL_UNSIGNED_BYTE, restart_image);
	}
	glFlush(); 
	glEnable(GL_FOG); 
}

//Draw Floor
void DrawFloor(){
	glPushMatrix();
		//glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		for (int i = 0; i < mapSize; ++i)
		{
			for (int j = 0; j < mapSize; ++j)
			{
				glPushMatrix();
					glTranslatef(i,0,j);
					s.glutSolidCube2(1);
				glPopMatrix();
			}
		}
	glPopMatrix();
}

void terrainFog(){
	glEnable(GL_FOG); 
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);  
	glFogf(GL_FOG_DENSITY, density); 
	glHint(GL_FOG_HINT, GL_NICEST); 
}

void enemyFog(){
	glEnable(GL_FOG); 
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);  
	glFogf(GL_FOG_DENSITY, density); 
	glHint(GL_FOG_HINT, GL_NICEST); 
	glFogf(GL_FOG_START,5);
	glFogf(GL_FOG_END,60);
}

//Draw Targets
void drawTargets(){
	enemyFog();
	for (int i = 0; i < targetList.size(); i++){
		targetList[i].draw(cameraPos->at(cameraIndex)->x,cameraPos->at(cameraIndex)->z);
	}
	terrainFog();


}

//Draw Enemies
void drawEnemies(){
	enemyFog();
	for (int i = 0; i < enemyList.size(); i++){

		if (cameraHeight == 2.5){
			enemyList[i].draw(cameraPos->at(cameraIndex)->x,cameraPos->at(cameraIndex)->y+1,cameraPos->at(cameraIndex)->z);
	
		}else {
			enemyList[i].draw(cameraPos->at(cameraIndex)->x,cameraPos->at(cameraIndex)->y+1.5,cameraPos->at(cameraIndex)->z);
		}
		
	}
	terrainFog();
}

//Draws the 3D scene
void Draw3DScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	/*float m_amb[] = {0.1, 0.1, 0.1, 1.0};
	float m_dif[] = {0.5, 0.5, 0.5, 1.0};
	float m_spec[] = {0.99, 0.91, 0.81, 1.0};
	float shiny =10;*/

	/*float m_amb[] = {0.3, 0, 0.0, 1.0};
	float m_dif[] = {0.6, 0, 0, 1.0};
	float m_spec[] = {0.8, 0.6, 0.6, 1.0};
	float shiny = 32;*/
	//printf("%f %f %f\n", cameraPos->at(cameraIndex)->x, cameraHeight, cameraPos->at(cameraIndex)->z);
	//printf("%f %f %f\n", lookAtPos->at(lookAtIndex)->x,lookAtPos->at(lookAtIndex)->y,lookAtPos->at(lookAtIndex)->z);
	gluLookAt(cameraPos->at(cameraIndex)->x, cameraHeight, cameraPos->at(cameraIndex)->z,
			  lookAtPos->at(lookAtIndex)->x,lookAtPos->at(lookAtIndex)->y,lookAtPos->at(lookAtIndex)->z ,
			  0,1,0);
	glEnable(GL_TEXTURE_2D);
	DrawFloor();
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glPushMatrix();
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glTranslatef(-5, 5, 0);
		glScalef(0.1, 0.1, 0.1);
		deer.render();
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(side,up,0);
		glTranslatef(24,1,23);
		glRotatef(70, 0,1,0);
		//glScalef(1.5,1.3,0.5);
		s.glutSolidCube2(1);
		glTranslatef(-1.3,0,-0.2);
		glRotatef(60, 0,1,0);
		s.glutSolidCube2(1);
		glColor3f(0,1,0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(15.1,1,21);
		glRotatef(10, 0,1,0);
		//glScalef(3,1.3,0.5);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		s.glutSolidCube2(1);
		glRotatef(13, 0,1,0);
		glTranslatef(1.2,0,0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		s.glutSolidCube2(1);
		glRotatef(-15, 0,1,0);
		glTranslatef(-2.2,0,1);
		
		s.glutSolidCube2(1);

		glRotatef(40, 0,1,0);
		glTranslatef(1.8,1,0);
		s.glutSolidCube2(1);
		glColor3f(0,1,0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(17,1,10);
		glRotatef(70, 0,1,0);
		glScalef(2,1.5,0.5);
		glBindTexture(GL_TEXTURE_2D, textures[4]);
		s.glutSolidCube2(1);
		glScalef(0.5,1,2);
		glRotatef(70, 0,1,0);
		glTranslatef(1,0,1);
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		s.glutSolidCube2(1);
		glTranslatef(-0.5,0,1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		s.glutSolidCube2(1);
		
		glColor3f(0,1,0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(17,1, 3);
		glRotatef(120, 0,1,0);
		glScalef(3,1.3,3);
		s.glutSolidCube2(1);
	glPopMatrix();

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		glColor3f(1,0,0);
		glTranslatef(12,1, 8);
		glRotatef(120, 0,1,0);
		glScalef(1,1,1);
		s.glutSolidCube2(1);
		glTranslatef(1.3,0,1);
		s.glutSolidCube2(1);
		glTranslatef(-2,0,0);
		s.glutSolidCube2(1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(10.5,1, 15);
		glRotatef(120, 0,1,0);
		glScalef(1,1,1);
		glutSolidTeapot(1);
		s.glutSolidCube2(1);
		glTranslatef(1.3,0,1);
		s.glutSolidCube2(1);
		glTranslatef(-2,0,0);
		s.glutSolidCube2(1);
	glPopMatrix();


	glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(30,1, 30);
		glRotatef(30, 0,1,0);
		glScalef(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		s.glutSolidCube2(1);

		glRotatef(-20, 0,1,0);
		glTranslatef(1,0,0);
		s.glutSolidCube2(1);
		glTranslatef(1,0,0);
		glRotatef(15, 0,1,0);

		s.glutSolidCube2(1);
		glTranslatef(0,0, -2);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		s.glutSolidCube2(1);
		glRotatef(-40, 0,1,0);
		glTranslatef(1,0,0 );
		glBindTexture(GL_TEXTURE_2D, textures[4]);
		s.glutSolidCube2(1);
		glTranslatef(1,0, -1);
		s.glutSolidCube2(1);

		glRotatef(120, 0,1,0);
		glTranslatef(2,0,0);
		s.glutSolidCube2(1);
		glTranslatef(-3,0,1);
		glRotatef(-45, 0,1,0);
		s.glutSolidCube2(1);
		glTranslatef(0,0, -2);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		s.glutSolidCube2(1);
		glRotatef(-40, 0,1,0);
		glTranslatef(1,0,0 );
		s.glutSolidCube2(1);

		glTranslatef(1,0, -1);
		s.glutSolidCube2(1);
	glPopMatrix();


	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		glColor3f(1,0,0);
		glTranslatef(30,1, 32);
		glRotatef(30, 0,1,0);
		glScalef(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures[4]);
		s.glutSolidCube2(1);

		glTranslatef(1,0,1);
		glScalef(1,1,1);
		s.glutSolidCube2(1);

		glTranslatef(-2,0, -2);
		glScalef(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		s.glutSolidCube2(1);
		glTranslatef(0.25,1, 1);
		glRotatef(30, 0,1,0);
		glScalef(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		s.glutSolidCube2(1);
		glTranslatef(3,-1, 5);
		glRotatef(30, 0,1,0);
		glScalef(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures[5]);
		s.glutSolidCube2(1);
	glPopMatrix();

	//drawPlayer();
	checkPlayerHit();

	/*glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); */
	glDisable(GL_TEXTURE_2D);

	if(isLevelCleared==false){
		drawTargets();
		drawEnemies();
	}
}

GLubyte* LoadPPM(char* file, int* width, int* height, int* max)
{
	GLubyte* img;
	FILE *fd;
	int n, m;
	int  k, nm;
	char c;
	int i;
	char b[100];
	float s;
	int red, green, blue;
	
	/* first open file and check if it's an ASCII PPM (indicated by P3 at the start) */
	fd = fopen(file, "r");
	fscanf(fd,"%[^\n] ",b);
	if(b[0]!='P'|| b[1] != '3')
	{
		printf("%s is not a PPM file!\n",file); 
		exit(0);
	}
	printf("%s is a PPM file\n", file);
	fscanf(fd, "%c",&c);

	/* next, skip past the comments - any line starting with #*/
	while(c == '#') 
	{
		fscanf(fd, "%[^\n] ", b);
		printf("%s\n",b);
		fscanf(fd, "%c",&c);
	}
	ungetc(c,fd); 

	/* now get the dimensions and max colour value from the image */
	fscanf(fd, "%d %d %d", &n, &m, &k);

	printf("%d rows  %d columns  max value= %d\n",n,m,k);

	/* calculate number of pixels and allocate storage for this */
	nm = n*m;
	img = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
	s=255.0/k;

	/* for every pixel, grab the read green and blue values, storing them in the image data array */
	for(i=0;i<nm;i++) 
	{
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*nm-3*i-3]=red*s;
		img[3*nm-3*i-2]=green*s;
		img[3*nm-3*i-1]=blue*s;
	}

	/* finally, set the "return parameters" (width, height, max) and return the image array */
	*width = n;
	*height = m;
	*max = k;

	return img;
}


void click(){
	if(isReloading == false && cameraHeight == 2.5){
	//get the ray picking vector
	vector<vec3D> vector = getRay();
	vec3D Rd = vector[1];
	vec3D R0 = vector[0];
/*	
	printf("--------------- \n ");
	printf("Rd: %f, %f, %f |  \n", Rd.x, Rd.y, Rd.z);
	printf("R0: %f, %f, %f | \n", R0.x, R0.y, R0.z);
*/
	//check floor intersections
	//floorIntersection(Rd,R0);

	//check for target intersections
	targetIntersections(Rd,R0);

	//calculate if you hit an enemy
	enemyIntersections(Rd,R0);

		//modify ammo count
		if(ammo > 0){
			ammo--;
		}else{
			ammo = 6;
		}
	}
	printf("ammo %i\n", ammo);
	//calculate if you hit an enemy



	/*
	-Search through list of objects in the Scene Graph and test Intersections with all of them
	-keep track of all the objects I hit, but only select the closest one
	*/


}

/*
Carlos Will comment this section
*/
void loadEnemies(){

	int stageIndex = 0;
	int saveIndex = 0;
	bool pushNewStage = false;

	string line;
	ifstream myfile( "loadEnemies.txt" );

	vector<Enemy> stageEnemies;

	if (myfile.is_open()){
		//iterate throught the file line by line
		while(getline(myfile,line)){
			vector<string> enemies = split(line, ' ');
			//insert the parameters of each target into the vector of enemies
			for (int i = 0; i < enemies.size(); i++){
				//cout << enemies[i] + ":";
				if (!enemies[i].compare(",")){
					stageIndex++;
					//printf("stage: %i ", stageIndex );
					pushNewStage = true;
					saveIndex = i;
					break;
				}
			}
			//printf("/ \n");
			if (pushNewStage == true){
				//printf("----------- \n");

				//push current vector of this stages enemies
				enemyInfo.push_back(stageEnemies);

				//Empty out current stage
				while (stageEnemies.size() > 0){
					stageEnemies.pop_back();
				}
				
				pushNewStage = false;
				
			}else {
				//make a Target and add to the stage
				Enemy e(stof(enemies[0]),
					stof(enemies[1]),
					stof(enemies[2]),
					stof(enemies[3]),
					stof(enemies[4]),
					stof(enemies[5]),
					stof(enemies[6]),
					stof(enemies[7]));
				stageEnemies.push_back(e);
			}
			
		}
		//printf("There are %i stages \n", stageIndex );
		myfile.close();
	}else {
		cout << "Unable to open file.";
	}
}

/*
Carlos Will comment this section
*/
void loadTargets(){

	int stageIndex = 0;
	int saveIndex = 0;
	bool pushNewStage = false;

	string line;
	ifstream myfile( "loadTargets.txt" );

	vector<Target> stageTarget;

	if (myfile.is_open()){
		//iterate throught the file line by line
		while(getline(myfile,line)){
			vector<string> targets = split(line, ' ');
			//insert the parameters of each target into the vector of targets
			for (int i = 0; i < targets.size(); i++){
				//cout << targets[i] + ":";
				if (!targets[i].compare(",")){
					stageIndex++;
					//printf("stage: %i ", stageIndex );
					pushNewStage = true;
					saveIndex = i;
					break;
				}
			}
			//printf("/ \n");
			if (pushNewStage == true){
				//printf("----------- \n");

				//push current vector of this stages targets
				targetInfo.push_back(stageTarget);

				//Empty out current stage
				while (stageTarget.size() > 0){
					stageTarget.pop_back();
				}
				
				pushNewStage = false;
				
			}else {
				//make a Target and add to the stage
				Target t(stof(targets[0]),
					stof(targets[1]),
					stof(targets[2]),
					stof(targets.at(3)),
					stof(targets.at(4)) );
				stageTarget.push_back(t);
			}
			
		}
		//printf("There are %i stages \n", stageIndex );
		myfile.close();
	}else {
		cout << "Unable to open file.";
	}
}


void restartGame(){
	isLevelCleared = false;
	stageNumber = 0;
	frameCounter = 0;
	cameraHeight = 2.5;
	first = true;
	lookAtIndex = 0;
	cameraIndex = 0;
	cameraPosSize = 0;

	health = 3;
	score = 0;

	ammo = 6;
	isTimed = true;
	isReloading = false;
	timeReloadCounter = 50;

	firstTime = true;
	gameOver = false;
	while(time1+timeIncr< 120){
		timeIncr++;
	}


	while (enemyList.size() > 0){
		enemyList.pop_back();
	}
	while (enemyList.size() > 0){
		enemyList.pop_back();
	}
	
	//create new scene?
	createTargetList();
	createEnemyList();



	/*
	while (enemyList.size() > 0){
		enemyList.pop_back();
	}
	while (enemyList.size() > 0){
		enemyList.pop_back();
	}
	*/
	//reinitialize everything

}

//handles the mouse events
void mouse(int btn, int state, int x, int y){

	mouseX = x;
	mouseY = 800 - y;

	//create a bump if the left mouse button is pressed
	if(btn == GLUT_LEFT_BUTTON){
		if(state == GLUT_DOWN){
			//printf("Left click %i , %i \n", mouseX, mouseY);
			click();
		}

	//create a dent if the right button is pressed
	}else if(btn == GLUT_RIGHT_BUTTON){
		if(state == GLUT_DOWN){
			printf("Right click %i , %i \n", mouseX, mouseY);
			click();
		}
	}

	if(x > 302 && x < 500 && y < 500 && y> 450 && (isLevelCleared == true || gameOver==true)){
		//printf("Restart\n");
		restartGame();
	}

	glutPostRedisplay();

}

//Passive keyboard function
void passive(int x, int y){
	mouseX = x;
	mouseY = 800 - y;
}


void keyboard(unsigned char key, int x, int y)
{

	/* key presses move the cube, if it isn't at the extents (hard-coded here) */
	switch (key)
	{
		case 'q':
		case 27:
			exit (0);
			break;

		case 'a':
		case 'A':
			if(light_pos[0] > -15)
				light_pos[0]-=1;
			break;

		case 'w':
		case 'W':
			if(light_pos[2] > -15)
				light_pos[2] -= 1;
			break;

		case 'd':
		case 'D':
			if(light_pos[0] < 15)
				light_pos[0]+=1;
			break;

		case 's':
		case 'S':
			if(light_pos[2] < 15)
				light_pos[2] += 1;
			break;

		case 'r':
			if(light_pos[1] < 15)
				light_pos[1] += 1;
			break;

		case 'f':
			if(light_pos[1] > 0)
				light_pos[1] -= 1;
			break;
		case 'x':
			//printf("cameraIndex %i, Size: %i  \n",cameraIndex, cameraPos->size()-1 );
			if(cameraIndex < cameraPos->size()-1){
				cameraIndex++;
			}
			break;
		case 'z':
			if(first == true){
				first = false;
			}else{
				if(stageNumber < stages->size() ){
					stageNumber++;
				}
			}
			break;
		case ' ':
			if(cameraHeight == 1.5){
				cameraHeight = 2.5;
			}else{
				cameraHeight = 1.5;
			}
			break;
		case 'v':
			if(lookAtIndex < lookAtPos->size()-1){
				lookAtIndex++;
			}else{
				lookAtIndex = 0;
			}
			break;
		case 'b':
			//image_data = LoadPPM("h2.ppm", &width, &height, &max2);
			health--;
			break;
		case 'n':
			//image_data = LoadPPM("h2.ppm", &width, &height, &max2);
			timeIncr +=10;
			break;


	}

	glutPostRedisplay();
}

void special(int key, int x, int y)
{
	/* arrow key presses move the camera */
	switch(key)
	{
		case GLUT_KEY_LEFT:
			camPos[0]-=0.1;
			break;

		case GLUT_KEY_RIGHT:
			camPos[0]+=0.1;
			break;

		case GLUT_KEY_UP:
			camPos[2] -= 0.1;
			break;

		case GLUT_KEY_DOWN:
			camPos[2] += 0.1;
			break;
		
		case GLUT_KEY_HOME:
			camPos[1] += 0.1;
			break;

		case GLUT_KEY_END:
			camPos[1] -= 0.1;
			break;

	}
	glutPostRedisplay();
}

/***
Given the start point and end point locations, This method
will interpolate steps number of location in between the start
and end locations. (Giving a smooth animation)
***/
void getSlopeVector(point3D *start, point3D *end, int steps){
	float x1 = start->x;
	float x2 = end->x;
	float y = start->y;

	float z1 = start->z;
	float z2 = end->z;

	float run = (x2 -x1)/ (float) steps;
	float rise = (z2 - z1)/ (float) steps;

	//add the camera position for each frame
	insertPoint3D(start);
	printf("Start: %f %f %f \n", start->x, start->y, start->z );
	frameCounter++;

	//interpolate all the points between the start and end points
	for (int i = 1; i < steps; ++i)
	{

		point3D *p = new point3D(x1+run, y, z1+rise);
		printf("\t %f %f %f \n", p->x, p->y, p->z );
		
		insertPoint3D(p);
		x1 +=run;
		z1 += rise;
		frameCounter++;
	}

	insertPoint3D(end);
	printf("End: %f %f %f \n", end->x, end->y, end->z );
	frameCounter++;
	stages->push_back(frameCounter);

}



//Load the positions of where the camera should be looking at
void loadLookAtPosition(){
	string line;
	ifstream myfile ("lookAtPositions.txt");
	if(myfile.is_open()){
		//iterate through the file line by line
		while(getline (myfile,line)){
			vector<string> sub = split(line, ' ');
			//insert each point into the vector of look at positions
			if(sub.size() > 1){
				point3D *p= new point3D(stoi(sub.at(0)),stoi(sub.at(1)),stoi(sub.at(2)));
				insertPoint3DLookAt(p);
			}
		}

	}else{
		cout << "Unable to open file.";
	}

}

//Load the camera positions from textfile
void loadCameraPoints(){
	string line;
	ifstream myfile ("cameraPositions.txt");
	if (myfile.is_open()){
		point3D *start;
		point3D *end;
		bool first = true;

		//iterate through the text file line by line
		while(getline (myfile,line)){
			//split the line into a vector
			vector<string> sub = split(line, ' ');

			if(sub.size() > 1){
				if(first == true){
					start = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
					cout<<"Start " <<sub.at(0) << " " + sub.at(1) << " "+ sub.at(2) << endl;
					first = false;
				}else{
					end = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
					getSlopeVector(start,end,15);
					start = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
				}		
			}
		}
		myfile.close();
	}else{
		cout << "Unable to open file";
	}
}

/*
Carlos Will comment this section
*/
void printTargetInfo(){

	for ( int i = 0; i < targetInfo.size(); i++){
		printf("Start of new Stage. Targets: \n");
		for (int j = 0; j< targetInfo[i].size(); j ++){
			printf("  %f %f %f %f %f \n",targetInfo[i][j].x,targetInfo[i][j].y,targetInfo[i][j].z,targetInfo[i][j].radius,targetInfo[i][j].scale );
		}
		printf("End of Stage. \n");
	}
}


//Display the proper health bar status of the character
void ManageHealth(){
	if(health == 3){ //Full Health
		healthBar_image = images[0];
	}else if (health ==2){ //Medium Health
		healthBar_image= images[1];
	}else if (health ==1){	//Low Health
		healthBar_image = images[2];
	}else if (health == 0){	//Empty Health
		healthBar_image = images[3];
		printf("GAME OVER\n");
		gameOver = true;
		//isLevelCleared = true;
		for (int i = 0; i < enemyList.size(); i++){
			enemyList[i].bullet.active = false;
		}


	}else{
		health = 3;
	}
}

//Display proper ammo image on hud
void ManageAmmo(){
	if(ammo == 6){
		ammo_image = images[4];
	}else if (ammo==5){
		ammo_image = images[5];
	}else if(ammo ==4){
		ammo_image = images[6];
	}else if(ammo==3){
		ammo_image = images[7];
	}else if (ammo==2){
		ammo_image = images[8];
	}else if(ammo ==1){
		ammo_image = images[9];
		startTime = time1;
	}else if(ammo==0){
		ammo_image = images[10];
		//handle the reloading time
		isReloading = true;
		if(isTimed== true){
			startTime = time1+timeIncr+timeReloadCounter;
			endTime= startTime - timeReloadCounter;
			printf("INITIAL Start time: %i, End time: %i\n",startTime, endTime);
			isTimed = false;
		}else{
			timeReloadCounter--;
			startTime = time1+timeIncr+timeReloadCounter;
			printf("Start time: %i, End time: %i\n",startTime, endTime);
			if(startTime <= endTime){
				printf("DONE RELOADING\n");
				isReloading = false;
				ammo=6;
				isTimed = true;
				timeReloadCounter = 50;
				glutPostRedisplay();
			}
		}
	}
}

//Initialize variables and Hud images
void init(void)
{
	deer = Object("car.obj");

	
	images[0] = LoadPPM("HUD/h1.ppm", &width, &height, &max2);
	images[1] = LoadPPM("HUD/h2.ppm", &width, &height, &max2);
	images[2] = LoadPPM("HUD/h3.ppm", &width, &height, &max2);
	images[3] = LoadPPM("HUD/h4.ppm", &width, &height, &max2);
	images[4] = LoadPPM("HUD/ammo6.ppm", &width2, &height2, &max3);
	images[5] = LoadPPM("HUD/ammo5.ppm", &width2, &height2, &max3);
	images[6] = LoadPPM("HUD/ammo4.ppm", &width2, &height2, &max3);
	images[7] = LoadPPM("HUD/ammo3.ppm", &width2, &height2, &max3);
	images[8] = LoadPPM("HUD/ammo2.ppm", &width2, &height2, &max3);
	images[9] = LoadPPM("HUD/ammo1.ppm", &width2, &height2, &max3);
	images[10] = LoadPPM("HUD/ammo0.ppm", &width2, &height2, &max3);
	images[11] = LoadPPM("HUD/restart.ppm", &width3, &height3, &max4);
	healthBar_image = images[0];
	ammo_image = images[4];
	restart_image = images[11];

	

	crate_tex = LoadPPM("Textures/crates_256.ppm", &width5, &height5, &max6);
	stone_tex = LoadPPM("Textures/stone_256.ppm", &width4, &height4, &max5);
	steel_tex = LoadPPM("Textures/steel_256.ppm", &width6, &height6, &max7);
	flameCrate_tex= LoadPPM("Textures/flamableCrate_256.ppm", &width7, &height7, &max8);
    steelCrate_tex = LoadPPM("Textures/steelCrate_256.ppm", &width8, &height8, &max9);
	modernCrate_tex = LoadPPM("Textures/modernCrate_256.ppm", &width9, &height9, &max10);
	glEnable(GL_TEXTURE_2D);

	glGenTextures(6, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width5, height5, 0, GL_RGB,GL_UNSIGNED_BYTE, crate_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width4, height4, 0, GL_RGB,GL_UNSIGNED_BYTE, stone_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[2]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width6, height6, 0, GL_RGB,GL_UNSIGNED_BYTE, steel_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[3]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width7, height7, 0, GL_RGB,GL_UNSIGNED_BYTE, flameCrate_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[4]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width8, height8, 0, GL_RGB,GL_UNSIGNED_BYTE, steelCrate_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	glBindTexture(GL_TEXTURE_2D, textures[5]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width9, height9, 0, GL_RGB,GL_UNSIGNED_BYTE, modernCrate_tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

	//images[12] = LoadPPM("Textures/crates_256.ppm", &width4, &height4, &max5);
	//images[13] = LoadPPM("Textreus/stone_256.ppm", &width4, &height4, &max5);


	glDisable(GL_TEXTURE_2D);


	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float position[4] = {20,10,20, 20};

	float amb[4] = {0.5, 0.5, 0.5, 1};
	float diff[4] = {1,1,1, 1};
	float spec[4] = {1,1,1, 1};


	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glEnable(GL_LIGHT1);

	float position1[4] = {0,10,0, 20};
	float amb1[4] = {0.5, 0.5, 0.5, 1};
	float diff1[4] = {1,1,1, 1};
	float spec1[4] = {1,1,1, 1};


	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

	loadCameraPoints();
	loadLookAtPosition();
	cameraPosSize = 0;



	glEnable(GL_FOG); 
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);  
	glFogf(GL_FOG_DENSITY, density); 
	glHint(GL_FOG_HINT, GL_NICEST); 
	glFogf(GL_FOG_START,1);
	glFogf(GL_FOG_END,45);


	//printf("Stage Size: %i\n", stages->size() );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, -2, 2);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	gluPerspective(45, 1, 1, 100);

	//Initialze Targets
	loadTargets();
	printTargetInfo();
	createTargetList();

	loadEnemies();
	createEnemyList();

	for (int x = 0; x < myfloor.size(); ++x)
	{
		for (int z = 0; z < myfloor.size(); ++z)
		{
			myfloor[x][z] = 0.0f;
		}
	}

	//elapsedTime = time (NULL);
		
}

void look(){
	//printf("LOOK: %i STAGE: %i\n", lookAtIndex, stageNumber);
	if(stageNumber == 0){
		lookAtIndex = 0;
	}else if (stageNumber == 1){
		lookAtIndex = 1;
	}else if (stageNumber ==2){
		lookAtIndex = 2;
	}else if (stageNumber ==3){
		lookAtIndex = 3;
	}else if (stageNumber ==4){
		lookAtIndex = 4;	
	}else if (stageNumber ==5){
		lookAtIndex = 5;	
	}else if (stageNumber ==6){
		lookAtIndex = 6;
	}else if (stageNumber ==7){
		lookAtIndex = 7;
	}else if (stageNumber ==8){
		lookAtIndex = 8;
	}
}

//OpenGl function that handles the frames per second
void FPS(int val){
	glutPostRedisplay();
	//printf("Stage number: %i\n", stageNumber );
	if(first == false){
		if(cameraIndex < cameraPos->size()-1 && cameraIndex <=stages->at(stageNumber)){
			cameraIndex++;
			glutTimerFunc(100,FPS,100);
		}
	}

}

void checkClearedStage(){
	if(targetList.size()==0 && enemyList.size() == 0){
		printf("Hello\n");
		if(first == true){
			first = false;
		}else{
			if(stageNumber < stages->size() ){
				stageNumber++;
			}
		}
		createTargetList();
		createEnemyList();
		timeIncr +=10;
	}
}


/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{

	glPushMatrix();
		// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glTranslatef(-5, 5, 0);
		glScalef(0.1, 0.1, 0.1);
		deer.render();
	glPopMatrix();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	elapsedTime = glutGet(GLUT_ELAPSED_TIME);
	if(isLevelCleared == true){
			timeAtReset = glutGet(GLUT_ELAPSED_TIME);
			//timeAtReset = glutGet(GLUT_ELAPSED_TIME);
			firstTime = false;
	}
	DrawHUD();
	DrawText();
	Draw3DScene();
	if(stageNumber == stages->size()-1){
		isLevelCleared = true;
	}

	if(isLevelCleared == false){
		ManageHealth();
		ManageAmmo();
		look();
		checkClearedStage();
	}
	glutTimerFunc(100,FPS,0);
	glutSwapBuffers(); 
	//printf("%f %f %f\n", cameraPos->at(cameraIndex)->x, cameraHeight, cameraPos->at(cameraIndex)->z);



}

/* main function - program entry point */
int main(int argc, char** argv)
{
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("On Rails");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passive);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}