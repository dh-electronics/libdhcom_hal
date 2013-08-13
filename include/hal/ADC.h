/*
 * ADC.h
 *
 *  Created on: Apr 16, 2013
 *      Author: Peter Kishalov (PK), DH electronics GmbH
 */

#ifndef ADC_H_
#define ADC_H_


#include <DHCOM_HAL.h>
#include <hal/Types.h>


namespace dhcom
{


class ADCImpl;
class System;


class DHCOM_HAL ADC
{
public:
	enum CHANNEL
	{
		CHANNEL_0,
		CHANNEL_1,
		CHANNEL_2,
	};

	ADC(const System &sys, CHANNEL channel);
	~ADC();

	void open();
	void close();
	bool isOpen() const;

	uint16_t read() const;

private:
	ADCImpl * const impl_;
};


/** \file
 *  \brief Definitions of the ADC class.
 *
 *  \class ADC
 *  \brief Class provides access to DHCOM module on-board ADC.
 *
 *  Status: Currently the class is not tested, may also not compile or work.
 */

}

#endif /* ADC_H_ */
