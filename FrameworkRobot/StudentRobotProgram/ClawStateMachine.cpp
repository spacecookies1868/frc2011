#include "ClawStateMachine.h"



void ClawStateMachine::UpdateState(double currTime_Sec, double deltaTime_sec){
	
	nextState =  m_stateVal;
	
	switch (m_stateVal){
	case (kIdle):
		if (m_bRequestClawOpen){
			nextState = kOpen;
		}else if (m_bRequestClawClosed){
			nextState = kClosed;
		}else {
			nextState = kIdle;
		}
		m_bRequestClawClosed = false;
		m_bRequestClawOpen = false;
		break;
	case (kReset):
		//close claw
		nextState = kInitialize;
		break;
	case (kInitialize):
		m_bRequestClawOpen = false;
		m_bRequestClawClosed = false;
		break;
	case (kOpen):
		//open claw
		if(m_bRequestClawClosed){
			nextState = kClosed;
		}else {
			nextState = kOpen;
		}
		m_bRequestClawOpen = false;
		break;
	case (kClosed):
		//close claw
		if(m_bRequestClawOpen){
			nextState = kOpen;
		}else {
			nextState = kClosed;
		}
		m_bRequestClawClosed = false;
		break;
	}
	
	m_stateVal = nextState;
	
}


void ClawStateMachine::RequestClawOpen(){
	
	m_bRequestClawOpen = true;
	
	
}

void ClawStateMachine::RequestClawClosed(){
	
	m_bRequestClawClosed = true;
	
	
}
