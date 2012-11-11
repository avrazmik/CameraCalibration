/**
*	@file MatrixDistortion.cpp
*	@brief file contains all necessary methods to work with distortion-type matrices.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "MatrixDistortion.h"

/** 
 * Default constructor 
 */
MatrixDistortion::MatrixDistortion(){}


/** 
 * Constructor
 * @param data  One dimentional data array with values of m
 */
MatrixDistortion::MatrixDistortion(GLdouble data[5]){

	for(int i = 0; i<5 ; i++)	
		m[i] = data[i];

}

/** 
 * This method is used to set the matrix. 
 * @author Razmik Avetisyan
 * @param data  Array with values of m
 */
void MatrixDistortion::set(GLdouble data[5]){

	for(int i = 0; i<5 ; i++)	
		m[i] = data[i];

}

/** 
 * This method is used to get the matrix. 
 * @author Razmik Avetisyan
 * @return Pointer to the first element of an array
 */
GLdouble* MatrixDistortion::get(){
	return &m[0];
}


/** 
 * This method is used to print out the data. 
 * @author Razmik Avetisyan
 */
void MatrixDistortion::print(){

	for(int j = 0; j < 5; j++){
		std::cout << std::setw (10);
		std::cout<<m[j]<<"  ";
	}
	std::cout<<std::endl;
}

/** 
 * This method is used to overload operator=
 * @author Razmik Avetisyan
 * @param Object MatrixDistortion
 */
void MatrixDistortion::operator= (MatrixDistortion data){

	GLdouble *tmp = data.get();
	for(int i = 0; i < 5; i++)
		m[i] = tmp[i];
}

/** 
 * This method is used to overload operator* (matrix-scalar multiplication)
 * @author Razmik Avetisyan
 * @param The scalar value for multiplication
 * @return Result of multiplication
 */
MatrixDistortion MatrixDistortion::operator* (int scalar)
{
	GLdouble temp[5];
	for(int i = 0; i < 5; i++){
		temp[i] = m[i] * scalar; 
	}

	MatrixDistortion obj(temp);
	return obj;

}
