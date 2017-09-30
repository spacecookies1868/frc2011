/*
 *  SimHardware.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/30/10.
 *
 */

#ifndef SIM_HARDWARE_H
#define SIM_HARDWARE_H

#include <stdint.h>
#include "SDL.h"
#include "SDL_mutex.h"
#include "SDL_thread.h"

class SimulatedRobotModel;
class SimulatedField;
class SimulatorController;

class SimHardware
{
public:
    
    static SimHardware* GetInstance();
    
    void Initialize(SimulatedRobotModel* pRobot, SimulatedField* pField, SimulatorController* pController);
    
    void Destroy();
    
    double GetTimeSec() const {return (static_cast<double>(m_simulationTime) / 1000.0); }
    
    uint32_t GetTimeMilliSec() const { return m_simulationTime; }
    
    void WaitTimeMilliSec(uint32_t ms);
    
private:
    
    SimHardware();
    ~SimHardware();
    
    static int threadFunc(void*);
    void threadLoop();
    
    static SimHardware* s_pSimHardware;
    
    uint32_t    m_simulationTime;
    
    SimulatedRobotModel*    m_pRobotModel;
    SimulatedField*         m_pField;
    SimulatorController*    m_pController;
    
    SDL_Thread* m_pThread;
    SDL_mutex*  m_lock;
};

#endif
