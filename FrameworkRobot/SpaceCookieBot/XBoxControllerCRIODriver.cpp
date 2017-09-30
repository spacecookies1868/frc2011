#include "XBoxControllerCRIODriver.h"
#include "Shared/Vector3.h"
#include <math.h>

XBoxControllerCRIODriver::XBoxControllerCRIODriver(int portNum)
: m_j1(portNum)
{
	
}

XBoxControllerCRIODriver::~XBoxControllerCRIODriver()
{
}

float XBoxControllerCRIODriver::getAxis (Axis a) {
	switch(a){
	case(LeftStickY):
		return m_j1.GetRawAxis(2);
	case(LeftStickX):
		return m_j1.GetRawAxis(1);
	case(RightStickX):
		return m_j1.GetRawAxis(4);
	case(RightStickY):
		return m_j1.GetRawAxis(5);
	default:
		return 0;
	}
}


bool XBoxControllerCRIODriver::getButton (Button b) {
	switch(b){
	case(A):
		return m_j1.GetRawButton(1);
	case(B):
		return m_j1.GetRawButton(2);
	case(X):
		return m_j1.GetRawButton(3);
	case(Y):
		return m_j1.GetRawButton(4);
	case(LB):
		return m_j1.GetRawButton(5);
	case(RB):
		return m_j1.GetRawButton(6);
	case(BACK):
		return m_j1.GetRawButton(7);
	case(START):
		return m_j1.GetRawButton(8);
	default:
		return false;
	}
}


