/**
*	@file MatrixT.cpp
*	@brief file contains all necessary methods to work with t matrices.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "MatrixT.h"

/** 
 * Default constructor 
 * @author Razmik Avetisyan
 */
MatrixT::MatrixT(){}


/** 
 * Constructor 
 * @author Razmik Avetisyan
 * @param data  One dimentional data array with values of m
 */
MatrixT::MatrixT(GLdouble data[3]){

	for(int i = 0; i<3 ; i++)	
		m[i] = data[i];

}

/** 
 * This method is used to set the matrix. 
 * @author Razmik Avetisyan
 * @param data  Array with values of m
 */
void MatrixT::set(GLdouble data[3]){

	for(int i = 0; i<3 ; i++)	
		m[i] = data[i];

}

/** 
 * This method is used to get the matrix. 
 * @author Razmik Avetisyan
 * @return Pointer to the first element of an array
 */
GLdouble* MatrixT::get(){
	return &m[0];
}


/** 
 * This method is used to print out the data. 
 * @author Razmik Avetisyan
 */
void MatrixT::print(){

	for(int j = 0; j < 3; j++){
		std::cout << std::setw (10);
		std::cout<<m[j]<<"  ";
	}
	std::cout<<std::endl;

}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to update the 'x' coordinate of MatrixT
 * @param dX Value 'x' coordinate should be updated with
 */
void MatrixT::updateX(double dX)
{
	m[0] += dX;
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to update the 'y' coordinate of MatrixT
 * @param dY Value 'y' coordinate should be updated with
 */
void MatrixT::updateY(double dY)
{
	m[1] += dY;
}
/**
 * @author: Geghetsik Dabaghyan
 * @brief Method to update the 'z' coordinate of MatrixT
 * @param dZ Value 'z' coordinate should be updated with
 */
void MatrixT::updateZ(double dZ)
{
	m[2] += dZ;
}

/** 
 * This method is used to make current matrix zero matrix
 * @author Razmik Avetisyan
 */
void MatrixT::zero()
{
	m[0] = 0;	m[1] = 0;	m[2] = 0;
}