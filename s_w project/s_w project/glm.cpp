/** 
 * @This method describes glm class.
 * @This code is written using following links:
 * @param http://www.teina.org/TER_3DModeler/glm_8cpp.html
 * @param http://code.google.com/p/blue-game-of-death/source/browse/glm/glm.cpp?r=bc2eb29c6c9dc755ae9a73da2cb190c52975fdac
 * @author Mikael Egibyan
 */


#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "glm.h"

#include <iostream>
using namespace std;

#define T(x) (model->triangles[(x)])

/** 
 * @This method is creating _GLMnode structure
 * @author Mikael Egibyan
 */
typedef struct _GLMnode 
{
    GLuint         index;
    GLboolean      averaged;
    struct _GLMnode* next;
} GLMnode;

/** 
 * @This method is used to determinate maximum ellement
 * @author Mikael Egibyan
 */
static GLfloat glmMax(GLfloat a, GLfloat b) 
{
    if (b > a)
        return b;
    return a;
}

/** 
 * @This method is used to determine if ellement is less than zero, returns absolute value 
 * @author Mikael Egibyan
 */
static GLfloat glmAbs(GLfloat f)
{
    if (f < 0)
        return -f;
    return f;
}

/** 
 * @This method is used to calculate the dot product of 2 vectors 
 * @author Mikael Egibyan
 */
