/*
 *  BaseProgram.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/31/10.
 *
 */

#ifndef SIMULATED_PROGRAM_H
#define SIMULATED_PROGRAM_H

#include "SDL.h"
#include "SDL_thread.h"

class RobotProgram;
class SimulatorController;

class SimulatedProgram
{
    
public:    
    
    explicit SimulatedProgram(RobotProgram* pRobotProg, SimulatorController* pController);
    
    virtual ~SimulatedProgram();
    
    void    ProgramStart(bool bAutonomous);
    
    void    WaitUntilProgramEnds();
    
private:
    
    static int prog_thread(void* inObj);
    
    SDL_Thread* m_pThread;
    
    RobotProgram* m_pRobotProg;
    SimulatorController* m_pController;
    
    bool            m_bAutonomous;
    
};

#endif
