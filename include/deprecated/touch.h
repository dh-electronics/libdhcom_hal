/*
 * touch.h
 *
 *  Created on: 07.11.2012
 *      Author: Peter Kishalov
 */


#ifndef TOUCH_H_
#define TOUCH_H_


class Touch
{
public:
	Touch();
	virtual ~Touch();

	virtual void getCoordinates(unsigned int &x, unsigned int &y);
};


/** \file
 *  \brief The definition of Touch class to support the touch screen interface.
 *
 *  \class Touch
 *  \brief Reading the touchscreen coordinates.
 *  The class is virtualized to allow mockups while doing unit testing.
 */


/** \fn Touch::Touch()
 *  \brief Constructs the touch object and connects to the hardware.
 */


/** \fn Touch::~Touch()
 *  \brief Destructs the touch object and disconnects from the hardware.
 */


/** \fn void Touch::getCoordinates(unsigned int &x, unsigned int &y);
 *  \brief Reads the coordinates of the touch.
 *
 */





#endif /* TOUCH_H_ */
