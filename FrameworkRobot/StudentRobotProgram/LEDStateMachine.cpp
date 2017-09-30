#include <StudentRobotProgram/LEDStateMachine.h>


void LEDStateMachine::UpdateState(double c, double d)
{
	
	nextState = m_stateVal;
	
	switch (m_stateVal){
	case(kReset):
		nextState = kInitialize;
		break;
	case (kInitialize):
		m_bRequestRed = false;
		m_bRequestWhite = false;
		m_bRequestBlue = false;
		nextState = kIdle;
		break;
	case (kIdle):
		if (m_bRequestRed == true){
			nextState = kRed;
		}
		else if (m_bRequestWhite == true){
			nextState = kWhite;
		}
		else if (m_bRequestBlue == true){
			nextState = kBlue;
		}
		break;
	case (kRed):
		//turn on red LED
		//turn off white LED
		//turn off blue LED
		nextState = kIdle;
		break;
	case (kWhite):
		//turn on white LED
		//turn off red LED
		//turn off blue LED
		nextState = kIdle;
		break;
	case (kBlue):
		//turn on blue LED
		//turn off red LED
		//turn off white LED
		nextState = kIdle;
		break;
	}
	m_stateVal = nextState;
}
/*LEDStateMachine::~LEDStateMachine()
{
}*/
void LEDStateMachine::RequestRed()
{
	m_bRequestRed = true;
	m_bRequestWhite = false;
	m_bRequestBlue = false;
}
void LEDStateMachine::RequestWhite()
{
	m_bRequestRed = false;
	m_bRequestWhite = true;
	m_bRequestBlue = false;
}
void LEDStateMachine::RequestBlue()
{
	m_bRequestRed = false;
	m_bRequestWhite = false;
	m_bRequestBlue = true;
}
