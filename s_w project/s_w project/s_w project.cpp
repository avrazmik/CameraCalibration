//------------------------------------------------------------------------------
//  (c) 2012 Razmik Avetisyan
//  University of Rostock
//
//  author: Razmik Avetisyan (razmik<dot>avetisyan<at>uni-rostock<dot>de)
//  version: 1.0.0
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <map>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

#include "Scene.h"
#include "Camera.h"

using namespace std;

Scene *scene;

const GLfloat light_ambient[]  = { 1.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 5.0f, 0.0f, 0.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 50.0f };
void lighting();

void init(void) 
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glShadeModel (GL_FLAT);
	scene->setGrid();
	scene->loadCameraModels();
	scene->getShowFrustumsState();
	lighting();
	scene->lookAtMatrix().eyex(0.0).eyey(0.0).eyez(-5.0).centerz(-1).upy(1.0);
}


/**
 * @author: Razmik Avetisyan
 * @brief Function is use to enable light
 */
void lighting()
{
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glEnable(GL_LIGHT0);
}

void display(void)
{	
	glLoadIdentity();

	
	scene->_glLookAt();

	glMatrixMode (GL_MODELVIEW);
	scene->render();

	glutSwapBuffers();	
	glFlush();
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to resize the application window
 */
void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ();
	glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 100.0);
	glMatrixMode (GL_MODELVIEW);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function handle keyboard events
 */
void keyboard(unsigned char key, int x, int y)
{
	scene->keyHandler(key, x, y);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle keyboard special key events
 */
void keyboardSpecial(int key, int x, int y)
{
	scene->keySpecialHandler(key, x, y);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle mouse clicks
 */
void mouse(int button, int state, int x, int y)
{
	scene->mouseButtonHandler(button, state, x, y);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle mouse movement
 */
void mouseMove(int x, int y) 
{
	scene->mouseMoveHandler(x, y);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle main menue events
 */
void processMainMenu(int evnt)
{
	//scene->handleMainMenu(evnt);
	if (ON == evnt) {
		scene->saveCalibrationData();
	}
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle transformation mode menue events
 */
void processTransformationModeMenu(int evnt)
{
	scene->handleTransformationModeMenu(evnt);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle frustum visibility menu events
 */
void processFrustumMenu(int evnt)
{
	scene->handleFrustumMenu(evnt);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle viewing anle menu events
 */
void processViewAngleMenu(int evnt)
{
	scene->handleViewAngleMenu(evnt);
}


/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle axis visibility menu events
 */
void processAxisMenu(int evnt)
{
	scene->handleAxisMenu(evnt);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to handle menu events to select what is adjusted (grid or view-point)
 */
void processAdjustObjectMenu(int evnt)
{
	scene->hadleAdjustObjectMenu(evnt);
}

/**
 * @author: Geghetsik Dabaghyan
 * @brief Function to create the menues
 */
void createPopupMenu()
{
	scene->menues.axisMenuID = glutCreateMenu(processAxisMenu);
	glutAddMenuEntry("On", ON);
	glutAddMenuEntry("Off", OFF);

    scene->menues.view_agleMenuID = glutCreateMenu(processViewAngleMenu);
	glutAddMenuEntry("On", ON);
	glutAddMenuEntry("Off", OFF);

	scene->menues.frustumMenuID = glutCreateMenu(processFrustumMenu);
	glutAddMenuEntry("On", ON);
	glutAddMenuEntry("Off", OFF);

	scene->menues.transformationModeMenuID = glutCreateMenu(processTransformationModeMenu);
	glutAddMenuEntry("Translation ('t' key)", TRANSLATION);
	glutAddMenuEntry("Rotation ('r' key)", ROTATION);

	scene->menues.adjustGrid_ViewPointID = glutCreateMenu(processAdjustObjectMenu);
	glutAddMenuEntry("Adjust grid ('g' key)", GRID);
	glutAddMenuEntry("Adjust view-point ('v' key)", VIEW_POINT);
	
	scene->menues.mainMenuID = glutCreateMenu(processMainMenu);
	glutAddSubMenu("Adjust Grid/View-point", scene->menues.adjustGrid_ViewPointID);
	glutAddSubMenu("Transformation mode", scene->menues.transformationModeMenuID);
	glutAddSubMenu("Show axis ('a' key)", scene->menues.axisMenuID);
	glutAddSubMenu("Show frustum ('f' key)", scene->menues.frustumMenuID);	
	glutAddSubMenu("Show viewing angle ('q' key)", scene->menues.view_agleMenuID);
	glutAddMenuEntry("Save current calibration state ('s' key)", ON);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char * argv[])
{	
	scene = Scene::getInstance();

	if(argc > 1 && argv[1] != NULL)
	{
		scene->setCalibrationFile(argv[1]);
	}
	else
	{
		scene->setCalibrationFile("C:\\Users\\Razmik\\software project\\Uni Rostock\\swp\\calibrationdata\\standard.calib");
		//scene->setCalibrationFile("C:\\Users\\Hovo\\Documents\\standard.calib");
	}
	
	scene->loadFileIntoMemory();

	glutInit(&argc, (char**)argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);

	glutCreateWindow ((char*)argv[0]);
	init();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);

	createPopupMenu();
	glutMainLoop();
	
	return 0;
}


