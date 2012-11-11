/**
*	@file LookAt.cpp
*	@brief file contains all necessary methods to work with LookAt matrix.
*	
*	@author Razmik Avetisyan
*/

#include "stdafx.h"
#include "LookAt.h"

/** 
 * Default constructor
 */
LookAt::LookAt()
{
	this->zero();
}

/** 
 * Default constructor
 * @param Array that contains look at matrix values
 */
LookAt::LookAt(GLdouble data[3][3])
{
	set(data);
}


/** 
 * This method is used to call gluLookAt method
 * @author Razmik Avetisyan
 */
void LookAt::doCommand()
{
	gluLookAt
	(
		eyex(), eyey(), eyez(),
		centerx(), centery(), centerz(),
		upx(), upy(), upz()
	);
}

 /** 
 * This method is used to get eyex value
 * @author Razmik Avetisyan
 * @return Returns eyex
 */
GLdouble LookAt::eyex()
{
	return get()[0];
}


 /** 
 * This method is used to get eyey value
 * @author Razmik Avetisyan
 * @return Returns eyey
 */
GLdouble LookAt::eyey()
{
	return get()[1];
}

 /** 
 * This method is used to get eyez value
 * @author Razmik Avetisyan
 * @return Returns eyez
 */
GLdouble LookAt::eyez()
{
	return get()[2];
}

 /** 
 * This method is used to get centerx value
 * @author Razmik Avetisyan
 * @return Returns centerx
 */
GLdouble LookAt::centerx()
{
	return get()[3];
}

 /** 
 * This method is used to get centery value
 * @author Razmik Avetisyan
 * @return Returns centery
 */
GLdouble LookAt::centery()
{
	return get()[4];
}

 /** 
 * This method is used to get centerz value
 * @author Razmik Avetisyan
 * @return Returns centerz
 */
GLdouble LookAt::centerz()
{
	return get()[5];
}

/** 
 * This method is used to get upx value
 * @author Razmik Avetisyan
 * @return Returns upx
 */
GLdouble LookAt::upx()
{
	return get()[6];
}

/** 
 * This method is used to get upy value
 * @author Razmik Avetisyan
 * @return Returns upy
 */
GLdouble LookAt::upy()
{
	return get()[7];	
}

/** 
 * This method is used to get upz value
 * @author Razmik Avetisyan
 * @return Returns upz
 */
GLdouble LookAt::upz()
{
	return get()[8];	
}

 /** 
 * This method is used to set eyex value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::eyex(GLdouble data)
{
	get()[0] = data;

	return *this;
}

 /** 
 * This method is used to set eyey value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::eyey(GLdouble data)
{
	get()[1] = data;

	return *this;
}

 /** 
 * This method is used to set eyez value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::eyez(GLdouble data)
{
	get()[2] = data;

	return *this;
}

 /** 
 * This method is used to set centerx value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::centerx(GLdouble data)
{
	get()[3] = data;

	return *this;
}

 /** 
 * This method is used to set centery value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::centery(GLdouble data)
{
	get()[4] = data;

	return *this;
}


 /** 
 * This method is used to set centerz value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::centerz(GLdouble data)
{
	get()[5] = data;

	return *this;
}

 /** 
 * This method is used to set upx value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::upx(GLdouble data)
{
	get()[6] = data;

	return *this;
}

 /** 
 * This method is used to set upy value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::upy(GLdouble data)
{
	get()[7] = data;

	return *this;
}

 /** 
 * This method is used to set upz value
 * @author Razmik Avetisyan
 * @param The value has to be set
 * @return Returns self
 */
LookAt& LookAt::upz(GLdouble data)
{
	get()[8] = data;

	return *this;
}

