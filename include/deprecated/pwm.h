/*
 * pwm.h
 *
 *  Created on: 07.11.2012
 *      Author: Peter Kishalov
 */

#ifndef PWM_H_
#define PWM_H_


class PWM
{
public:
	PWM();
	virtual ~PWM();

	virtual bool setDivisorAndLimit(unsigned int divisor, unsigned int limit);
	virtual bool setComparisonValue(unsigned int value);
	virtual void enable(bool enabled);
};


/** \file
 *  \brief The PWM class definitions.
 *
 *  \class PWM
 *  \brief The PWM object controls PWM output of the DHCOM module.
 *  The class is virtualized to enable many mockups when testing.
 *  The class does not provide exclusive control of the PWM avoid instantiating it several times (in the tests it is allowed, though).
 */


/** \fn bool PWM::setDivisorAndLimit(unsigned int divisor, unsigned int limit)
 *  \brief setting the frequency divisor of the PWM and the counting limit.
 *  \param divisor - the value of the divisor
 *  \param limit - the counted value after which the counter is reset
 *  \return - true if the parameters are valid, and enabled.
 *  Both parameters directly control the frequency of the PWM output.
 */


/** \fn bool PWM::setComparisonValue(unsigned int value)
 *  \brief setting the comparison value of the PWM.
 *  \param value - the value, which must lie between 0 and the counter limit.
 *  \return true - the value is accepted and set.
 * The value directly influences the PWM pulse width.
 */


/** \fn void PWM::enable(bool enabled)
 *  \brief Enabling or disabling the PWM output.
 *  \param enabled - if true the output is enabled.
 */



#endif /* PWM_H_ */
