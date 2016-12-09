#ifndef __OBJLOAD__H_ 
#define __OBJLOAD__H_ 
#include "basicMathLibrary.h" 
#include <math.h> 
#include <vector> 
#include <stdio.h> 
#include <fstream> 
#include <sstream> 
#include <iostream> 
 
class facePoint {
public:
	point3D v;
	point3D vn;
	// point3D vt;
	facePoint();
	facePoint(point3D newV, point3D newVN);
};

class Face {
public:
	facePoint f1;
	facePoint f2;
	facePoint f3;
	Face();
	Face(facePoint x, facePoint y, facePoint z);
};


bool loadObj(std::string path); 
void getVertices(); 
void getNormals();
void getFaces();
std::vector<std::string> objSplit(std::string root, char delim); 

class Object {
	public:
		Object(std::string path);
		Object();
		std::vector<point3D> getVertices();
		std::vector<point3D> getNormals();
		void render();
	private:
		std::vector<point3D> vertexList;
		std::vector<point3D> normalList;
		std::vector<Face> facesList;
		std::vector<facePoint> facePoints;
};
 
#endif