/**
*	@file Camera.cpp
*	@brief file contains all necessary methods to handle cameras.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "Camera.h"

/** 
 * Default constructor 
 * @author Razmik Avetisyan
 * @param id  Camera identification number
 */
Camera::Camera(int id){	
	camera_id = id;
}


/** 
 * This method is used to set the name of matrix K. 
 * @author Razmik Avetisyan
 * @param K  The name of camera matrix K
 */
void Camera::_setCameraK(string K)
{
	_cam_K = K;
}


/** 
 * This method is used to set the name of matrix R. 
 * @author Razmik Avetisyan
 * @param R  The name of camera matrix R
 */
void Camera::_setCameraR(string R)
{
	_cam_R = R;
}

/** 
 * This method is used to set the name of matrix t. 
 * @author Razmik Avetisyan
 * @param t  The name of camera matrix t
 */
void Camera::_setCameraT(string t)
{
	_cam_t = t;
}

/** 
 * This method is used to set the name of matrix d. 
 * @author Razmik Avetisyan
 * @param t  The name of camera matrix t
 */
void Camera::_setCameraDistortion(string d)
{
	_distortion = d;
}

/** 
 * This method is used to get the name of matrix K. 
 * @author Razmik Avetisyan
 * @return The name of camera matrix K
 */
string Camera::_getCameraK()
{
	return _cam_K;
}

/** 
 * This method is used to get the name of matrix R. 
 * @author Razmik Avetisyan
 * @return The name of camera matrix R
 */
string Camera::_getCameraR()
{
	return _cam_R;
}

/** 
 * This method is used to get the name of matrix d. 
 * @author Razmik Avetisyan
 * @return The name of camera matrix d
 */
string Camera::_getCameraDistortion()
{
	return _distortion;
}

/** 
 * This method is used to get the name of matrix t. 
 * @author Razmik Avetisyan
 * @return The name of camera matrix t
 */
string Camera::_getCameraT()
{
	return _cam_t;
}

/** 
 * This method is used to print out camera details.
 * @author Razmik Avetisyan
 */
void Camera::print()
{
	cout<<"Camera ID - "
		<<this->camera_id
			<<endl
			<<"--------------------"
			<<endl;

		//***************** printing camera K
		cout<<endl
			<<"K"
			<<endl
			<<"----------------------------------------------------------"
			<<endl;
		this->c_K.print();
		//***************** printing camera R
		cout<<endl
			<<"R"
			<<endl
			<<"----------------------------------------------------------"
			<<endl;
		this->c_R.print();
		//***************** printing camera T
		cout<<endl
			<<"T"
			<<endl
			<<"----------------------------------------------------------"
			<<endl;
		this->c_t.print();
		//***************** printing camera Distortion
		cout<<endl
			<<"Distortion"
			<<endl
			<<"----------------------------------------------------------"
			<<endl;
		this->c_d.print();
		
		cout<<endl<<endl
			<<"//////////////////////////////////////////////////////////"
			<<endl
			<<endl;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to update the translation vector of the camera
 * @param dX Value of the 'x' cordinate of thranslation vector
 * @param dY Value of the 'y' cordinate of thranslation vector
 * @param dZ Value of the 'z' cordinate of thranslation vector
 */
void Camera::updateTranslation(double dX, double dY, double dZ)
{
	c_t.updateX(dX);
	c_t.updateY(dY);
	c_t.updateZ(dZ);
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to update the rotation matrix of the camera
 * @param rot Rotation matrix to be updated with
 */
void Camera::updateRotation(Matrix2d* rot)
{
	c_R = c_R * (*rot);
}
