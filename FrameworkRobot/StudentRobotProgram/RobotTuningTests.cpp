/*
 *  RobotTuningTests.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/29/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#include "RobotTuningTests.h"
#include "Shared/RobotModel.h"
#include "Shared/NumericalVector.hpp"
#include "Shared/PolyFunction.hpp"
#include "PID.hpp"

static double WheelRotationDistance(double encoderValue, const RobotModel* pRobot)
{
	static const float kDistancePerRotation = 2.f * M_PI * pRobot->GetWheelRadius(RobotModel::kLeftWheel);
	return encoderValue * kDistancePerRotation;
}

void TurnInPlace(double degreesToTurn, RobotModel* pRobot)
{
	//if it rotates backwards flip the wheel values
	double currentDegrees = pRobot->GetGyroValue();
	if(degreesToTurn > currentDegrees)
		while(degreesToTurn > currentDegrees)
		{
			pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, -1);
			pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 1);
			currentDegrees = pRobot->GetGyroValue();
		}
	else
		while(degreesToTurn < currentDegrees)
		{
			pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 1);
			pRobot->SetWheelMotorValue(RobotModel::kRightWheel, -1);
			currentDegrees = pRobot->GetGyroValue();
		}
}

static double accelerationCurve(double time, double limitSpeed);

static inline double ConvertRPSToMotorPWM(double rps_value, RobotModel* pRobot){
	return rps_value / (pRobot->GetMaxMotorRPS()/pRobot->GetWheelGearingRatio(RobotModel::kRightWheel)*pRobot->GetEfficiencyLoss());
}

void TestPID(RobotModel* pRobot, double speed, double driveTime){
	
	pRobot->EnableWheelEncoders();
	pRobot->ResetWheelEncoders();
    
	const double startTime = pRobot->GetTimeSec();
    
	double lEncPrev = pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel);
	double rEncPrev = pRobot->GetWheelEncoderValue(RobotModel::kRightWheel);
	double prevTime = startTime;
    
	PID lPID(0.5, 0, 0);
	PID rPID(0.5, 0, 0);
    
	//PID tPID(1.1/2, .06, 0);
	PID tPID(.0155/2,.012,.0001);
    
	while(pRobot->GetTimeSec()-startTime < driveTime){
		//Calculating Actual RPS
		
        pRobot->WaitTimeMilliSec(50);
        
        const double curr_time = pRobot->GetTimeSec();
        
		const double lEncCurr = pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel);
		const double rEncCurr = /**0.11;*/pRobot->GetWheelEncoderValue(RobotModel::kRightWheel);
        
		const double lDeltaEnc = lEncCurr - lEncPrev;
		const double rDeltaEnc = rEncCurr - rEncPrev;
		const double deltaTime = curr_time - prevTime;
        
		const double lActualRPS = lDeltaEnc/deltaTime;
		const double rActualRPS = rDeltaEnc/deltaTime;
        
		const double expectedRPS = speed * (pRobot->GetMaxMotorRPS()/pRobot->GetWheelGearingRatio(RobotModel::kRightWheel)*pRobot->GetEfficiencyLoss());
        
		const double expectedRPS_accelerated = accelerationCurve(curr_time - startTime, expectedRPS);
        
		//Calculating Right and Left Errors
		double lErrorRPS = expectedRPS_accelerated - lActualRPS;
		double rErrorRPS = expectedRPS_accelerated - rActualRPS;
        
		if (fabs(lErrorRPS) < .05)
			lErrorRPS=0;
		if (fabs(rErrorRPS) < .05)
			rErrorRPS=0;
        
		const double tError = (expectedRPS_accelerated - expectedRPS_accelerated) - (lActualRPS - rActualRPS);
        
		const double lError_out = lPID.Compute(lErrorRPS, deltaTime);
		const double rError_out = rPID.Compute(rErrorRPS, deltaTime);
        
		const double tError_out = tPID.Compute(tError, deltaTime);
        
		const double lError_compensate = ConvertRPSToMotorPWM(lError_out, pRobot);
		const double rError_compensate = ConvertRPSToMotorPWM(rError_out, pRobot);
		const double tError_compensate = ConvertRPSToMotorPWM(tError_out, pRobot);
        
		const double l_pwm = speed + lError_compensate;
		const double r_pwm = speed + rError_compensate;
		double l_drive;
		double r_drive;
        
		if(l_drive>=1.0 || r_drive >= 1.0)
		{
			if (1)
			{
				if (tError_compensate >= 0)
				{
					l_drive = speed - tError_compensate;
					r_drive = speed;
				} else {
					r_drive = speed + tError_compensate;
					l_drive = speed;
				}
                
			} else {
				if(lError_compensate>=rError_compensate)
				{	l_drive = speed;
					r_drive = r_pwm - lError_compensate;
				} else {
					r_drive = speed;
					l_drive = l_pwm - rError_compensate;
				}
			}
		} else {
			if (1)
			{
				l_drive = speed - (tError_compensate/2);
				r_drive = speed + (tError_compensate/2);
			}
			else
			{
				l_drive = l_pwm;
				r_drive = r_pwm;
			}
            
		}
        
        
		printf("Time %f lErr %f rErr %f tErr %f LPWM %f RPWM %f Gyro %f\n",
               curr_time-startTime, lErrorRPS, rErrorRPS, tError, l_drive, r_drive, pRobot->GetGyroValue());
        
		pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, l_drive);
		pRobot->SetWheelMotorValue(RobotModel::kRightWheel, r_drive);
        
        lEncPrev = lEncCurr;
		rEncPrev = rEncCurr;
		prevTime = curr_time;
		
	}
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 0);
	double startingTime = pRobot->GetTimeSec();
	while(pRobot->GetTimeSec()-startingTime < 3){
		printf("Gyro Value: %f\n", pRobot->GetGyroValue());
	}
	pRobot->WaitTimeMilliSec(3000);
	pRobot->DisableWheelEncoders();

}

