/* CS 3GC3 - Simple lighting example
 * by R. Teather
 */
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
#include "basicMathLibrary.cpp" 		//for 3D points and Vectors
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std; 


//float verts[8][3] = { {-1,-1,1}, {-1,1,1}, {1,1,1}, {1,-1,1}, {-1,-1,-1}, {-1,1,-1}, {1,1,-1}, {1,-1,-1} };
//float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };
float light_pos[] = {5,10,5,1};
vector<point3D*> *cameraPos = new vector<point3D*>;
vector<point3D*> *lookAtPos = new vector<point3D*>;
vector<int> *stages = new vector<int>;
int stageNumber = 0;
int frameCounter = 0;
//int lookAtCounter = 0;
int cameraHeight = 1;
bool first = true;

int lookAtIndex = 0;
int cameraIndex = 0;
int cameraPosSize = 0;

float pos[] = {0,1,0};

float camPos[] = {5, 5, 10};
float angle = 0.0f;

int height = 0;
int width = 0;
int max2 = 0;
GLubyte *image_data;

int side = 0;
int up = 0;
float ang = 0.0f;

void insertPoint3D(point3D *p){
	//printf("insert %f , %f , %f\n", p->x, p->y,p->z);
	cameraPos->push_back(p);
	//cameraPosSize++;
	//printf("Insert, CameraPos Size: %i \n", cameraPosSize );
}

void insertPoint3DLookAt(point3D *p){
	//printf("insert %f , %f , %f\n", p->x, p->y,p->z);
	lookAtPos->push_back(p);
	//lookAtCounter++;
	//printf("Insert, CameraPos Size: %i \n", cameraPosSize );
}

void DrawHUD(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 800, 0, 800);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(width,0);
	glPixelZoom(-1, 1);
	glDrawPixels(width,height,GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glFlush(); 
}

void DrawFloor(){
	glPushMatrix();
		glScalef(100,0.5,100);
		glutSolidCube(1);
	glPopMatrix();
}

void Draw3DScene(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	float origin[] = {0,0,0,1};
	float m_amb[] = {0.33, 0.22, 0.03, 1.0};
	float m_dif[] = {0.78, 0.57, 0.11, 1.0};
	float m_spec[] = {0.99, 0.91, 0.81, 1.0};
	float shiny = 27;

	/*float m_amb[] = {0.3, 0, 0.0, 1.0};
	float m_dif[] = {0.6, 0, 0, 1.0};
	float m_spec[] = {0.8, 0.6, 0.6, 1.0};
	float shiny = 32;*/

	gluLookAt(cameraPos->at(cameraIndex)->x, cameraHeight, cameraPos->at(cameraIndex)->z,
			  lookAtPos->at(lookAtIndex)->x,lookAtPos->at(lookAtIndex)->y,lookAtPos->at(lookAtIndex)->z ,
			  0,1,0);
	

	//gluLookAt(cameraPos->at(cameraIndex)->x, cameraPos->at(cameraIndex)->y, cameraPos->at(cameraIndex)->z, 0,0,0, 0,1,0);
	
	//gluLookAt(camPos[0], camPos[1] , camPos[2], 0,0,0, 0,1,0);

	//glColor3f(1,1,1);

	DrawFloor();

	/*glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
	*/
	glPushMatrix();
		glTranslatef(side,up,0);
		glTranslatef(0,2,0);
		glRotatef(ang, 0,1,0);
		glutSolidTeapot(1);
		glColor3f(0,1,0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-25,1,5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1,0,0);
		glTranslatef(7,1,-15);
		glutSolidSphere(1,50,50);

	glPopMatrix();
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

void mouse(int btn, int state, int x, int y){
	y = 800 - y;

	if(btn = GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		printf("x = %d , y= %d \n", x, y);
		if( x >= 12 && x <=64 && y >= 137 && y <= 180){
			side--;
		}
	}

	glutPostRedisplay();
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
				if(stageNumber < stages->size() -1){
					stageNumber++;
				}
			}
			break;
		case ' ':
			if(cameraHeight == 1){
				cameraHeight = 8;
			}else{
				cameraHeight = 1;
			}
			break;
		case 'v':
			if(lookAtIndex < lookAtPos->size()-1){
				lookAtIndex++;
			}else{
				lookAtIndex = 0;
			}
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

	//printf("%f\n", rise/steps);
	//printf("%f\n", run/steps);


	insertPoint3D(start);
	frameCounter++;
	//cameraIndex++;
	//glutPostRedisplay();

	for (int i = 0; i < steps; ++i)
	{

		point3D *p = new point3D(x1+run, y, z1+rise);
		insertPoint3D(p);
		x1 +=run;
		z1 += rise;
		frameCounter++;
		//cameraIndex++;
		//glutPostRedisplay();
	}

	insertPoint3D(end);
	frameCounter++;
	//printf("Frame Count: %i\n", frameCounter);
	stages->push_back(frameCounter);
	//cameraIndex++;
	//glutPostRedisplay();
	
	//float incr = 0.1;
	//if(x1 > x2)

}

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

	/*point3D *origin = new point3D(0,0,0); 
	insertPoint3DLookAt(origin);

	point3D *p = new point3D(7,1,-15); 
	insertPoint3DLookAt(p);

	p = new point3D(-25,5,5); 
	insertPoint3DLookAt(p);*/
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
				//printf("%s %s %s \n", sub.at(0).c_str(), sub.at(1).c_str(), sub.at(2).c_str());
				if(first == true){
					//printf("Start 1: %s %s %s \n", sub.at(0).c_str(), sub.at(1).c_str(), sub.at(2).c_str());
					start = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
					cout<<"Start " <<sub.at(0) << " " + sub.at(1) << " "+ sub.at(2) << endl;
					first = false;
					//isStart = false;
				}else{
					//printf("End 2: %s %s %s \n", sub.at(0).c_str(), sub.at(1).c_str(), sub.at(2).c_str());	
					end = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
					//cout<<"End " <<sub.at(0) << " " + sub.at(1) << " "+ sub.at(2) << endl;
					getSlopeVector(start,end,15);
					start = new point3D(stoi(sub.at(0)),stoi(sub.at(1)), stoi(sub.at(2)));
					//cout<<"Start" <<sub.at(0) << " " + sub.at(1) << " "+ sub.at(2) << endl;
					//isStart = true;
				}
					
					
			}
		}
		myfile.close();
	}else{
		cout << "Unable to open file";
	}


