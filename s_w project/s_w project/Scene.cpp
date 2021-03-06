/**
*	@file Scene.cpp
*	@brief file contains all necessary methods to handle scene.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "Scene.h"

Scene* Scene::instance  = NULL;

/** 
 * This method is used to get the instance of scene (singleton util)
 * @author Razmik Avetisyan
 * @return Pointer to instance of the scene
 */
Scene * Scene::getInstance(){
	if (instance == NULL){
		instance = new Scene();
	}

	return instance;
}

/** 
 * Default constructor 
 * @author Razmik Avetisyan
 */
Scene::Scene(){
	transformationMode = TRANSLATION;
	adjustObject = GRID;
	show_frustums = false;
	show_view_angle = false;
	show_local_coord_axes = true;
	mouse.left_button_pressed = false;
	mouse.middle_button_pressed = false;
	mouse.prev_x_coord = -1;
	mouse.prev_y_coord = -1;
}


/** 
 * This method is used to load calibration file into memory
 * @author Razmik Avetisyan
 */
void Scene::loadFileIntoMemory()
{	
	ifstream indata;
	indata.open(calibrationFile.c_str());

	if(!indata) {
      cerr << "Error: file could not be opened" << endl;
      exit(1);
    }

	string line;
	while (getline(indata, line)){
		file_data.push_back(line);
	}

	loadCalibrationData();

}

/** 
 * Method-wrapper for loading file into memory
 * @author Razmik Avetisyan
 */
void Scene::loadCalibrationData()
{
	loadHeaders();
	loadCameraData();
}

/** 
 * Method-helper for loading headers of calibration file into the memory
 * @author Razmik Avetisyan
 */
void Scene::loadHeaders()
{
	string matrix_name = "";
	vector<string>::iterator ii;
	int index = 0;
	for(ii = file_data.begin(); ii < file_data.end(); ii++)
	{
		string line = *ii;
		if(line.find("]") != string::npos && matrix_name == "nccl_cam_calib_header"){		
			break;
		}
		int pos = line.find("=[");
		if(string::npos != pos)
			matrix_name = line.substr(0,pos);

		if(matrix_name != "nccl_cam_calib_header" || (string::npos != pos) ) continue;
		
		line = line.substr(line.find("\"") + 1, line.length());
		string camera_id = line.substr(0, line.find("\""));
		line = line.substr(line.find("\"") + 1, line.length());

		int cam_id = atoi(camera_id.c_str());
 		Camera *camera = new Camera(cam_id);

		line = line.substr(line.find("\"") + 1, line.length());
		string _distortion = line.substr(0, line.find("\""));
		line = line.substr(line.find("\"") + 1, line.length());
		camera->_setCameraDistortion(_distortion);

		line = line.substr(line.find("\"") + 1, line.length());
		string _cam_K = line.substr(0, line.find("\""));
		line = line.substr(line.find("\"") + 1, line.length());
		camera->_setCameraK(_cam_K);

		line = line.substr(line.find("\"") + 1, line.length());
		string _cam_R = line.substr(0, line.find("\""));
		line = line.substr(line.find("\"") + 1, line.length());
		camera->_setCameraR(_cam_R);

		line = line.substr(line.find("\"") + 1, line.length());
		string _cam_t = line.substr(0, line.find("\""));
		line = line.substr(line.find("\"") + 1, line.length());
		camera->_setCameraT(_cam_t);
		
		cameras[index++] = camera;
		//static_cameras.insert()[cam_id] = new Camera(camera);
		
		//static_cameras.push_back(camera);
	}
}

/** 
 * Method-helper scans the string vector and exracts camera data
 * @author Razmik Avetisyan
 */
