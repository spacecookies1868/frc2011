#include "JoystickControllerCRIODriver.h"
#include "Shared/Vector3.h"
#include <math.h>

JoystickControllerCRIODriver::JoystickControllerCRIODriver(int port1Num, int port2Num, int port3Num, int port4Num)
: m_j1(port1Num)
, m_j2(port2Num)
, m_j3(port3Num)
, m_j4(port4Num)
{
	
}

JoystickControllerCRIODriver::~JoystickControllerCRIODriver()
{
}

float JoystickControllerCRIODriver::getAxis (Axis a) {
	switch(a){
	case(LStickY):
		return m_j1.GetRawAxis(2);
	case(LStickX):
		return m_j1.GetRawAxis(1);
	case(LStickZ):
		return m_j1.GetRawAxis(3);
	case(RStickY):
		return m_j2.GetRawAxis(2);
	case(RStickX):
		return m_j2.GetRawAxis(1);
	case(RStickZ):
		return m_j2.GetRawAxis(3);
	case(AStickX):
		return m_j3.GetRawAxis(1);
	case(AStickY):
		return m_j3.GetRawAxis(2);
	case(AStickZ):
		return m_j3.GetRawAxis(3);
	default:
		return 0;
	}
}


bool JoystickControllerCRIODriver::getButton (Button b) {
	switch(b){
	case(LTrig):
		return m_j1.GetRawButton(1);
	case(LTwo):
		return m_j1.GetRawButton(2);
	case(LThree):
		return m_j1.GetRawButton(3);
	case(LFour):
		return m_j1.GetRawButton(4);
	case(LFive):
		return m_j1.GetRawButton(5);
	case(LSix):
		return m_j1.GetRawButton(6);
	case(LSeven):
		return m_j1.GetRawButton(7);
	case(LEight):
		return m_j1.GetRawButton(8);
	case(LNine):
		return m_j1.GetRawButton(9);
	case(LTen):
		return m_j1.GetRawButton(10);
	case(LEleven):
		return m_j1.GetRawButton(11);
	case(LTwelve):
		return m_j1.GetRawButton(12);
	case(RTrig):
		return m_j2.GetRawButton(1);
	case(RTwo):
		return m_j2.GetRawButton(2);
	case(RThree):
		return m_j2.GetRawButton(3);
	case(RFour):
		return m_j2.GetRawButton(4);
	case(RFive):
		return m_j2.GetRawButton(5);
	case(RSix):
		return m_j2.GetRawButton(6);
	case(RSeven):
		return m_j2.GetRawButton(7);
	case(REight):
		return m_j2.GetRawButton(8);
	case(RNine):
		return m_j2.GetRawButton(9);
	case(RTen):
		return m_j2.GetRawButton(10);
	case(REleven):
		return m_j2.GetRawButton(11);
	case(RTwelve):
		return m_j2.GetRawButton(12);
	case(ATrig):
		return m_j3.GetRawButton(1);
	case(ATwo):
		return m_j3.GetRawButton(2);
	case(AThree):
		return m_j3.GetRawButton(3);
	case(AFour):
		return m_j3.GetRawButton(4);
	case(AFive):
		return m_j3.GetRawButton(5);
	case(ASix):
		return m_j3.GetRawButton(6);
	case(ASeven):
		return m_j3.GetRawButton(7);
	case(AEight):
		return m_j3.GetRawButton(8);
	case(ANine):
		return m_j3.GetRawButton(9);
	case(ATen):
		return m_j3.GetRawButton(10);
	case(AEleven):
		return m_j3.GetRawButton(11);
	case(ATwelve):
		return m_j3.GetRawButton(12);
	case(POne):
		return m_j4.GetRawButton(1);
	case(PTwo):
		return m_j4.GetRawButton(2);
	case(PThree):
		return m_j4.GetRawButton(3);
	case(PFour):
		return m_j4.GetRawButton(4);
	case(PFive):
		return m_j4.GetRawButton(5);
	case(PSix):
		return m_j4.GetRawButton(6);
	case(PSeven):
		return m_j4.GetRawButton(7);
	case(PEight):
		return m_j4.GetRawButton(8);
	case(PNine):
		return m_j4.GetRawButton(9);
	case(PTen):
		return m_j4.GetRawButton(10);
	case(PEleven):
		return m_j4.GetRawButton(11);
	case(PTwelve):
		return m_j4.GetRawButton(12);
	default:
		return false;
	}
}