/*
	point3D *p1 = new point3D(25,5,25);
	//insertPoint3D(p1);
	point3D *p2 = new point3D(5,5,10);
	//insertPoint3D(p2);

	getSlopeVector(p1,p2,15);
	point3D *p3 = new point3D(-5,5,15);
	getSlopeVector(p2,p3,15);
	
	point3D *p4 = new point3D(-5,5,25);
	getSlopeVector(p3,p4,15);

	point3D *p5 = new point3D(25,5,25);
	getSlopeVector(p4,p5,15);

	point3D *p6 = new point3D(50,5,50);
	getSlopeVector(p5,p6,15);

	point3D *p7 = new point3D(30,5,25);
	getSlopeVector(p6,p7,15);

	point3D *p8 = new point3D(10,5,0);
	getSlopeVector(p7,p8,15);

	point3D *p9 = new point3D(30,5,0);
	getSlopeVector(p8,p9,15);*/
}

void init(void)
{
	glClearColor(0, 0, 0, 0);
	glColor3f(1, 1, 1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float position[4] = {1.5,0,0, 0};

	float amb[4] = {1.0, 1, 1, 1};
	float diff[4] = {1,0,0, 1};
	float spec[4] = {0,0,1, 1};


	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

	loadCameraPoints();
	loadLookAtPosition();
	cameraPosSize = 0;

	printf("Stage Size: %i\n", stages->size() );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, -2, 2);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	gluPerspective(45, 1, 1, 100);
		
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
	//glutTimerFunc(100,FPS,100); // 1sec = 1000, 59fps = 1000/59 = 17
	//glutTimerFunc(FPSspeed(speedScalar),FPS,FPSspeed(speedScalar)); // 1sec = 1000, 59fps = 1000/59 = 17
}

/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw3DScene();
	DrawHUD();
	glutTimerFunc(100,FPS,0);
	glutSwapBuffers(); 

}

/* main function - program entry point */
int main(int argc, char** argv)
{
	
	image_data = LoadPPM("interface.ppm", &width, &height, &max2);
	glutInit(&argc, argv);		//starts up GLUT
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("On Rails");	//creates the window

	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutSpecialFunc(special);

	glEnable(GL_DEPTH_TEST);
	init();

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}