void Scene::loadCameraData()
{
	if(getCameraCount() < 1){
		cerr << "Error: file could not be opened" << endl;
		exit(1);	
	}

	string r,k,t,dis;	
	string matrix_name = "";
	for(map<int,Camera *>::iterator ii=cameras.begin(); ii!=cameras.end(); ++ii)
	{
		r = (*ii).second->_getCameraR();
		k = (*ii).second->_getCameraK();
		t = (*ii).second->_getCameraT();
		dis = (*ii).second->_getCameraDistortion();

		vector<string>::iterator jj;
		string line;
		GLdouble r_c[3][3];
		GLdouble k_c[3][3];
		GLdouble dis_c[5];
		GLdouble t_c[3];
		
		int r_row = 0;
		int k_row = 0;
		for(jj = file_data.begin(); jj < file_data.end(); jj++)
		{
			line = *jj;
			int pos = line.find("=[");
			if(string::npos != pos)
				matrix_name = line.substr(0,pos);
			
			if((line.find("=[") == string::npos) && (line.find("]") == string::npos)){
				float p1,p2,p3,p4,p5;
				//************** case when is R matrix
				if(matrix_name == r)
				{					
					sscanf_s(line.c_str(),"%e %e %e", &p1, &p2, &p3);	
					r_c[r_row][0] = p1;
					r_c[r_row][1] = p2;
					r_c[r_row][2] = p3;
					r_row++;
				}	
				//************** case when is K matrix
				if(matrix_name == k)
				{
					sscanf_s(line.c_str(),"%e %e %e", &p1, &p2, &p3);	
					k_c[k_row][0] = p1;
					k_c[k_row][1] = p2;
					k_c[k_row][2] = p3;
					k_row++;
				}
				//************** case when is T matrix
				if(matrix_name == t)
				{
					sscanf_s(line.c_str(),"%e %e %e", &p1, &p2, &p3);	
					t_c[0] = p1;
					t_c[1] = p2;
					t_c[2] = p3;
				}
				//************** case when is distortion matrix
				if(matrix_name == dis)
				{
					sscanf_s(line.c_str(),"%e %e %e %e %e", &p1, &p2, &p3, &p4, &p5);	
					dis_c[0] = p1;
					dis_c[1] = p2;
					dis_c[2] = p3;
					dis_c[3] = p4;
					dis_c[4] = p5;
				}
			}
		}	

		(*ii).second->c_R.set(r_c);
		(*ii).second->c_K.set(k_c);
		(*ii).second->c_d.set(dis_c);
		(*ii).second->c_t.set(t_c);

		static_cameras[(*ii).first] = new Camera(*(*ii).second);
		
	}
	
}

/** 
 * This method is used to return number of cameras
 * @author Razmik Avetisyan
 * @return Number of cameras
 */
int Scene::getCameraCount()
{
	return cameras.size();
}

/** 
 * This method is used to get the map of cameras
 * @author Razmik Avetisyan 
 * @param boolean staticCameras	 Pass true to get static cameras, othewise will return dynamic cameras
 * @return Map of cameras (id , camera *)
 */
map<int, Camera *> Scene::getCameras(bool staticCameras)
{
	if(staticCameras == true) 
		return static_cameras;

	return cameras;
}

/** 
 * This method is used to get camera by id
 * @author Razmik Avetisyan
 * @return Pointer to camera
 */
Camera* Scene::getCameraByID(int camera_id)
{
	for(map<int,Camera *>::iterator ii=cameras.begin(); ii!=cameras.end(); ++ii)
	{
		if((*ii).second->getCameraID() == camera_id)
			return (*ii).second;
	}
	
	return NULL;
}

/** 
 * This method is used to print all cameras within the scene 
 * @author Razmik Avetisyan
 */
void Scene::printCameraData()
{
	for(map<int,Camera *>::iterator ii=cameras.begin(); ii!=cameras.end(); ++ii)
		(*ii).second->print();
}


/** 
 * This method is used to toggle state(show/hide) of frustums 
 * @author Razmik Avetisyan
 */
void Scene::toggleShowFrustums()
{
	show_frustums = !show_frustums;
}

/** 
 * This method is used to toggle state(show/hide) of viewing angle
 * @author Geghetsik Dabagyan
 */
void Scene::toggleShowViewAngle()
{
	show_view_angle = !show_view_angle;
}

