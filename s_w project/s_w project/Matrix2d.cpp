/**
*	@file Matrix2d.cpp
*	@brief file contains all necessary methods to work with matrices.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "Matrix2d.h"


/** 
 * Default constructor 
 */
Matrix2d::Matrix2d()
{
}

/** 
 * Constructor 
 * @param data  Two dimentional matrix
 */
Matrix2d::Matrix2d(GLdouble data[3][3])
{

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			m[i][j] = data[i][j];			
		}
	}
}

/** 
 * This method is used to set the matrix. 
 * @author Razmik Avetisyan
 * @param data  Two dimentional data array with values of m
 */
void Matrix2d::set(GLdouble data[3][3])
{

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			m[i][j] = data[i][j];			
		}
	}
}

/** 
 * This method is used to get the matrix. 
 * @author Razmik Avetisyan
 * @return Pointer to the first element of an array
 */
GLdouble* Matrix2d::get()
{
	return &m[0][0];
}


/** 
 * This method is used to print out the matrix data. 
 * @author Razmik Avetisyan
 */
void Matrix2d::print()
{

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			std::cout << std::setw (15);
			std::cout<<m[i][j]<<"  ";
		}
		std::cout<<std::endl;
	}
}

/** 
 * This method is used to overload operator=
 * @author Razmik Avetisyan
 * @param Object Matrix2d
 */
Matrix2d & Matrix2d::operator= (Matrix2d data)
{
	GLdouble *tmp = data.get();

	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			m[i][j] = tmp[i*3 + j];			
		}
	}
	return *this;
}

/** 
 * This method is used to overload operator* (matrix-scalar multiplication)
 * @author Razmik Avetisyan
 * @param The scalar value for multiplication
 * @return Result of multiplication
 */
Matrix2d Matrix2d::operator* (int scalar)
{
	GLdouble temp[3][3];
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			temp[i][j] = m[i][j] * scalar;
		}
	}

	Matrix2d obj(temp);
	return obj;
}

/** 
 * This method is used to overload operator* (matrix-matrix multiplication)
 * @author Razmik Avetisyan
 * @param Object Matrix2d for multiplication
 * @return Result of multiplication
 */
Matrix2d Matrix2d::operator* (Matrix2d data)
{
	GLdouble *tmp = data.get();
	GLdouble temp[3][3];
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			temp[i][j] = (tmp[j] * m[2][i])+ (tmp[1*3 + j] * m[1][i]) + (tmp[2*3 + j] * m[0][i]);

	Matrix2d obj(temp);
	return obj;
}

/** 
 * This method is used to overload operator* (matrix-vector multiplication)
 * @author Razmik Avetisyan
 * @param Object MatrixT for multiplication
 * @return Result of multiplication
 */
GLdouble * Matrix2d::operator* (MatrixT data)
{
	GLdouble *temp = new GLdouble[3];

	GLdouble *matrix = get();
	GLdouble *vector = data.get();

	for(int i=0; i < 3; i++)
	{
		temp[i] = 0;
		for(int j=0; j < 3; j++)
			temp[i] += matrix[i*3 + j] * vector[j];
	}
		
	return temp;
}

/** 
 * This method is used to transpose the matrix
 * @author Razmik Avetisyan
 * @return Result of transpose operation
 */
Matrix2d Matrix2d::transpose()
{
	Matrix2d data = m;
	GLdouble *tmp = data.get();
	int swap;

    for(int i = 0; i < 3; i++)
	   for(int j = i+1; j < 3;j++){
			swap = tmp[i*3+j];
			tmp[i*3+j] = tmp[j*3+i];
			tmp[j*3+i] = swap;
	   } 

	return data;
}

/** 
 * This method is used to make current matrix identity matrix
 * @author Razmik Avetisyan
 */
void Matrix2d::identity()
{
	m[0][0] = 1;	m[0][1] = 0;	m[0][2] = 0;
	m[1][0] = 0;	m[1][1] = 1;	m[1][2] = 0;
	m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 1;
}

/** 
 * This method is used to make current matrix zero matrix
 * @author Razmik Avetisyan
 */
void Matrix2d::zero()
{
	m[0][0] = 0;	m[0][1] = 0;	m[0][2] = 0;
	m[1][0] = 0;	m[1][1] = 0;	m[1][2] = 0;
	m[2][0] = 0;	m[2][1] = 0;	m[2][2] = 0;
}

/** 
 * This method is used to wrap the matrix into homogeneous matrix
 * @author Razmik Avetisyan
 * @return Matrix represented in homogeneous form 4x4
 */
GLdouble * Matrix2d::homogeneous()
{
	GLdouble homogeneous[4][4] = 
		{
			{m[0][0], m[0][1], m[0][2], 0.0},
			{m[1][0], m[1][1], m[1][2], 0.0},
			{m[2][0], m[2][1], m[2][2], 0.0},
			{0.0, 0.0, 0.0, 1.0},
		};

	return 	&homogeneous[0][0];
}
