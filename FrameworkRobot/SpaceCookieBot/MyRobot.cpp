#include "WPILib.h"
#include "Shared/RobotModel.h"
#include "StudentRobotProgram/StudentProgram.h"
#include "SpaceCookieBot.h"
#include "XBoxControllerCRIODriver.h"
#include "JoystickControllerCRIODriver.h"
#include "SmartDashboard.h"
#include "SmartDashboardPacketFactory.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public IterativeRobot
{
	JoystickControllerCRIODriver 	m_driveController;
	SpaceCookieBot 				m_robotInstance;
	StudentProgram      		m_programInstance;
	SmartDashboard      		mySmartDashboard;

	uint32_t 			m_autoPeriodicLoops;
	uint32_t 			m_disabledPeriodicLoops;
	uint32_t 			m_telePeriodicLoops;
	
	double 				m_prevUpdateTimeSec;

public:
	RobotDemo(void)
	: m_driveController(1,2,3,4)		// as they are declared above.
	, m_robotInstance()
	, m_programInstance(&m_robotInstance,&m_driveController)
	, mySmartDashboard(0)
	, m_autoPeriodicLoops(0)
	, m_disabledPeriodicLoops(0)
	, m_telePeriodicLoops(0)
	, m_prevUpdateTimeSec(0)
	{
		printf("made it to robotdemo constructor...\n");
		GetWatchdog().SetEnabled(false);
	}

	virtual ~RobotDemo() 
	{
	}


	/** Init Functions */

	void RobotInit(void) 
	{
		SetPeriod(0.01);
		m_robotInstance.ClearGyro();
		printf("RobotInit() completed with Period %f Loops Per Second %f.\n", 
				GetPeriod(), GetLoopsPerSec());
		mySmartDashboard.init();
		m_robotInstance.EnableCompressor();
		//m_robotInstance.EnableCamera();
		m_robotInstance.SetGearType(RobotModel::kLowGear);
		//m_robotInstance.SetDeployType(RobotModel::kDeploy);
		m_robotInstance.WaitTimeMilliSec(1000);		
	}

	void DisabledInit(void) 
	{
		m_disabledPeriodicLoops = 0;
		mySmartDashboard.Log("Disabled", "System State");
	}

	void AutonomousInit(void) 
	{
		m_autoPeriodicLoops = 0;
		GetWatchdog().SetEnabled(true);
		m_prevUpdateTimeSec = m_robotInstance.GetTimeSec();
		mySmartDashboard.Log("Autonomous", "System State");
		m_robotInstance.ClearGyro();
		m_programInstance.autonomousInit();
		
		m_robotInstance.SetGearType(RobotModel::kHighGear);
		//m_robotInstance.SetDeployType(RobotModel::kDeploy);
	}

	void TeleopInit(void) 
	{
		GetWatchdog().SetEnabled(true);
		m_telePeriodicLoops = 0;
		mySmartDashboard.Log("Teleop", "System State");
		m_robotInstance.ClearGyro();
		m_programInstance.teleopInit();
	}

	/** Periodic Functions */

	void DisabledPeriodic(void)  
	{
		m_disabledPeriodicLoops++;
	}

	void AutonomousPeriodic(void) 
	{
		//GetWatchdog().SetEnabled(true);
		GetWatchdog().Feed();
		m_autoPeriodicLoops++;
		//printf("AutonomousPeriodic\n");
		double curr_time = m_robotInstance.GetTimeSec();
		double delta_time = curr_time - m_prevUpdateTimeSec;
		m_programInstance.autonomousControlPeriodic(curr_time, delta_time);
		m_prevUpdateTimeSec = curr_time;
		//if (m_autoPeriodicLoops == 1)
		//m_programInstance.autonomousControl();
	}


	void TeleopPeriodic(void) {
		m_telePeriodicLoops++;
		//printf("Operator Control Established!\n");
		GetWatchdog().SetEnabled(true);
		GetWatchdog().Feed();
		double curr_time = m_robotInstance.GetTimeSec();
		double delta_time = curr_time - m_prevUpdateTimeSec;
		m_programInstance.operatorControl(curr_time, delta_time);
		m_prevUpdateTimeSec = curr_time;
	}
	
	
	void DisabledContinuous()
	{	
	}
	
	virtual void AutonomousContinuous()
	{
	}
	
	virtual void TeleopContinuous()
	{
	}

};

START_ROBOT_CLASS(RobotDemo);