/** 
 * This method is used to toggle state(show/hide) of local coordinate axis 
 * @author Razmik Avetisyan
 */
void Scene::toggleShowLocalCoordAxes()
{
	show_local_coord_axes = !show_local_coord_axes;
}

/** 
 * This method is used to get the state of show frustums var
 * @author Razmik Avetisyan
 * @return Returns the state of frustums
 */
bool Scene::getShowFrustumsState()
{
	return show_frustums;
}

/** 
 * This method is used to get the state of show viewing angle var
 * @author Geghetsik Dabaghyan
 */
bool Scene::getShowViewAngle()
{
	return show_view_angle;
}

/** 
 * This method is used to get the state of show local coordinate axes var
 * @author Razmik Avetisyan
 * @return Returns the state of local coordinate axes
 */
bool Scene::getShowLocalCoordAxes()
{
	return show_local_coord_axes;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Keyboard event handler
 * @param key The ASCII code of pressed key
 * @param x,y location of mouse when 'key' was pressed 
 */
void Scene::keyHandler(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 27: {
			exit(0); break;
		}
		case 'f': {
			toggleShowFrustums();			
			break;
		}
		case 'q': {
			toggleShowViewAngle();
			break;
		}
		case 'a': {
			toggleShowLocalCoordAxes();
			break;
		}
		case 'b': {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateZ(-0.3);
			} else if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationZ(-5); // rotation around z counterclockwise
			} else if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				look_at.eyez(look_at.eyez() - 0.3).centerz(look_at.centerz() - 0.3);
			} else if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
			}
			break;
		}
		case 'u': {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateZ(0.3);
			} else if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationZ(5); // rotation around z counterclockwise
			} else if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				look_at.eyez(look_at.eyez() + 0.3).centerz(look_at.centerz() + 0.3);
			} else if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
			}	
			break;
		}
		case 'r': {
			setTransformationMode(ROTATION);
			break;
		}
		case 't': {
			setTransformationMode(TRANSLATION);
			break;
		}
		case 'v': {
			setAdjustObject(VIEW_POINT);
			break;
		}
		case 'g': {
			setAdjustObject(GRID);
			break;
		}
	}
	glutPostRedisplay();
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Keyboard special key event handler
 * @param key The code of pressed specials key
 * @param x,y location of mouse when 'key' was pressed 
 */
void Scene::keySpecialHandler(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_RIGHT: {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateX(-0.3);
			}
			if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationY(-5); // rotation around y clockwise
			}
			if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				// TODO:
				look_at.eyex(look_at.eyex() + 0.3).centerx(look_at.centerx() + 0.3);
			}
			if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
				 rotateViewLeftRight(5);
			}
			break;
		}
		case GLUT_KEY_LEFT: {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateX(0.3);
			}
			if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationY(5); // rotation around y counterclockwise
			}
			if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				look_at.eyex(look_at.eyex() - 0.3).centerx(look_at.centerx() - 0.3);
			}
			if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
				rotateViewLeftRight(-5);
			}
			break;
		}
		case GLUT_KEY_UP: {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateY(0.3);
			}
			if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationX(-5); // rotation around x clockwise
			}
			if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				look_at.eyey(look_at.eyey() - 0.3).centery(look_at.centery() - 0.3);
			}
			if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
				rotateViewUpDown(5);
			}
			break;
		}
		case GLUT_KEY_DOWN: {
			if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				grid.updateTranslateY(-0.3);
			}
			if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
				grid.updateRotationX(5); // rotation around x counterclockwise
			}
			if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
				look_at.eyey(look_at.eyey() + 0.3).centery(look_at.centery() + 0.3);
			}
			if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
				// TODO:
				rotateViewUpDown(-5);
			}
			break;
		}
	}
	glutPostRedisplay();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Mouse button handler
 * @param button The mouse button to be handled
 * @param state The state of the mouse button 
 * @param x,y location of mouse
 */
