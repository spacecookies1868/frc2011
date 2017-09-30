/*
 *  BaseProgram.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/31/10.
 *
 */

#include "SimulatedProgram.h"
#include "SimGUI/SimulatorController.h"
#include "Shared/RobotProgram.h"
#include "SimGUI/SimHardware.h"

SimulatedProgram::SimulatedProgram(RobotProgram* pRobotProg, SimulatorController* pController)
: m_pThread(NULL)
, m_pRobotProg(pRobotProg)
, m_pController(pController)
{
}


SimulatedProgram::~SimulatedProgram()
{
    if (m_pThread)
    {
        SDL_WaitThread(m_pThread, NULL);
        m_pThread = NULL;
    }
}

int SimulatedProgram::prog_thread(void* obj)
{
    SimulatedProgram* progObj = reinterpret_cast<SimulatedProgram*> (obj);
    RobotProgram* pRobotProgram = progObj->m_pRobotProg;
    
    if (progObj->m_bAutonomous)
    {
        // Call into old control function just once
        pRobotProgram->autonomousControl();
    
        while (!pRobotProgram->IsAutonomousDone()) {
            double prevTime = SimHardware::GetInstance()->GetTimeSec();
            SimHardware::GetInstance()->WaitTimeMilliSec(10);
            double currTime = SimHardware::GetInstance()->GetTimeSec();
            pRobotProgram->autonomousControlPeriodic(currTime, prevTime);
        }
    } else {
        while (1) 
        {
            double prevTime = SimHardware::GetInstance()->GetTimeSec();
            SimHardware::GetInstance()->WaitTimeMilliSec(10);
            double currTime = SimHardware::GetInstance()->GetTimeSec();
            pRobotProgram->operatorControl(currTime, prevTime);
        }
    }
    
    progObj->m_pController->StopSimulation();
    return 0;
}

void SimulatedProgram::ProgramStart(bool bAutonomous)
{
    m_bAutonomous = bAutonomous;
    m_pThread = SDL_CreateThread(SimulatedProgram::prog_thread, this);
}

void SimulatedProgram::WaitUntilProgramEnds()
{
    if (m_pThread)
    {
        SDL_WaitThread(m_pThread, NULL);
        m_pThread = NULL;
    }
}
