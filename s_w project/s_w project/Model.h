/**
*	@file Model.h
*	@brief file contains the definaion of the class Model.
*	
*	@author Mikael Egibyan
*/


#include "stdafx.h"
#include "glm.h"
#include <GL/glut.h>
#include <assert.h>

class Model
{
private:

	GLMmodel* pmodel;
	bool drawOBJ;
	
public:

	void SetDrawOBJ(bool draw)
	{
		drawOBJ = draw;
	}

	void load(char* filename)
	{
		pmodel = glmReadOBJ(filename);
		glmFacetNormals(pmodel);        
		glmVertexNormals(pmodel, 90.0);
		assert(NULL != pmodel);
	}

	void render()
	{
		if(drawOBJ == true)
		{
			assert(0 != pmodel->vertices); 
			glmDraw(pmodel, GLM_SMOOTH);
		}
	}
	Model() : pmodel(NULL){}	
	virtual ~Model(){}	  
};