/**
*	@file Matrix2d.h
*	@brief file contains the definaion of the class LookAt.
*	
*	@author Razmik Avetisyan
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <GL/glut.h>
#include "Matrix2d.h"

class LookAt:public Matrix2d{
	friend class Scene;
public:
	LookAt();
	LookAt(GLdouble data[3][3]);

	LookAt& eyex(GLdouble data);
	GLdouble eyex();
	
	LookAt& eyey(GLdouble data);
	GLdouble eyey();

	LookAt& eyez(GLdouble data);
	GLdouble eyez();

	LookAt& centerx(GLdouble data);
	GLdouble centerx();

	LookAt& centery(GLdouble data);
	GLdouble centery();

	LookAt& centerz(GLdouble data);
	GLdouble centerz();

	LookAt& upx(GLdouble data);
	GLdouble upx();

	LookAt& upy(GLdouble data);
	GLdouble upy();

	LookAt& upz(GLdouble data);
	GLdouble upz();
private:
	void doCommand();
};