static double accelerationCurve(double time, double limitSpeed)
{
	double y=0;
	if(time <= .04)
	{
		return 0;
	}
	else if(time >= 1.0)
	{
		return limitSpeed;
	}
    
	const double polyConstants[6] =
    {	34.2977, 	-103.6708, 	125.9292, 	-80.7524,
        29.6553,	-0.4154};
    
	PolyFunction<5> polyFunc(polyConstants);
	y = polyFunc(time);
    
	if(fabs(y)>=fabs(limitSpeed))
	{
		return limitSpeed;
	}
    
	return y;
    
}

void FindAcceleration(double dir, RobotModel* pRobot)
{
	FILE * pFile;
    
	char filename[32];
	sprintf(filename, "accelerationInfo_%f.csv", dir);
    
	pFile = fopen(filename,"w");
	pRobot->EnableWheelEncoders();
	double previousTime = pRobot->GetTimeSec();
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, dir);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, dir);
    
	double experimentStart = pRobot->GetTimeSec();
    
	while(pRobot->GetTimeSec() - experimentStart < 3)
	{
		double encoderValue[2];
		encoderValue[0] = pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel);
		encoderValue[1] = pRobot->GetWheelEncoderValue(RobotModel::kRightWheel);
        
		double time = pRobot->GetTimeSec();
		double delta_time = time - previousTime;
		previousTime = time;
		fprintf(pFile, "1.0, %f, %f, %f\n", time,
				encoderValue[0]/delta_time, encoderValue[1]/delta_time);
		pRobot->ResetWheelEncoders();
		pRobot->WaitTimeMilliSec(20);
        
	}
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 0);
    
	double experimentEnd = pRobot->GetTimeSec();
	while((pRobot->GetTimeSec() - experimentEnd) <3)
	{
		double encoderValue[2];
		encoderValue[0] = pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel);
		encoderValue[1] = pRobot->GetWheelEncoderValue(RobotModel::kRightWheel);
        
		double time = pRobot->GetTimeSec();
		double delta_time = time - previousTime;
		previousTime = time;
		fprintf(pFile, "0.0, %f, %f, %f\n", time,
				encoderValue[0]/delta_time, encoderValue[1]/delta_time);
		pRobot->ResetWheelEncoders();
		pRobot->WaitTimeMilliSec(20);
        
	}
    
	pRobot->DisableWheelEncoders();
	fclose(pFile);
}

void FindEncoderConstant(RobotModel* pRobot){
	pRobot->EnableWheelEncoders();
	{
		for(int j = 0; j < 60; j++){
			printf("Left: %f Dist: %f\t Right: %f Dist: %f\n",
                   pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel),
                   WheelRotationDistance(pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel),pRobot),
                   pRobot->GetWheelEncoderValue(RobotModel::kRightWheel),
                   WheelRotationDistance(pRobot->GetWheelEncoderValue(RobotModel::kRightWheel),pRobot)
                   );
			pRobot->WaitTimeMilliSec(1000);
		}
	}
	pRobot->DisableWheelEncoders();
	
}


