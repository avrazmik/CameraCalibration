/**
*	@file Grid.h
*	@brief file contains the definaion of the class Grid.
*	
*	@author Razmik Avetisyan
*/

#pragma once
#include <iostream>
#include <iomanip>
#include <GL/glut.h>
#include <list>
#include <math.h>

#include "Camera.h"

using namespace std;

enum Transformation {TRANSLATION, ROTATION};
const double pi = 3.14;

class Grid{
public:
	Grid();
	void render();
	void updateTranslateX(double);
	void updateTranslateY(double);
	void updateTranslateZ(double);
	void updateRotationX(double);
	void updateRotationY(double);
	void updateRotationZ(double);
	void addCamera(Camera*);
	void setTransformationMode(Transformation);
	Transformation getTransformationMode();

	// Helper functions
	Matrix2d* getXRotationMatrix(double);
	Matrix2d* getYRotationMatrix(double);
	Matrix2d* getZRotationMatrix(double);
	Matrix2d getRotationMatrix();
	MatrixT getTranslationVector();

	// this is for testing
	bool axes;
private:
	list<Camera*> cameras;
	MatrixT translation;
	Matrix2d rotation;
	int rows;
	int cols;
	double width;
	Transformation transformationMode;
	float i, j;	
};