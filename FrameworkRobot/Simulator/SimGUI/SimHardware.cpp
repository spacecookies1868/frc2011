/*
 *  SimHardware.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/30/10.
 *
 */

#include <math.h>
#include "SimHardware.h"
#include "SimulatorWindow.h"
#include "SimulatorController.h"
#include "SimulatedRobot/SimulatedRobotModel.h"
#include "SimulatedRobot/SimulatedField.h"
#include "SimulatedRobot/SimModules/SimModuleBase.h"

#if  defined(__APPLE__)
#elif defined(WINDOWS) 
#endif

SimHardware::SimHardware()
{
    m_pThread=NULL;
    m_lock = SDL_CreateMutex();
}

SimHardware::~SimHardware()
{
}

SimHardware* SimHardware::s_pSimHardware = NULL;

SimHardware* SimHardware::GetInstance()
{
    if (s_pSimHardware == NULL)
        s_pSimHardware = new SimHardware();
    return s_pSimHardware;
}

void SimHardware::WaitTimeMilliSec(uint32_t waitMS)
{
    waitMS = ((waitMS+9) / 10) * 10;
    uint32_t endTime = GetTimeMilliSec() + waitMS;
    while (endTime > GetTimeMilliSec()) {
        SDL_Delay(1);
    }
}

int SimHardware::threadFunc(void* inObj)
{
    SimHardware* simHW = reinterpret_cast<SimHardware*> (inObj);
    simHW->threadLoop();
    return 0;
}

void SimHardware::Initialize(SimulatedRobotModel* pRobot, SimulatedField* pField, SimulatorController* pController)
{
    m_pRobotModel = pRobot;
    m_pField = pField;
    m_pController = pController;
	m_pThread = SDL_CreateThread(SimHardware::threadFunc, this);
}

void SimHardware::Destroy()
{
    SDL_WaitThread(m_pThread, NULL);
    SDL_DestroyMutex(m_lock);
}

void SimHardware::threadLoop()
{
    
    SimModuleBase::BuildModuleNetwork();
    
    while (m_pController->IsProgramActive()) {
        
        if (m_pController->IsProgramPaused() || !m_pController->IsProgramRunning())
            continue;
           
        SDL_mutexP(m_lock);
		m_simulationTime++;
        m_pRobotModel->UpdateSimulation(m_simulationTime);
		//if(!(m_simulationTime%SAMPLING_RATE)) m_pField->CheckCollisions();
		//m_pField->CheckCollisions();
        SDL_mutexV(m_lock);
						
        RobotPositionRecord rec;
        rec.simTime = m_simulationTime;
        rec.pos = m_pRobotModel->GetPosition();
        rec.angle = m_pRobotModel->GetRotationAngle().z;
        SimulatorWindow::UpdateRobot(rec);
        /*
        if (rec.pos.x <= 0.0 || rec.pos.y <= 0.0 || rec.pos.x >= m_pField->GetFieldLength() || rec.pos.y >= m_pField->GetFieldWidth())
            m_pController->StopSimulation();*/
        SDL_Delay(1);
        
    }
    
    SDL_Delay(1);
}

