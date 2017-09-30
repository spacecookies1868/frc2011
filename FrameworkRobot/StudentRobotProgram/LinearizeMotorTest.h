/*
 *  LinearizeMotorTest.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 2/11/11.
 *
 */

#ifndef LINEARIZE_MOTOR_TEST_H
#define LINEARIZE_MOTOR_TEST_H

#include "Shared/RobotModel.h"

class LinearizeMotorTest : public virtual FiniteStateMachine
{
private:
    
    enum State
    {
        kInitState,
        kPreparingForSpeed,
        kSpinningAtSpeed,
        kFinishing,
        kDone
    };
	
    FILE*               m_pFile;
    double              m_driveSpeed;
    double              m_timestamp;
    RobotModel::WheelValues     m_prevDriveEncoderVal;
	double						m_prevElevatorEncoderVal;
    
public:

	
    explicit LinearizeMotorTest(RobotModel* pRobot) : FiniteStateMachine(pRobot, kInitState)
    {
	}
    
    virtual ~LinearizeMotorTest() {}
    
    virtual bool IsMachineDone() {return m_stateVal == kDone;}
    
    virtual void UpdateState(double currTime_Sec, double deltaTime_sec)
    {
        uint32_t nextState = m_stateVal;
        switch (m_stateVal) {
            case kInitState:
                m_pFile = fopen("linearizeInfo.csv","w");
				m_driveSpeed = -1.0;
				m_timestamp = currTime_Sec;
                nextState = kPreparingForSpeed;
                break;
            case kPreparingForSpeed:
                m_pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, m_driveSpeed);
                m_pRobot->SetWheelMotorValue(RobotModel::kRightWheel, m_driveSpeed);					
            	
                if ((currTime_Sec - m_timestamp) >= 1.0)
                {
                    nextState = kSpinningAtSpeed;
                    m_timestamp = currTime_Sec;
                    m_pRobot->EnableWheelEncoders();
                    m_prevDriveEncoderVal = m_pRobot->GetWheelEncoderValues();
                } 
				else
				{
					nextState = kPreparingForSpeed;					
				}
                break;
            case kSpinningAtSpeed:
                if ((currTime_Sec - m_timestamp) >= 1.0)
                {
                    if (m_driveSpeed >= 1.0)
                        nextState = kFinishing;
                    else
                        nextState = kPreparingForSpeed;
					
                    RobotModel::WheelValues postEncoderValue;
                    RobotModel::WheelValues encoderDiff;
                    RobotModel::WheelValues encoderSpeed;
                    
                    postEncoderValue = m_pRobot->GetWheelEncoderValues();
                    m_pRobot->ResetWheelEncoders();
                    
                    encoderDiff = postEncoderValue - m_prevDriveEncoderVal;
                    encoderSpeed = encoderDiff / (currTime_Sec - m_timestamp);
                    
                    fprintf(m_pFile, "%f, %f, %f\n", m_driveSpeed, encoderSpeed[0], encoderSpeed[1]);						
                
                    m_timestamp = currTime_Sec;
                    m_driveSpeed += 0.1;
                    
                    
                } else
                {    nextState = kSpinningAtSpeed;
                }
                break;
            case kFinishing:
                m_pRobot->SetWheelMotorValue(RobotModel::kLeftWheel, 0);
                m_pRobot->SetWheelMotorValue(RobotModel::kRightWheel, 0);
            
                fclose(m_pFile);
                printf("Closing File\n");
                nextState = kDone;
                break;
            case kDone:
                nextState = kDone;
                break;
            default:
                break;
        }
        
        m_stateVal = nextState;
    }
};

#endif