void Scene::mouseButtonHandler(int button, int state, int x, int y)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON: {
			if (GLUT_DOWN == state) { // the button is pressed
				mouse.left_button_pressed = true;
				mouse.prev_x_coord = x;
				mouse.prev_y_coord = y;
			} else {
				mouse.left_button_pressed = false;
				mouse.prev_x_coord = -1;
				mouse.prev_y_coord = -1;
			}
			break;
		}
		case GLUT_MIDDLE_BUTTON: {
			if (GLUT_DOWN == state) { // the button is pressed
				mouse.middle_button_pressed = true;
				mouse.prev_y_coord = y;
			} else {
				mouse.middle_button_pressed = false;
				mouse.prev_y_coord = -1;
			}
			break;
		}
	}
	glutPostRedisplay();
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Mouse move handler
 * @param x,y location of mouse
 */
void Scene::mouseMoveHandler(int x, int y)
{
	if (mouse.left_button_pressed) {
		if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
			double dX = -(x - mouse.prev_x_coord) * 0.005;
			grid.updateTranslateX(dX);		
			double dY = -(y - mouse.prev_y_coord) * 0.005;
			grid.updateTranslateY(dY);

			mouse.prev_x_coord = x;
			mouse.prev_y_coord = y;
		}
		if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
			double d_angleY = -(x - mouse.prev_x_coord)*0.1;
			if (d_angleY < -0.3 || d_angleY > 0.3) {
				grid.updateRotationY(d_angleY);
				mouse.prev_x_coord = x;
			}
			double d_angleX = (y - mouse.prev_y_coord)*0.1;
			if (d_angleX < -0.3 || d_angleX > 0.3) {
				grid.updateRotationX(d_angleX);
				mouse.prev_y_coord = y;
			}							
		}
		if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {
			double dX = (x - mouse.prev_x_coord) * 0.005;
			look_at.eyex(look_at.eyex() + dX).centerx(look_at.centerx() + dX);		
			double dY = (y - mouse.prev_y_coord) * 0.005;
			look_at.eyey(look_at.eyey() + dY).centery(look_at.centery() + dY);

			mouse.prev_x_coord = x;
			mouse.prev_y_coord = y;
		}
		if (VIEW_POINT == getAdjustObject() && ROTATION == getTransformationMode()) {
			// TODO:
			double d_angleX = -(x - mouse.prev_x_coord)*0.1;
			if (d_angleX < -0.3 || d_angleX > 0.3) {
				rotateViewLeftRight(d_angleX);
				mouse.prev_x_coord = x;
			}
			double d_angleY = (y - mouse.prev_y_coord)*0.1;
			if (d_angleY < -0.3 || d_angleY > 0.3) {
				rotateViewUpDown(d_angleY);
				mouse.prev_y_coord = y;
			}
		}
	}
	if (mouse.middle_button_pressed) {
		if (GRID == getAdjustObject() && TRANSLATION == getTransformationMode()) {
			double dZ = (y - mouse.prev_y_coord) * 0.05;
			grid.updateTranslateZ(-dZ);
			mouse.prev_y_coord = y;
		}
		if (GRID == getAdjustObject() && ROTATION == getTransformationMode()) {
			double d_angleZ = (y - mouse.prev_y_coord)*0.1;
			if (d_angleZ < -0.1 || d_angleZ > 0.1) {
				grid.updateRotationZ(d_angleZ);
				mouse.prev_y_coord = y;
			}
		}
		if (VIEW_POINT == getAdjustObject() && TRANSLATION == getTransformationMode()) {			
			double dZ = -(y - mouse.prev_y_coord) * 0.05;
			look_at.eyez(look_at.eyez() + dZ).centerz(look_at.centerz() + dZ);
			mouse.prev_y_coord = y;
		}		
	}
	glutPostRedisplay();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to connect the cameras with the grid
 */
void Scene::setGrid()
{
	map<int, Camera *>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		grid.addCamera((*it).second);
	}
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to get geometry to be drawn
 * @return Returns pointer to Grid
 */
