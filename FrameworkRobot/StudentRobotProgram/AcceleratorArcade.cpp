#include "AcceleratorArcade.h"

AcceleratorArcade::AcceleratorArcade(float multiplier)
:	m_turnVelocity(0)
,	m_accelerator_multiplier(multiplier)
	{

	}

#if 1

NumericalVector<2> AcceleratorArcade::update(float throttle, float axis, RobotModel::GearSet gear, bool bQuickTurn)
{

	NumericalVector<2> driveVec;
	bool isQuickTurn = bQuickTurn;
	bool isHighGear = (gear == RobotModel::kHighGear) ? true : false ;

	double wheel = axis;
	double wheelNonLinearity;

	if(fabs(wheel) < 0.1) //If user does not want to steer, stop and cancel acceleration
		wheel = 0.0;
	
	static double old_wheel = 0.0;
	double neg_inertia = wheel - old_wheel;
	old_wheel = wheel;
	
	/***
	if (isHighGear) {
		wheelNonLinearity = 0.9 ; // ("TURN_NONLIN_HIGH", 0.9);
		// Apply a sin function that's scaled to make it feel better.
		wheel = sin((M_PI / 2.0) * wheelNonLinearity * wheel) / sin((M_PI / 2.0) * wheelNonLinearity);
		wheel = sin((M_PI / 2.0) * wheelNonLinearity * wheel) / sin((M_PI / 2.0) * wheelNonLinearity);
	} else {
		wheelNonLinearity = 0.8 ; // ("TURN_NONLIN_LOW", 0.8);
		// Apply a sin function that's scaled to make it feel better.
		wheel = sin((M_PI / 2.0) * wheelNonLinearity * wheel) / sin((M_PI / 2.0) * wheelNonLinearity);
		wheel = sin((M_PI / 2.0) * wheelNonLinearity * wheel) / sin((M_PI / 2.0) * wheelNonLinearity);
		wheel = sin((M_PI / 2.0) * wheelNonLinearity * wheel) / sin((M_PI / 2.0) * wheelNonLinearity);
	}
	***/
	

	double left_pwm, right_pwm, overPower;
	float sensitivity = 1.7;

	float angular_power;
	float linear_power;
	
	static int i = 0;
	i++;


	double neg_inertia_scalar;
	if (isHighGear) {
		neg_inertia_scalar = 10.0 ; 	// ("NEG_INERTIA_HIGH", 0.0);
		sensitivity = 1.2 ; 		// ("SENSE_HIGH", 1.7);
	} else {
		if (wheel * neg_inertia > 0) {
			neg_inertia_scalar = 5.0 ; // ("NEG_INERTIA_LOW_MORE", 0.0);
		} else {
			if (fabs(wheel) > 0.65) {
				neg_inertia_scalar = 80.0 ; // ("NEG_INERTIA_LOW_LESS_EXT", 0.0);
			} else {
				neg_inertia_scalar = 3.0 ; // ("NEG_INERTIA_LOW_LESS", 0.0);
			}
		}
		sensitivity = 1.1 ; // ("SENSE_LOW", 1.2);
		
		if (fabs(throttle) > 0.1 ) {		// ("SENSE_CUTTOFF", 0.1
			sensitivity = 1 - (1 - sensitivity) / fabs(throttle);
		}
	}
	wheel = wheel + neg_inertia * neg_inertia_scalar;

	linear_power = throttle;

	if (isQuickTurn) {
		overPower = 1.0;
		if (isHighGear) {
			sensitivity = 1.0;
		} else {
			sensitivity = 1.0;
		}
		angular_power = wheel;
	} else {
		overPower = 0.0;
		angular_power = fabs(throttle) * wheel * sensitivity;
	}

	right_pwm = left_pwm = linear_power;
	left_pwm += angular_power;
	right_pwm -= angular_power;

	if (left_pwm > 1.0) {
		right_pwm -= overPower*(left_pwm - 1.0);
		left_pwm = 1.0;
	} else if (right_pwm > 1.0) {
		left_pwm -= overPower*(right_pwm - 1.0);
		right_pwm = 1.0;
	} else if (left_pwm < -1.0) {
		right_pwm += overPower*(-1.0 - left_pwm);
		left_pwm = -1.0;
	} else if (right_pwm < -1.0) {
		left_pwm += overPower*(-1.0 - right_pwm);
		right_pwm = -1.0;
	}

	// if (i % 50 == 0) {
	// 	printf("l: %f r: %f t: %f w: %f ax: %f\n", left_pwm, right_pwm, throttle, wheel, axis);
	// }
	driveVec[0] = left_pwm ;
	driveVec[1] = right_pwm ;

    return driveVec;
}


#else 	// Dmitri update

NumericalVector<2> AcceleratorArcade::update(float throttle, float axis, RobotModel::GearSet gear, bool bQuickTurn)
{
	// if (throttle < 0)
	//	axis = -axis;
	
	const float sign_x = (axis < 0.0) ? -1.0 : 1.0;
			
	//m_turnVelocity += m_accelerator_multiplier * axis * fabs(throttle) ; //Continue to increase the accleration, from where we left off
	
	if (gear == RobotModel::kHighGear)
	{
		const double exponent_value = exp(1); // e = 2.7????
		const double func_val = (pow(exponent_value, fabs(axis)) - 1.0) / (exponent_value - 1.0);
		m_turnVelocity = sign_x*func_val*(1.35 * fabs(throttle));
	} else {
		const double exponent_value = exp(1); // e = 2.7????
		const double func_val = log((exponent_value * fabs(axis)) + 1) / log(exponent_value);
		m_turnVelocity = sign_x*func_val*(fabs(throttle));	
	}
	 
	if(fabs(m_turnVelocity) < .1) //If user does not want to steer, stop and cancel acceleration
		m_turnVelocity = 0;

	// limit speed to point turn at current throttle
	if(fabs(m_turnVelocity) > (fabs(throttle) * fabs(axis) * 2)){
	    //Limit our acceleration force proportional to the throttle
		m_turnVelocity = sign_x * fabs(throttle) * fabs(axis) * 2;
    }

	NumericalVector<2> driveVec;
	const double sign_throttle = (throttle < 0.0) ? -1.0 : 1.0;
	if((fabs(m_turnVelocity) + fabs(throttle)) > 1)
	{
	    if (axis > 0)
	    {
            driveVec[0] = throttle;
            driveVec[1] =  sign_throttle * (fabs(throttle) - m_turnVelocity);
	    } else {
            driveVec[0] = sign_throttle * (fabs(throttle) + m_turnVelocity);;
            driveVec[1] = throttle;
	    }
	} else {
	    driveVec[0] = throttle + m_turnVelocity;
        driveVec[1] = throttle - m_turnVelocity;
	}
	
	
	if( bQuickTurn ){ //if quickturn
		
		const float sign_x = (axis<0.0) ? -1.0 : 1.0;
		
		m_turnVelocity = sign_x * sqrt(fabs(axis)) ;
		// m_turnVelocity = sign_x*fabs(axis)*1.7;	// originally 1.7
		driveVec[0] = m_turnVelocity;
		driveVec[1] = -m_turnVelocity;
	}
	
	static int i = 0;
	i++;
	
	// if (i % 50 == 0) {
	// 	printf("l: %f r: %f t: %f ax: %f\n", driveVec[0], driveVec[0], throttle, axis);
	// }


    return driveVec;
}

#endif