GLfloat glmDot(GLfloat* u, GLfloat* v)
{
    assert(u); assert(v);
    
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

/** 
 * @This method is used to defign cross product for 2 vectors
 * @author Mikael Egibyan
 */
static GLvoid glmCross(GLfloat* u, GLfloat* v, GLfloat* n)
{
    assert(u); assert(v); assert(n);
    
    n[0] = u[1]*v[2] - u[2]*v[1];
    n[1] = u[2]*v[0] - u[0]*v[2];
    n[2] = u[0]*v[1] - u[1]*v[0];
}

/** 
 * @This method is used for normalization
 * @author Mikael Egibyan
 */
static GLvoid glmNormalize(GLfloat* v)
{
    GLfloat l;
    
    assert(v);
    
    l = (GLfloat)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;
}

/** 
 * @This method is used to return equal ellements
 * @author Mikael Egibyan
 */
static GLboolean
glmEqual(GLfloat* u, GLfloat* v, GLfloat epsilon)
{
    if (glmAbs(u[0] - v[0]) < epsilon &&
        glmAbs(u[1] - v[1]) < epsilon &&
        glmAbs(u[2] - v[2]) < epsilon) 
    {
        return GL_TRUE;
    }
    return GL_FALSE;
}

/** 
 * @This method is used to find a group in the model
 * @author Mikael Egibyan
 */
GLMgroup*
glmFindGroup(GLMmodel* model, char* name)
{
    GLMgroup* group;
    
    assert(model);
    
    group = model->groups;
    while(group) {
        if (!strcmp(name, group->name))			
            break;
        group = group->next;
    }
    
    return group;
}

/** 
 * @This method is used to add a group to the model
 * @author Mikael Egibyan
 */
GLMgroup* glmAddGroup(GLMmodel* model, char* name)
{
    GLMgroup* group;
    
    group = glmFindGroup(model, name);
    if (!group) {
        group = (GLMgroup*)malloc(sizeof(GLMgroup));
        group->name = strdup(name);
        group->material = 0;
        group->numtriangles = 0;
        group->triangles = NULL;
        group->next = model->groups;
        model->groups = group;
        model->numgroups++;
    }
    
    return group;
}


/** 
 * @This method is used to make a first pass through the file to get a count of the number of vertices, normals
 * @author Mikael Egibyan
 */
static GLvoid glmFirstPass(GLMmodel* model, FILE* file, mycallback *call) 
{
    GLuint  numvertices;        
    GLuint  numnormals;         
    GLuint  numtexcoords;       
    GLuint  numtriangles;       
    GLMgroup* group;            
    unsigned    v, n, t;
    char        buf[128];
    
	/*@make a default group*/
    group = glmAddGroup(model, "default");
    
    numvertices = numnormals = numtexcoords = numtriangles = 0;
    while(fscanf(file, "%s", buf) != EOF) 
	{
        switch(buf[0]) 
		{
        case '#':               
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':               
            switch(buf[1]) 
			{
            case '\0':         
                fgets(buf, sizeof(buf), file);
                numvertices++;
                break;
            case 'n':           
                fgets(buf, sizeof(buf), file);
                numnormals++;
                break;
            case 't':           
                fgets(buf, sizeof(buf), file);
                numtexcoords++;
                break;
            default:
                printf("glmFirstPass(): Unknown token \"%s\".\n", buf);
                exit(1);
                break;
            }
            break;


/** 
 * @This method is used to load object to buffer, viewing it as triangles
 * @author Mikael Egibyan
 */
#if SINGLE_STRING_GROUP_NAMES
                sscanf(buf, "%s", buf);
#else
                buf[strlen(buf)-1] = '\0'; 
#endif
                group = glmAddGroup(model, buf);
                break;
            case 'f':               
                v = n = t = 0;
                fscanf(file, "%s", buf);
                if (strstr(buf, "//")) 
				{
                    sscanf(buf, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) 
					{
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
				else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) 
				{
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) 
					{
                        numtriangles++;
                        group->numtriangles++;
                    }
                } 
				else if (sscanf(buf, "%d/%d", &v, &t) == 2)
				{
                    fscanf(file, "%d/%d", &v, &t);
                    fscanf(file, "%d/%d", &v, &t);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0)
					{
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
				else 
				{
                    fscanf(file, "%d", &v);
                    fscanf(file, "%d", &v);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) 
					{
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                break;
                
            default:
                fgets(buf, sizeof(buf), file);
                break;
        }
  }
  
  model->numvertices  = numvertices;
  model->numnormals   = numnormals;
  model->numtexcoords = numtexcoords;
  model->numtriangles = numtriangles;
  
  group = model->groups;
  while(group) {
      group->triangles = (GLuint*)malloc(sizeof(GLuint) * group->numtriangles);
      group->numtriangles = 0;
      group = group->next;
  }
}

/** 
 * @This method is used to get all data
 * @author Mikael Egibyan
 */
static GLvoid glmSecondPass(GLMmodel* model, FILE* file, mycallback *call) 
{
    GLuint  numvertices;      
    GLuint  numnormals;       
    GLuint  numtexcoords;      
    GLuint  numtriangles;       
    GLfloat*    vertices;           
    GLfloat*    normals;    
    GLfloat*    texcoords;     
    GLMgroup* group;           
    GLuint  material;          
    GLuint  v, n, t;
    char        buf[128];
	char afis[80];
    
    vertices       = model->vertices;
    normals    = model->normals;
    texcoords    = model->texcoords;
    group      = model->groups;

    numvertices = numnormals = numtexcoords = 1;
    numtriangles = 0;
    material = 0;
    while(fscanf(file, "%s", buf) != EOF) 
	{
        switch(buf[0])
		{
        case '#':    
            fgets(buf, sizeof(buf), file);
            break;
        case 'v':             
            switch(buf[1])
			{
            case '\0':       
                fscanf(file, "%f %f %f", 
                    &vertices[3 * numvertices + 0], 
                    &vertices[3 * numvertices + 1], 
                    &vertices[3 * numvertices + 2]);
                numvertices++;
				if (numvertices%200==0)
					if (call)
					{
						sprintf(afis,"%s (%s )... ",call->text, group->name);
						int procent = ((float)((float)numvertices*70/model->numvertices+30)/100)*(call->end-call->start)+call->start;
						call->loadcallback(procent,afis); 
					}
                break;
            case 'n':        
                fscanf(file, "%f %f %f", 
                    &normals[3 * numnormals + 0],
                    &normals[3 * numnormals + 1], 
                    &normals[3 * numnormals + 2]);
                numnormals++;
                break;
            case 't':     
                fscanf(file, "%f %f", 
                    &texcoords[2 * numtexcoords + 0],
                    &texcoords[2 * numtexcoords + 1]);
                numtexcoords++;
                break;
            }
            break;

/** 
 * @This method is used to indicize model
 * @author Mikael Egibyan
 */
#if SINGLE_STRING_GROUP_NAMES
                sscanf(buf, "%s", buf);
#else
                buf[strlen(buf)-1] = '\0'; 
#endif
                group = glmFindGroup(model, buf);
                group->material = material;
                break;
            case 'f':           
                v = n = t = 0;
		T(numtriangles).findex = -1;
		T(numtriangles).vecini[0]=-1;
		T(numtriangles).vecini[1]=-1;
		T(numtriangles).vecini[2]=-1;
                fscanf(file, "%s", buf);
                if (strstr(buf, "//")) {
                    sscanf(buf, "%d//%d", &v, &n);
                    T(numtriangles).vindices[0] = v;
					if (n== 181228)
					{
						printf("");
					}
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0)
					{						
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } 
				else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) 
				{		
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) 
					{
						if (n== 181228)					
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } 
				else if (sscanf(buf, "%d/%d", &v, &t) == 2) 
				{				
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) 
					{						
					    T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } 
				else 
				{			
                    sscanf(buf, "%d", &v);
                    T(numtriangles).vindices[0] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[1] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[2] = v;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) 
					{
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).vindices[2] = v;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                }
                break;
                
            default:
                fgets(buf, sizeof(buf), file);
                break;
    }
  }
}

/** 
 * @This method is used to generate facet normals for a model
 * @author Mikael Egibyan
 */
GLvoid glmFacetNormals(GLMmodel* model)
{
    GLuint  i;
    GLfloat u[3];
    GLfloat v[3];
    
    assert(model);
    assert(model->vertices);
    
    if (model->facetnorms)
        free(model->facetnorms);
    
    model->numfacetnorms = model->numtriangles;
    model->facetnorms = (GLfloat*)malloc(sizeof(GLfloat) *
                       3 * (model->numfacetnorms + 1));
    
    for (i = 0; i < model->numtriangles; i++) 
	{
        model->triangles[i].findex = i+1;		
        
        u[0] = model->vertices[3 * T(i).vindices[1] + 0] -
            model->vertices[3 * T(i).vindices[0] + 0];
        u[1] = model->vertices[3 * T(i).vindices[1] + 1] -
            model->vertices[3 * T(i).vindices[0] + 1];
        u[2] = model->vertices[3 * T(i).vindices[1] + 2] -
            model->vertices[3 * T(i).vindices[0] + 2];
        
        v[0] = model->vertices[3 * T(i).vindices[2] + 0] -
            model->vertices[3 * T(i).vindices[0] + 0];
        v[1] = model->vertices[3 * T(i).vindices[2] + 1] -
            model->vertices[3 * T(i).vindices[0] + 1];
        v[2] = model->vertices[3 * T(i).vindices[2] + 2] -
            model->vertices[3 * T(i).vindices[0] + 2];
        
        glmCross(u, v, &model->facetnorms[3 * (i+1)]);
        glmNormalize(&model->facetnorms[3 * (i+1)]);
    }
}

/** 
 * @This method is used to generate vertex normals for a model
 * @author Mikael Egibyan
 */
GLvoid glmVertexNormals(GLMmodel* model, GLfloat angle)
{
    GLMnode*    node;
    GLMnode*    tail;
    GLMnode** members;
    GLfloat*    normals;
    GLuint  numnormals;
    GLfloat average[3];
    GLfloat dot, cos_angle;
    GLuint  i, avg;
    
    assert(model);
    assert(model->facetnorms);
    
    cos_angle = cos(angle * M_PI / 180.0);
    
    if (model->normals)
        free(model->normals);
    
    model->numnormals = model->numtriangles * 3; 
    model->normals = (GLfloat*)malloc(sizeof(GLfloat)* 3* (model->numnormals+1));
    
    members = (GLMnode**)malloc(sizeof(GLMnode*) * (model->numvertices + 1));
    for (i = 1; i <= model->numvertices; i++)
        members[i] = NULL;
    
    for (i = 0; i < model->numtriangles; i++) {
        node = (GLMnode*)malloc(sizeof(GLMnode));
        node->index = i;
        node->next  = members[T(i).vindices[0]];
        members[T(i).vindices[0]] = node;
        
        node = (GLMnode*)malloc(sizeof(GLMnode));
        node->index = i;
        node->next  = members[T(i).vindices[1]];
        members[T(i).vindices[1]] = node;
        
        node = (GLMnode*)malloc(sizeof(GLMnode));
        node->index = i;
        node->next  = members[T(i).vindices[2]];
        members[T(i).vindices[2]] = node;
    }
    
    numnormals = 1;
    for (i = 1; i <= model->numvertices; i++) {
        node = members[i];
        /*if (!node)
            fprintf(stderr, "glmVertexNormals(): vertex w/o a triangle\n");*/
        average[0] = 0.0; average[1] = 0.0; average[2] = 0.0;
        avg = 0;
        while (node) {
            dot = glmDot(&model->facetnorms[3 * T(node->index).findex],
                &model->facetnorms[3 * T(members[i]->index).findex]);
            if (dot > cos_angle) {
                node->averaged = GL_TRUE;
                average[0] += model->facetnorms[3 * T(node->index).findex + 0];
                average[1] += model->facetnorms[3 * T(node->index).findex + 1];
                average[2] += model->facetnorms[3 * T(node->index).findex + 2];
                avg = 1;           
            } else {
                node->averaged = GL_FALSE;
            }
            node = node->next;
        }
        
        if (avg) {
            glmNormalize(average);
            
            model->normals[3 * numnormals + 0] = average[0];
            model->normals[3 * numnormals + 1] = average[1];
            model->normals[3 * numnormals + 2] = average[2];
            avg = numnormals;
            numnormals++;
        }
        
        node = members[i];
        while (node) 
		{
			if (node->index==204543)
				{
					printf("");
				}
            if (node->averaged) {
				
                if (T(node->index).vindices[0] == i)
                    T(node->index).nindices[0] = avg;
                else if (T(node->index).vindices[1] == i)
                    T(node->index).nindices[1] = avg;
                else if (T(node->index).vindices[2] == i)
                    T(node->index).nindices[2] = avg;
            } else {
				
                model->normals[3 * numnormals + 0] = 
                    model->facetnorms[3 * T(node->index).findex + 0];
                model->normals[3 * numnormals + 1] = 
                    model->facetnorms[3 * T(node->index).findex + 1];
                model->normals[3 * numnormals + 2] = 
                    model->facetnorms[3 * T(node->index).findex + 2];
                if (T(node->index).vindices[0] == i)
                    T(node->index).nindices[0] = numnormals;
                else if (T(node->index).vindices[1] == i)
                    T(node->index).nindices[1] = numnormals;
                else if (T(node->index).vindices[2] == i)
                    T(node->index).nindices[2] = numnormals;
                numnormals++;
            }
            node = node->next;
        }
    }
    
    model->numnormals = numnormals - 1;
    for (i = 1; i <= model->numvertices; i++) {
        node = members[i];
        while (node) {
            tail = node;
            node = node->next;
            free(tail);
        }
    }
    free(members);

    normals = model->normals;
    model->normals = (GLfloat*)malloc(sizeof(GLfloat)* 3* (model->numnormals+1));
    for (i = 1; i <= model->numnormals; i++) {
        model->normals[3 * i + 0] = normals[3 * i + 0];
        model->normals[3 * i + 1] = normals[3 * i + 1];
        model->normals[3 * i + 2] = normals[3 * i + 2];
    }
    free(normals);
}

/** 
 * @This method is used to read a model description from obj file
 * @author Mikael Egibyan
 */
GLMmodel* glmReadOBJ(char* filename)
{
	return glmReadOBJ(filename,0);
}
GLMmodel* glmReadOBJ(char* filename,mycallback *call)
{
    GLMmodel* model;
    FILE*   file;
    file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "glmReadOBJ() failed: can't open data file \"%s\".\n",
            filename);
        exit(1);
    }
    
    model = (GLMmodel*)malloc(sizeof(GLMmodel));
    model->pathname    = strdup(filename);
    model->mtllibname    = NULL;
    model->numvertices   = 0;
    model->vertices    = NULL;
    model->numnormals    = 0;
    model->normals     = NULL;
    model->numtexcoords  = 0;
    model->texcoords       = NULL;
    model->numfacetnorms = 0;
    model->facetnorms    = NULL;
    model->numtriangles  = 0;
    model->triangles       = NULL;
    model->numgroups       = 0;
    model->groups      = NULL;
    model->position[0]   = 0.0;
    model->position[1]   = 0.0;
    model->position[2]   = 0.0;

    glmFirstPass(model, file, call);
    
    model->vertices = (GLfloat*)malloc(sizeof(GLfloat) *
        3 * (model->numvertices + 1));
    model->triangles = (GLMtriangle*)malloc(sizeof(GLMtriangle) *
        model->numtriangles);
    if (model->numnormals) {
        model->normals = (GLfloat*)malloc(sizeof(GLfloat) *
            3 * (model->numnormals + 1));
    }
    if (model->numtexcoords) {
        model->texcoords = (GLfloat*)malloc(sizeof(GLfloat) *
            2 * (model->numtexcoords + 1));
    }
    
    rewind(file);
    
    glmSecondPass(model, file,call);
    
    fclose(file);
    
    return model;
}

