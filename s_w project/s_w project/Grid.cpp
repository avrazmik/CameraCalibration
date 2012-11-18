/**
*	@file Grid.cpp
*	@brief file contains all necessary methods to work with Grid.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "Grid.h"

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to set the 'x' coordinate of translation vector
 * @param dX Value of the 'x' cordinate of thranslation vector
 */
void Grid::updateTranslateX(double dX)
{
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateTranslation(dX, 0, 0);
	}
	translation.updateX(dX);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to set the 'y' coordinate of translation vector
 * @param dX Value of the 'y' cordinate of thranslation vector
 */
void Grid::updateTranslateY(double dY)
{
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateTranslation(0, dY, 0);
	}
	translation.updateY(dY);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to set the 'z' coordinate of translation vector
 * @param dX Value of the 'z' cordinate of thranslation vector
 */
void Grid::updateTranslateZ(double dZ)
{
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateTranslation(0, 0, dZ);
	}
	translation.updateZ(dZ);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to add rotation around 'x' axis
 * @param angle Value of the angle in degrees
 */
void Grid::updateRotationX(double angle)
{
	Matrix2d* rot = getXRotationMatrix(angle);
	rotation = rotation * (*rot);
	Matrix2d* rot_cam = getXRotationMatrix(-angle);
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateRotation(rot_cam);
	}
	delete rot;
	delete rot_cam;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to add rotation around 'y' axis
 * @param angle Value of the angle in degrees
 */
void Grid::updateRotationY(double angle)
{
	Matrix2d* rot = getYRotationMatrix(angle);
	rotation = rotation * (*rot);
	Matrix2d* rot_cam = getYRotationMatrix(-angle);
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateRotation(rot_cam);		
	}
	delete rot;
	delete rot_cam;
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to add rotation around 'z' axis
 * @param angle Value of the angle in degrees
 */
void Grid::updateRotationZ(double angle)
{
	Matrix2d* rot = getZRotationMatrix(angle);
	rotation = rotation * (*rot);
	Matrix2d* rot_cam = getZRotationMatrix(-angle);
	list<Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		(*it)->updateRotation(rot_cam);
	}
	delete rot;
	delete rot_cam;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to return grid to it's original position
 */
void Grid::returnGridToOrigin()
{
	rotation.identity();
	translation.zero();
}

/** 
 * Default constructor 
 */
Grid::Grid()
{
	rows = 10;
	cols = 10;

	translation.zero();
	rotation.identity();
}

/** 
 * This method is used to render the grid. 
 * @author Mikael Egibyan
 */
void Grid::render()
{
	glPushMatrix();

	GLdouble* t = getTranslationVector().get();
	glMultMatrixd(getRotationMatrix().homogeneous());
	glTranslatef(t[0], t[1], t[2]);
	
	glClear (GL_COLOR_BUFFER_BIT);
	
	//glRotated(45.0, 1, 1, 0);

	glColor3f (0.5, 0.0, 1.0);

    glBegin(GL_LINES);
		for(i=-2.5; i<=2.5; i+=0.5) {
			glVertex3f(i, 0.0, -2.5);
			glVertex3f(i, 0.0, 2.5);
		}

		for(j=-2.5; j<=2.5; j+=0.5) {
			glVertex3f(-2.5, 0.0, j);
			glVertex3f(2.5, 0.0, j);
		}

		if (axes) {
			// draw 'x' axis
			glColor3f (0.0, 1.0, 1.0);
			glVertex3f(-10.0, 0.0, 0.0);
			glVertex3f(10.0, 0.0, 0.0);

			// draw 'y' axis
			glColor3f (0.0, 1.0, 0.0);
			glVertex3f(0.0, -10.0,0.0);
			glVertex3f(0.0, 10.0, 0.0);

			// draw 'z' axis
			glColor3f (1.0, 0.0, 0.0);
			glVertex3f(0.0, 0.0, -10.0);
			glVertex3f(0.0, 0.0, 10.0);
		}
		/*
	glColor3f(1.0, 2.0, 1.0);
    glVertex3f(-6.0, 0.0, 0.0);
   glVertex3f(6.0, 0.0, 0.0);

   glVertex3f(5.0, 0.0, 1.0);
   glVertex3f(6.0, 0.0, 0.0);

   glVertex3f(5.0, 0.0, -1.0);
   glVertex3f(6.0, 0.0, 0.0);
   */
	glEnd();
	glPopMatrix();

	glFlush();
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to add camera to grid
 * @param camera Camera to be added to grid
 */
void Grid::addCamera(Camera* camera)
{
	cameras.push_back(camera);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method-helper to get a rotation matrix representing rotation around 'x' axis
 * @param angle Value of the angle of the rotation in degrees
 */
Matrix2d* Grid::getXRotationMatrix(double angle)
{
	GLdouble rot[3][3] = {0};
	double angle_in_rad = (angle*pi) / 180;
	rot[0][0] = 1;
	rot[1][1] = cos (angle_in_rad);
	rot[1][2] = -sin (angle_in_rad);
	rot[2][1] = sin (angle_in_rad);
	rot[2][2] = cos (angle_in_rad);
	Matrix2d* rot_matrix = new Matrix2d();
	rot_matrix->set(rot);
	return rot_matrix;
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method-helper to get a rotation matrix representing rotation around 'y' axis
 * @param angle Value of the angle of the rotation in degrees
 */
Matrix2d* Grid::getYRotationMatrix(double angle)
{
	GLdouble rot[3][3] = {0};
	double angle_in_rad = (angle*pi) / 180;
	rot[0][0] = cos (angle_in_rad);
	rot[0][2] = sin (angle_in_rad);
	rot[1][1] = 1;
	rot[2][0] = -sin (angle_in_rad);
	rot[2][2] = cos (angle_in_rad);
	Matrix2d* rot_matrix = new Matrix2d();
	rot_matrix->set(rot);
	return rot_matrix;
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method-helper to get a rotation matrix representing rotation around 'z' axis
 * @param angle Value of the angle of the rotation in degrees
 */
Matrix2d* Grid::getZRotationMatrix(double angle)
{
	GLdouble rot[3][3] = {0};
	double angle_in_rad = (angle*pi) / 180;
	rot[0][0] = cos (angle_in_rad);
	rot[0][1] = -sin (angle_in_rad);
	rot[1][0] = sin (angle_in_rad);
	rot[1][1] = cos (angle_in_rad);
	rot[2][2] = 1;
	Matrix2d* rot_matrix = new Matrix2d();
	rot_matrix->set(rot);
	return rot_matrix;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to get a rotation matrix of the grid
 */
Matrix2d Grid::getRotationMatrix()
{
	return rotation;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to get a rotation translation vector of the grid
 */
MatrixT Grid::getTranslationVector()
{
	return translation;
}