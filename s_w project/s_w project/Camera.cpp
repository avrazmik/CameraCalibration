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

/** 
 * This method is used to render camera models 
 * @author Mikael Egibyan
 */
void Camera::render()
{
	glPushMatrix();

		GLdouble *t = c_t.get();
		glTranslated(-t[0], -t[1], -t[2]);
		glMultMatrixd(c_R.homogeneous());
		glScalef(0.001, 0.001, 0.001);
		model->render();
	
	glPopMatrix();
}

/** 
 * This method is used to render camera labels 
 * @author Mikael Egibyan
 */
void Camera::renderLabel(int index)
{
	glPushMatrix();

	GLdouble *t = c_t.get();
	glTranslated(-t[0], -t[1], -t[2]);
	glMultMatrixd(c_R.homogeneous());
	char buf[10];
	string cam = "C " + string(itoa(index, buf, 10)); 
	glRasterPos3f(0.0, 0.2, 0.0);
	for (int i = 0; i < cam.length(); i++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, cam[i]);
	}

	glFlush();
	glPopMatrix();
}

/** 
 * @This method is used to render local coordinate system for all cameras 
 * @author Mikael Egibyan
 */
void Camera::renderLocalCoordFrame()
{
	glPushMatrix();

		GLdouble *t = c_t.get();
		glTranslated(-t[0], -t[1], -t[2]);
		glMultMatrixd(c_R.homogeneous());
			glBegin(GL_LINES);	
			// draw 'x' axis
			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.2, 0.0, 0.0);
			glVertex3f(0.0, 0.0, 0.0);
			
			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.2, 0.0, 0.0);
			glVertex3f(0.17, 0.05, 0.0);

			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.2, 0.0, 0.0);
			glVertex3f(0.17, -0.05, 0.0);

			// draw 'y' axis
			glColor3f (1.0, 1.0, 0.0);
			glVertex3f(0.0, 0.2,0.0);
			glVertex3f(0.0, 0.0, 0.0);

			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.0, 0.2, 0.0);
			glVertex3f(0.05, 0.17, 0.0);

			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.0, 0.2, 0.0);
			glVertex3f(-0.05, 0.17, 0.0);

			// draw 'z' axis
			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.0, 0.0, 0.2);
			glVertex3f(0.0, 0.0, 0.0);	

			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.0, 0.0, 0.2);
			glVertex3f(0.05, 0.0, 0.17);

			glColor3f (1.0, 1.0, 1.0);
			glVertex3f(0.0, 0.0, 0.2);
			glVertex3f(-0.05, 0.0, 0.17);
	
			glEnd();
			glFlush();

		glPopMatrix();
}

/** 
 * @This method is used to load camera models 
 * @author Mikael Egibyan
 */
void Camera::loadModel()
{
	model = new Model();
	model->load("camera.obj");
	model->SetDrawOBJ(true);
}

Camera::~Camera()
{
	delete model;
}

/** 
 * @This method is used to calculate camera field of view
 * @author Mikael Egibyan
 */
void Camera::calculateFOV()
{
	GLdouble *t = c_K.get();		
	this->hor_FOV = 2*atan(t[0*3+2]/t[0*3 + 0]);
	this->vert_FOV = 2*atan(t[1*3+2]/t[1*3+1]);
}
/** 
 * @This method is used to render camera's FOV
 * @author Mikael Egibyan
 */
void Camera::renderViewAngle()
{
	glPushMatrix();

		GLdouble *t = c_t.get();
		glTranslated(-t[0], -t[1], -t[2]);
		glMultMatrixd(c_R.homogeneous());

		double k_x, k_y;
		k_x =  t[0*3+0] * tan(hor_FOV/2);
		k_y =  t[0*3+0] * tan(vert_FOV/2);

		glBegin(GL_LINES);

		glColor3f(1.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(k_x, 0.0, t[0*3+0]);

		glColor3f(1.0, 1.0, 0.0);
		glVertex3d(-k_x, 0.0, t[0*3+0]);
		glVertex3d(0.0, 0.0, 0.0);

		glColor3f(0.5, 0.0, 0.5);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, k_y, t[0*3+0]);

		glColor3f(0.5, 0.0, 0.5);
		glVertex3d(0.0, -k_y, t[0*3+0]);
		glVertex3d(0.0, 0.0, 0.0);

		glEnd();

	glFlush();
	glPopMatrix();
}
/** 
 * @This method is used to render camera's frustum with already known FOV
 * @author Mikael Egibyan
 */
void Camera::renderFrustum()
{
	glPushMatrix();

		GLdouble *t = c_t.get();
		glTranslated(-t[0], -t[1], -t[2]);
		glMultMatrixd(c_R.homogeneous());

		double l_x, l_y;
		l_x = t[0*3+0] * 1/(tan(vert_FOV/2)) * tan(hor_FOV/2);
		l_y = t[0*3+0] * 1/(tan(hor_FOV/2)) * tan(vert_FOV/2);

		glColor3f(0.8, 0.0, 0.0);
		glBegin(GL_LINES);

		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(l_x, l_y, t[0*3+0]);

		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(-l_x, l_y, t[0*3+0]);

		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(-l_x, -l_y, t[0*3+0]);

		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(l_x, -l_y, t[0*3+0]);
		
		glVertex3d(l_x, l_y, t[0*3+0]);
		glVertex3d(-l_x, l_y, t[0*3+0]);

		glVertex3d(-l_x, l_y, t[0*3+0]);
		glVertex3d(-l_x, -l_y, t[0*3+0]);

		glVertex3d(-l_x, -l_y, t[0*3+0]);
		glVertex3d(l_x, -l_y, t[0*3+0]);
		
		glVertex3d(l_x, -l_y, t[0*3+0]);
		glVertex3d(l_x, l_y, t[0*3+0]);

		glEnd();

		glFlush();
	glPopMatrix();
}