/** 
 * @This method is used to draw model
 * @author Mikael Egibyan
 */
GLvoid glmDraw(GLMmodel* model, GLuint mode)
{
	glmDraw(model,mode,0);
}
GLvoid glmDraw(GLMmodel* model, GLuint mode,char *drawonly)
{
    static GLuint i;
    static GLMgroup* group;
    static GLMtriangle* triangle;
    GLuint IDTextura;
    
    assert(model);
    assert(model->vertices);
    
    if (mode & GLM_FLAT && !model->facetnorms) {
        printf("glmDraw() warning: flat render mode requested "
            "with no facet normals defined.\n");
        mode &= ~GLM_FLAT;
    }
    if (mode & GLM_SMOOTH && !model->normals) {
        printf("glmDraw() warning: smooth render mode requested "
            "with no normals defined.\n");
        mode &= ~GLM_SMOOTH;
    }
    if (mode & GLM_TEXTURE && !model->texcoords) {
        printf("glmDraw() warning: texture render mode requested "
            "with no texture coordinates defined.\n");
        mode &= ~GLM_TEXTURE;
    }
    if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
        printf("glmDraw() warning: flat render mode requested "
            "and smooth render mode requested (using smooth).\n");
        mode &= ~GLM_FLAT;
    }

    IDTextura = -1;
    group = model->groups;
    while (group) 
	{
		if (drawonly)
			if (strcmp(group->name,drawonly))
			{
				group=group->next;
				continue;
			}		
        
        glBegin(GL_TRIANGLES);
        for (i = 0; i < group->numtriangles; i++) {
            triangle = &T(group->triangles[i]);

            if (mode & GLM_FLAT)
                glNormal3fv(&model->facetnorms[3 * triangle->findex]);
            
            if (mode & GLM_SMOOTH)
                glNormal3fv(&model->normals[3 * triangle->nindices[0]]);
            if (mode & GLM_TEXTURE)
                glTexCoord2fv(&model->texcoords[2 * triangle->tindices[0]]);
            glVertex3fv(&model->vertices[3 * triangle->vindices[0]]);
            
            if (mode & GLM_SMOOTH)
                glNormal3fv(&model->normals[3 * triangle->nindices[1]]);
            if (mode & GLM_TEXTURE)
                glTexCoord2fv(&model->texcoords[2 * triangle->tindices[1]]);
            glVertex3fv(&model->vertices[3 * triangle->vindices[1]]);
            
            if (mode & GLM_SMOOTH)
                glNormal3fv(&model->normals[3 * triangle->nindices[2]]);
            if (mode & GLM_TEXTURE)
                glTexCoord2fv(&model->texcoords[2 * triangle->tindices[2]]);
            glVertex3fv(&model->vertices[3 * triangle->vindices[2]]);
            
        }
        glEnd();
        
        group = group->next;
    }
}