Grid* Scene::getGrid()
{
	return &grid;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle the main menu events
 */
void Scene::handleMainMenu(int evnt)
{

}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle the transformation mode menu
 */
void Scene::handleTransformationModeMenu(int evnt)
{
	setTransformationMode((Transformation) evnt);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle the transformation mode menu
 */
void Scene::handleFrustumMenu(int evnt)
{
	show_frustums = (State) evnt;
	glutPostRedisplay();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle the viewing angle mode menu
 */
void Scene::handleViewAngleMenu(int evnt)
{
	show_view_angle = (State) evnt;
	glutPostRedisplay();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle menu events to select what is adjusted (grid or view-point)
 */
void Scene::hadleAdjustObjectMenu(int evnt)
{
	setAdjustObject((AdjustObject) evnt);	
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to handle the transformation mode menu
 */
void Scene::handleAxisMenu(int evnt)
{
	show_local_coord_axes = (State) evnt;
	glutPostRedisplay();
}

/** 
 * This method is used to set the look at matrix
 * @author Razmik Avetisyan
 * @param The look at matrix values 3x3
 */
void Scene::lookAtMatrix(GLdouble data[3][3])
{
	look_at.set(data);
	_glLookAt();
}

/** 
 * This method is used to set the look at matrix
 * @author Razmik Avetisyan
 * @param Look at object
 */
void Scene::lookAtMatrix(LookAt data)
{
	look_at = data;
	_glLookAt();
}

/** 
 * This method is used to get the look at matrix
 * @author Razmik Avetisyan
 * @return Returns look at matrix
 */
LookAt & Scene::lookAtMatrix()
{
	return look_at;
}

/** 
 * This method triggers doCommand of look at object
 * @author Razmik Avetisyan
 */
void Scene::_glLookAt()
{
	look_at.doCommand();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to set the transformation mode
 * @param mode Transformation kind to be set
 */
void Scene::setTransformationMode(Transformation mode)
{
	transformationMode = mode;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to get the transformation mode
 */
Transformation Scene::getTransformationMode()
{
	return transformationMode;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to set which object is being adjusted
 * @param mode AdjustObject to be set
 */
void Scene::setAdjustObject(AdjustObject obj)
{
	adjustObject = obj;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to get which object is being adjusted
 */
AdjustObject Scene::getAdjustObject()
{
	return adjustObject;
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to rotate the view point left/right
 */
void Scene::rotateViewLeftRight(double angle)
{
	double posx = look_at.eyex();
	double posy = look_at.eyey();
	double posz = look_at.eyez();

	double viewx = look_at.centerx();
	double viewy = look_at.centery();
	double viewz = look_at.centerz();

	double dist = sqrt((viewx - posx) * (viewx - posx) +
					   (viewy - posy) * (viewy - posy) +
					   (viewz - posz) * (viewz - posz));
	double rad = sqrt((viewx - posx) * (viewx - posx) + 
					  (viewz - posz) * (viewz - posz));
	double B = angle * (rad / dist);
	double sinB = sin(B * pi / 180);
	double cosB = cos(B * pi / 180);

	double newX = posx + (viewx - posx) * cosB - (viewz - posz) * sinB;
	double newY = viewy;
	double newZ = posz + (viewz - posz) * sinB + (viewz - posz) * cosB;
	
	look_at.centerx(newX).centery(newY).centerz(newZ);	
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to rotate the view point up/down
 */
void Scene::rotateViewUpDown(double angle)
{
	double posx = look_at.eyex();
	double posy = look_at.eyey();
	double posz = look_at.eyez();

	double x = look_at.centerx() - posx;
	double y = look_at.centery() - posy;
	double z = look_at.centerz() - posz;

	double dist = sqrt((x * x) + (y * y) + (z * z));
	double sinA = sin(angle * pi / 180);
	double cosA = cos(angle * pi / 180);

	double newy = y * cosA + sqrt((x * x) + (z * z)) * sinA;
	double ratio = (sqrt((dist * dist) - (newy * newy)) / sqrt((dist * dist) - (y * y)));
	double newx = x * ratio;
	double newz = z * ratio;

	look_at.centerx(posx + newx).centery(posy + newy).centerz(posz + newz);

}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to save the calibration data in the file
 */
void Scene::saveCalibrationData()
{
	ofstream file;
	file.open(this->calibrationFile.c_str());
	//file.open(file_name.c_str());
	saveHeaders(file);
	map<int, Camera*>::iterator it;
	map<int, Camera*>::iterator it_static;
	for (it = cameras.begin(), it_static = static_cameras.begin(); 
		 it != cameras.end(); it++, it_static++) {
		(*it).second->saveCalibrationData(file, (*it).first);
	}
	file.close();
	updateStaticCameras();
	grid.returnGridToOrigin();
	glutPostRedisplay();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to save the headers of calibration data in the file
 */
void Scene::saveHeaders(ofstream& file)
{
	if (!file.is_open()) {
		cerr << "Error: Cannot save headers, file is not opened" << endl;
		exit(1);
	}

	file << "nccl_cam_calib_header=[" << endl;
	map<int, Camera*>::iterator it;
	for (it = cameras.begin(); it != cameras.end(); it++) {
		Camera* cam = (*it).second;
		char buf[10];
		string cam_i = "\"cam" + string(itoa((*it).first, buf, 10));
		file << "\"" << cam->getCameraID() << "\" " << cam_i << "_distortion\" " 
			 << cam_i << "_K\" " << cam_i << "_R\" " << cam_i << "_t\";" << endl;
	}

	file << "]" << endl;
}


/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to save the calibration data in the file
 */
void Scene::render()
{	
	grid.render();
	map<int,Camera *> initial_cameras = getCameras(true);
	int i = 0;
	for (map<int, Camera*>::iterator it = initial_cameras.begin(); it != initial_cameras.end(); it++, i++) {
		(*it).second->render();
		(*it).second->renderLabel(i);
		if (this->getShowLocalCoordAxes()) {
			(*it).second->renderLocalCoordFrame();
		}			
		if (this->getShowFrustumsState()) {
			(*it).second->renderFrustum();
		}
		if (this->getShowViewAngle()) {
			(*it).second->renderViewAngle();
		}
	}
	/*if (this->getShowViewAngle()) {
			(*initial_cameras.begin()).second->renderViewAngle();
		}*/
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to save the calibration data in the file
 */
void Scene::updateStaticCameras()
{
	map<int, Camera*>::iterator it;
	map<int, Camera*>::iterator it_static;
	for (it = cameras.begin(), it_static = static_cameras.begin(); it != cameras.end(); it++, it_static++) {
		(*it_static).second->c_K = (*it).second->c_K;
		(*it_static).second->c_R = (*it).second->c_R;
		(*it_static).second->c_d = (*it).second->c_d;
		(*it_static).second->c_t = (*it).second->c_t;
	}
}

/** 
 * @This method is used to set camera models 
 * @author Mikael Egibyan
 */
void Scene::loadCameraModels()
{
	map<int,Camera *> initial_cameras = getCameras(true);
	for (map<int, Camera*>::iterator it = initial_cameras.begin(); it != initial_cameras.end(); it++) {
		(*it).second->loadModel();	
	}
}

/** 
 * This method is used to set the calibration file
 * @author Razmik Avetisyan
 */
void Scene::setCalibrationFile(char *file_path)
{
	calibrationFile = file_path;

	size_t found;
	found  = calibrationFile.find_last_of(".");
	if(found == string::npos)
	{
		cerr << "File extention is not specified" << endl;
		exit(1);
	}

	if(calibrationFile.substr(found+1) != "calib") 
	{
		cerr << "File extention should be .calib" << endl;
		exit(1);
	}

}

/** 
 * @This method is used to show camera labels
 * @author Mikael Egibyan
 * @param  Returns show_labels
 */
bool Scene::getShowLabels()
{
	return show_labels;
}