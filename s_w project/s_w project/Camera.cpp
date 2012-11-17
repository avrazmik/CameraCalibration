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

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method get the camera ID
 * @return Camera ID
 */
int Camera::getCameraID()
{
	return camera_id;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method save the calibration data of the camera into file
 * @param file File to save the calibration data into
 * @param num Number of the camera
 */
void Camera::saveCalibrationData(ofstream& file, int num)
{
	if (!file.is_open()) {
		cerr << "Error: Cannot save headers, file is not opened" << endl;
		exit(1);
	}
	/// Save the camera K matrix
	file << "cam" << num << "_K=[" << endl;
	GLdouble* K = c_K.get();
	file << scientific 
		 << "\t" << K[0] << "  " << K[1] << "  " << K[2] << ";" << endl
	     << "\t" << K[3] << "  " << K[4] << "  " << K[5] << ";" << endl
		 << "\t" << K[6] << "  " << K[7] << "  " << K[8] << endl;
	file << "]" << endl;

	/// Save the camera R matrix
	file << "cam" << num << "_R=[" << endl;
	GLdouble* R = c_R.get();
	file << scientific 
		 << "\t" << R[0] << "  " << R[1] << "  " << R[2] << ";" << endl
	     << "\t" << R[3] << "  " << R[4] << "  " << R[5] << ";" << endl
		 << "\t" << R[6] << "  " << R[7] << "  " << R[8] << endl;
	file << "]" << endl;

	/// Save the camera distortion matrix
	file << "cam" << num << "_distortion=[" << endl;
	GLdouble* d = c_d.get();
	file << scientific << d[0] << " " << d[1] << " " << d[2] << " " << d[3] << " " << d[4] << endl;
	file << "]" << endl;

	/// Save the camera t matrix
	file << "cam" << num << "_t=[" << endl;
	GLdouble* t = c_t.get();
	file << scientific << "\t" << t[0] << "  " << t[1] << "  " << t[2] << endl;
	file << "]" << endl;
}