double FindStoppingDistance(double speed, RobotModel* pRobot){
	double realSpeed=speed*1.68;
    
	static const double rps_deccel_value = -2.75;
	const double deccel = rps_deccel_value * 2 * M_PI * pRobot->GetWheelRadius(RobotModel::kLeftWheel);
    
	double stoppingDistance = -(realSpeed*realSpeed) /(2*deccel);
	return stoppingDistance;
}

//Incorporate Right Encoder
double FindVelocity(RobotModel* pRobot){
	double initialDist = WheelRotationDistance(pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel), pRobot);
	pRobot->WaitTimeMilliSec(50);
	//double r_velocity = WheelRotationDistance(pRobot->getEncoderValue(RobotModel::kRightWheel), pRobot) /.05;
	double l_velocity = (WheelRotationDistance(pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel), pRobot) - initialDist) /.05;
	return l_velocity;
}

void TempDriveStraight(double distance, double speed, RobotModel* pRobot)
{
	double distanceTraveled = 0;
	PID pid(.3,0,0);
	while(distanceTraveled < distance){
        
		double error = pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel)-pRobot->GetWheelEncoderValue(RobotModel::kRightWheel);
		if(error < -(.5)){
			//double errorRotation = fabs(realError)*2*M_PI*pRobot->GetWheelRadius(RobotModel::kRightWheel);
			pRobot->SetWheelMotorValue(RobotModel::kRightWheel, speed);
			pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, speed+.1);
		}
		else if (error > .5){
			pRobot->SetWheelMotorValue(RobotModel::kRightWheel, speed+.1);
			pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, speed);
		}
		else{
			pRobot->SetWheelMotorValue(RobotModel::kRightWheel, speed);
			pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, speed);
		}
		distanceTraveled=WheelRotationDistance(pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel), pRobot);
		pRobot->WaitTimeMilliSec(10);
        
	}
}

void FindRotationTensor(double l_speed, double r_speed, RobotModel* pRobot){
	//handles output
	FILE* file;
	char filename[64];
	sprintf(filename, "RotationTensorTest_%.2f_%.2f.csv", l_speed, r_speed);
	file=fopen(filename, "w");
	
	pRobot->EnableWheelEncoders();
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, l_speed);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, r_speed);
	pRobot->WaitTimeMilliSec(1500);
	pRobot->ResetWheelEncoders();
	const double test_init_time = pRobot->GetTimeSec();
	const double gyro_init = pRobot->GetGyroValue();
	
	fprintf(file,"%f, %f, %f, %f, %f, %f \n",
					pRobot->GetTimeSec()-test_init_time, l_speed, r_speed,
					pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel)/pRobot->GetTimeSec(),
					pRobot->GetWheelEncoderValue(RobotModel::kRightWheel)/pRobot->GetTimeSec(),
					pRobot->GetGyroValue()-gyro_init);
	
	while(pRobot->GetTimeSec()-test_init_time < 2.0){
		pRobot->WaitTimeMilliSec(50);
		fprintf(file,"%f, %f, %f, %f, %f, %f \n",
				pRobot->GetTimeSec()-test_init_time, l_speed, r_speed,
				pRobot->GetWheelEncoderValue(RobotModel::kLeftWheel)/pRobot->GetTimeSec(),
				pRobot->GetWheelEncoderValue(RobotModel::kRightWheel)/pRobot->GetTimeSec(),
				pRobot->GetGyroValue()-gyro_init);
	}
	
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 0);
	fclose(file);
}

void MotorPortTest(RobotModel* pRobot){
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 1.0);
	pRobot->WaitTimeMilliSec(1000);
	pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
	
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 1.0);
	pRobot->WaitTimeMilliSec(1000);
	pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 0);
	/*
	pRobot->SetWheelMotorValue(RobotModel::kElevator, 1.0);
	pRobot->WaitTimeMilliSec(1000);
	pRobot->SetWheelMotorValue(RobotModel::kElevator, 0);
	
	pRobot->SetWheelMotorValue(RobotModel::kTopRoller, 1.0);
	pRobot->WaitTimeMilliSec(1000);
	pRobot->SetWheelMotorValue(RobotModel::kTopRoller, 0);
	
	pRobot->SetWheelMotorValue(RobotModel::kBottomRoller, 1.0);
	pRobot->WaitTimeMilliSec(1000);
	pRobot->SetWheelMotorValue(RobotModel::kBottomRoller, 0);*/
}
