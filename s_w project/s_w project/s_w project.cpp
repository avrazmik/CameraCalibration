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
	//scene->getGrid()->updateTranslateY(-5.0);
	lighting();
}

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

	scene->lookAtMatrix().eyex(0.0).eyey(0.0).eyez(-5.0).upy(1.0);
	scene->_glLookAt();

	glMatrixMode (GL_MODELVIEW);
	map<int,Camera *> initial_cameras = scene->getCameras(true);
	glPushMatrix();
	map<int, Camera*> cameras = scene->getCameras(false);


	scene->getGrid()->render();
	glPopMatrix();	

	glColor3f (0.5, 0.0, 1.0);


	
	for(map<int,Camera *>::iterator ii=initial_cameras.begin(); ii!=initial_cameras.end(); ++ii)
	{
		/*
		if (ii == initial_cameras.begin()) {
			Camera *tmp = (*ii).second;
			tmp->c_t.print();
			map<int,Camera *>::iterator it = cameras.begin();
			Camera *tmp1 = (*it).second;
			tmp1->c_t.print();
		}
		*/
		glPushMatrix();
		Camera *tmp = (*ii).second;

		GLdouble *t = tmp->c_t.get();
		
		glTranslated(-t[0], -t[1], -t[2]);

		glMultMatrixd(tmp->c_R.homogeneous());
		
		glScalef(2.0, 1.0, 1.0);
		glutSolidCube(0.2);
		glPopMatrix();
		
	}
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
	scene->handleMainMenu(evnt);
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

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int _tmain(int argc, _TCHAR* argv[])
{	
	scene = Scene::getInstance();

	scene->loadFileIntoMemory("C:\\Users\\Hovo\\Documents\\standard.calib");

	glutInit(&argc, (char**)argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);

	glutCreateWindow ((char*)argv[0]);
	init ();
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


