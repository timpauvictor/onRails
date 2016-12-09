#include "basicMathLibrary.h" 
#include "objLoad.h"
#include <math.h> 
#include <vector> 
#include <stdio.h> 
#include <fstream> 
#include <sstream> 
#include <iostream>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

facePoint::facePoint() {
	//empty contructor
} 

facePoint::facePoint(point3D newV, point3D newVN) {
	v = newV;
  vn = newVN;
}

Face::Face(facePoint x, facePoint y, facePoint z) {
	f1 = x;
	f2 = y;
	f3 = z;
}
 
std::vector<std::string> objSplit(std::string root, char delim) { 
  std::vector<std::string> elements; 
  std::stringstream ss; 
  ss.str(root);
  std::string item; 
 
  while (std::getline(ss, item, delim)) { 
    elements.push_back(item); 
  } 
 
  return elements; 
} 

Object::Object() {
  //do nothing
}
 
Object::Object(std::string s) {
  int counter = 0; 
  printf("loading obj file"); 
  std::string line; 
  std::ifstream myfile(s); 
  std::vector<std::string> args; 
  std::vector<std::string> faceArgs; 
   
  if (myfile.is_open()) { 
    while (std::getline(myfile, line)) { 
      counter++; 
      printf("\n%i", counter);
      args = objSplit(line, ' '); 
      if (args.at(0) == "v") { 
        vertexList.push_back(point3D( 
          std::stof(args.at(1)), 
          std::stof(args.at(2)),
          std::stof(args.at(3)))); 
      } else if (args.at(0) == "vn") { 
        normalList.push_back(point3D( 
          std::stof(args.at(1)), 
          std::stof(args.at(2)), 
          std::stof(args.at(3)))); 
      } else if (args.at(0) == "f") {
        facePoints.clear();
        for (int i = 1; i < 4; i++) {
          // printf("reached f");
          faceArgs = objSplit(args.at(i),'/'); 
          point3D v =  vertexList.at(std::stof(faceArgs.at(0)) - 1); 
          point3D vn = vertexList.at(std::stof(faceArgs.at(2)) - 1);
          facePoint x(v, vn);
          facePoints.push_back(x);
        }
        // printf("\n\nfacePoints size: %i\n\n", facePoints.size());
        Face y = Face(facePoints.at(0), facePoints.at(1), facePoints.at(2));
        facesList.push_back(y);
      } else if (args.at(0) == "vt") {
        
      }
    }
    printf("Succesfully loaded object with %i faces", facesList.size());
  } else {
    printf("\n\nERROR OPENING FILE\n\n");
  }
}

void Object::render() {
  facePoint one;
  facePoint two;
  facePoint three;
  glBegin(GL_TRIANGLES);
    for (int i = 0; i < facesList.size(); i++) {
        facePoint one = facesList.at(i).f1;
        point3D toDraw = one.v;
        //printf("Drawing at point: %f %f %f",toDraw.x, toDraw.y, toDraw.z);
        glVertex3f(toDraw.x, toDraw.y, toDraw.z);

        facePoint two = facesList.at(i).f2;
        toDraw = two.v;
        
        //printf("Drawing at point: %f %f %f",toDraw.x, toDraw.y, toDraw.z);
        glVertex3f(toDraw.x, toDraw.y, toDraw.z);

        facePoint three = facesList.at(i).f3;
        toDraw = three.v;
        //printf("Drawing at point: %f %f %f",toDraw.x, toDraw.y, toDraw.z);
        glVertex3f(toDraw.x, toDraw.y, toDraw.z);
    }
  glEnd();
}