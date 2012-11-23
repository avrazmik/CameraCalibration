/**
*	@file Camera.h
*	@brief file contains the definaion of the class Camera.
*	
*	@author Razmik Avetisyan
*/

#pragma once
#include <iostream>
#include <fstream>
#include <math.h>
#include <GL/glut.h>
#include <iomanip>
#include "string"
#include "Matrix2d.h"
#include "MatrixDistortion.h"
#include "MatrixT.h"
#include "Model.h"

using namespace std;

class Camera{
public:
	Camera(int);
	~Camera();

	void render();
	void calculateFOV();
	void renderFrustum(); 
	void renderViewAngle();
	void renderLocalCoordFrame();
	void renderLabel(int);
	void loadModel();
	
	void _setCameraK(string);
	void _setCameraR(string);
	void _setCameraDistortion(string);
	void _setCameraT(string);

	string _getCameraK();
	string _getCameraR();
	string _getCameraDistortion();
	string _getCameraT();
	int getCameraID();
	double hor_FOV;
	double vert_FOV;

	Matrix2d c_K;
	Matrix2d c_R;
	MatrixDistortion c_d;
	MatrixT c_t;
	
	void print();
	void updateTranslation(double, double, double);
	void updateRotation(Matrix2d*);

	void saveCalibrationData(ofstream&, int);
private:
	int camera_id;
	string _distortion;
	string _cam_K;
	string _cam_R;
	string _cam_t;

	Model *model;

};