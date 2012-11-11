/**
*	@file MatrixT.h
*	@brief file contains the definaion of the class MatrixT.
*	
*	@author Razmik Avetisyan
*/


#pragma once
#include <iostream>
#include <iomanip>
#include <GL/glut.h>

class MatrixT{
public:
	MatrixT();
	MatrixT(GLdouble data[3]);
	void set(GLdouble data[3]);
	GLdouble* get();
	void updateX(double);
	void updateY(double);
	void updateZ(double);
	void print();
private:
	GLdouble m[3];
};

