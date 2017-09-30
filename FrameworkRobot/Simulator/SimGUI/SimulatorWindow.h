/*
 *  GLScreen.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/29/10.
 *
 */

#ifndef GL_SCREEN_H
#define GL_SCREEN_H

#include "SDL.h"
#include <stdint.h>
#include <math.h>
#include "Shared/Vector3.h"

class SimulatedRobotModel;
class SimulatedField;
class SimulatorController;
class RobotProgram;

struct RobotPositionRecord
{
    explicit RobotPositionRecord()
    : simTime(0)
    , pos(0,0,0)
    , angle(0.f)
    {
    }
    
    bool PositionChanged(const RobotPositionRecord& rhs)
    {
        if (simTime != rhs.simTime)
            return (pos - rhs.pos).magnitude() > 0.0;
        else 
            return false;
    }
    
    bool AngleChanged(const RobotPositionRecord& rhs)
    {
        if (simTime != rhs.simTime)
            return fabs(angle - rhs.angle) >= 3.f;
        else 
            return false;
    }
    
    uint32_t simTime;
    
    Vector3 pos;
    float angle;
};

class SimulatorWindow
{
public:
    
    static void StartGLScreen(bool fullscreen, 
                              const SimulatedRobotModel* pRobotModel,
                              const RobotProgram* pRobotProgram,
                              const SimulatedField* pSimulatedField,
                              SimulatorController* pController);
    
    static void FinishGLScreen();
    
    static void UpdateRobot(const RobotPositionRecord& newRecord);
    
    static void ResetView();
    
    static void RotateMouseMove(uint32_t x, uint32_t y, int32_t xrel, int32_t yrel);
    
    static void ScaleMouseMove(uint32_t x, uint32_t y, int32_t scale);
    
    static void Reset3DView();
    
    static void ResizeWindow(uint32_t width, uint32_t height);
    
private:
    
    static int  mainLoop(void*);
    
    static SDL_Thread*  s_DisplayThread;    
    
    static const SimulatedRobotModel* s_pRobotModel;
    
    static const RobotProgram* s_pRobotProgram;
    
    static const SimulatedField* s_pSimulatedField;
    
    static const SimulatorController* s_pController;
    
};

#endif
