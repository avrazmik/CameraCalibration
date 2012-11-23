/**
*	@file glm.h
*	@brief file contains the definaion of the class glm.
*	
*	@author Mikael Egibyan
*/

#include "stdafx.h"
#include <GL/glut.h>


#ifndef M_PI
#define M_PI 3.14159265f
#endif

#define GLM_NONE     (0)       
#define GLM_FLAT     (1 << 0)       
#define GLM_SMOOTH   (1 << 1)       
#define GLM_TEXTURE  (1 << 2)      


typedef struct _GLMtriangle {
  GLuint vindices[3];          
  GLuint nindices[3];           
  GLuint tindices[3];         
  GLuint findex;               
  GLuint vecini[3];
  bool visible;
} GLMtriangle;
 
typedef struct _GLMgroup {
  char*             name;           
  GLuint            numtriangles;
  GLuint*           triangles;     
  GLuint            material;   
  struct _GLMgroup* next;    
} GLMgroup;

 
typedef struct _GLMmodel {
  char*    pathname;            
  char*    mtllibname;         

  GLuint   numvertices;       
  GLfloat* vertices;       

  GLuint   numnormals;         
  GLfloat* normals;    

  GLuint   numtexcoords; 
  GLfloat* texcoords;       

  GLuint   numfacetnorms;      
  GLfloat* facetnorms;      

  GLuint       numtriangles;    
  GLMtriangle* triangles;    


  GLuint       numgroups;    
  GLMgroup*    groups;          

  GLfloat position[3];         

} GLMmodel;
struct mycallback
{
	void (*loadcallback)(int,char *);
	int start; 
	int end;
	char *text;
};



GLfloat glmDot(GLfloat* u, GLfloat* v);
GLvoid glmFacetNormals(GLMmodel* model);
GLvoid glmVertexNormals(GLMmodel* model, GLfloat angle);
GLMmodel* glmReadOBJ(char* filename);
GLMmodel* glmReadOBJ(char* filename,mycallback *call);
GLvoid glmDraw(GLMmodel* model, GLuint mode);
GLvoid glmDraw(GLMmodel* model, GLuint mode,char *drawonly);
GLMgroup* glmFindGroup(GLMmodel* model, char* name);
