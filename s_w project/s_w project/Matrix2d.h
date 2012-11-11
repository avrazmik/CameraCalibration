/**
*	@file Matrix2d.h
*	@brief file contains the definaion of the class Matrix2d.
*	
*	@author Razmik Avetisyan
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <GL/glut.h>
#include "MatrixT.h"

class Matrix2d{
public:
	Matrix2d();
	Matrix2d(GLdouble data[3][3]);
	void set(GLdouble data[3][3]);
	GLdouble * get();
	void print();
	Matrix2d & operator= (Matrix2d);
	Matrix2d operator* (int);
	Matrix2d operator* (Matrix2d);
	GLdouble * operator* (MatrixT data);
	Matrix2d transpose();
	void identity();
	void zero();
	GLdouble * homogeneous();
private:
	GLdouble m[3][3];
};

