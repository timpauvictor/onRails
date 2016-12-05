/* CS 3GC3 - Simple lighting example
 * by R. Teather
 *
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

//float verts[8][3] = { {-1,-1,1}, {-1,1,1}, {1,1,1}, {1,-1,1}, {-1,-1,-1}, {-1,1,-1}, {1,1,-1}, {1,-1,-1} };
//float cols[6][3] = { {1,0,0}, {0,1,1}, {1,1,0}, {0,1,0}, {0,0,1}, {1,0,1} };
float light_pos[] = {5,10,5,1};

float pos[] = {0,1,0};

float camPos[] = {5, 5, 10};
float angle = 0.0f;

int height = 0;
int width = 0;
int max = 0;
GLubyte *image_data;

int side = 0;
int up = 0;
float ang = 0.0f;



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


	gluLookAt(camPos[0], camPos[1], camPos[2], 0,0,0, 0,1,0);
	glColor3f(1,1,1);


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	glPushMatrix();
		glTranslatef(side,up,0);
		glRotatef(ang, 0,1,0);
		glutSolidTeapot(1);
		glColor3f(1,1,1);

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


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-2, 2, -2, 2, -2, 2);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	gluPerspective(45, 1, 1, 100);
		
}



/* display function - GLUT display callback function
 *		clears the screen, sets the camera position, draws the ground plane and movable box
 */
void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw3DScene();
	DrawHUD();
	glutSwapBuffers(); 

}

/* main function - program entry point */
int main(int argc, char** argv)
{
	
	image_data = LoadPPM("interface.ppm", &width, &height, &max);
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