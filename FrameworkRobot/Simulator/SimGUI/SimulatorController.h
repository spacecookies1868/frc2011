/*
 *  SimulatorModel.h
 *  RoboSimOSX
 *
 *  Copyright 2010 Apple Inc.. All rights reserved.
 *
 */

#ifndef SIMULATOR_MODEL_H
#define SIMULATOR_MODEL_H

class StudentProgram;
class SimulatedProgram;
class SimulatedXBoxDriveController;

class SimulatorController
{
public:
    
    explicit SimulatorController();
    
    virtual ~SimulatorController();
    
    bool    IsProgramRunning() const {  return m_bSimulatorRunning; }
    bool    IsProgramPaused() const { return m_bPaused; }
    bool    IsProgramActive() const { return m_bActive; }
    
    void    Initialize(SimulatedProgram* pProgram, SimulatedXBoxDriveController* pXBoxController);
    
    void    RunEventLoop();
    
    void    StopSimulation();
    
private:
    
    int32_t    m_leftBtnPressCount;
    int32_t    m_rightBtnPressCount;
    bool        m_bPaused;
    bool        m_bProgramRunning;
    bool        m_bSimulatorRunning;
    bool        m_bActive;
    SimulatedProgram* m_pProgram;
    SimulatedXBoxDriveController* m_pXBoxController;
};

#endif
