/**
*	@file MatrixDistortion.h
*	@brief file contains the definaion of the class MatrixDistortion.
*	
*	@author Razmik Avetisyan
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <GL/glut.h>

class MatrixDistortion{
public:
	MatrixDistortion();
	MatrixDistortion(GLdouble data[5]);
	void set(GLdouble data[5]);
	GLdouble* get();
	void print();	
	void operator= (MatrixDistortion);
	MatrixDistortion operator* (int);
private:
	GLdouble m[5];
};