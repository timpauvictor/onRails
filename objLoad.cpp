#include "basicMathLibrary.h" 
#include "objLoad.h"
#include <math.h> 
#include <vector> 
#include <stdio.h> 
#include <fstream> 
#include <sstream> 
#include <iostream>

facePoint::facePoint() {
	//empty contructor
} 

facePoint::facePoint(point3D newV) {
	v = newV;
}

Face::Face(facePoint x, facePoint y, facePoint z) {
	f1 = x;
	f2 = y;
	f3 = z;
}
 
std::vector<point3D> vertexList; 
std::vector<point3D> normalList; 
std::vector<Face> facesList; 
 
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
 
bool loadObj(std::string s) { 
  int counter = 0; 
  printf("loading obj file"); 
  std::string line; 
  std::ifstream myfile(s); 
  std::vector<std::string> args; 
  std::vector<std::string> faceArgs; 
   
  if (myfile.is_open()) { 
    while (std::getline(myfile, line)) { 
      counter++; 
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
        for (int i = 1; i < 4; i++) {
          faceArgs = objSplit(args.at(i),'/'); 
          point3D v =  vertexList.at(std::stof(faceArgs.at(0)) - 1); 
          // point3D vn = vertexList.at(std::stof(faceArgs.at(2)) - 1); 
          facePoint x(v);
        } 
      } 
    } 
  } 
}