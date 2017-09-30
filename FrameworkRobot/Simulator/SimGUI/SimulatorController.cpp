/*
 *  SimulatorModel.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 10/30/10.
 *
 */
#include <stdint.h>
#include "SimulatorController.h"
#include "SDL.h"
#include "SimHardware.h"
#include "SimulatedProgram.h"
#include "SimulatorWindow.h"
#include "SimulatedRobot/SimulatedXBoxController.h"

SimulatorController::SimulatorController()
: m_leftBtnPressCount(0)
, m_rightBtnPressCount(0)
, m_bPaused(false)
, m_bProgramRunning(false)
, m_bSimulatorRunning(true)
, m_bActive(true)
, m_pProgram(NULL)
{
}

SimulatorController::~SimulatorController()
{
}

void SimulatorController::Initialize(SimulatedProgram* pProgram, SimulatedXBoxDriveController* pXBoxController)
{
    m_pProgram = pProgram;
    m_pXBoxController = pXBoxController;
}

void DisplayModifiers(SDL_KeyboardEvent *key)
{
	SDLMod modifier = key->keysym.mod;
	if( modifier & KMOD_NUM ) printf( "NUMLOCK " );
	if( modifier & KMOD_CAPS ) printf( "CAPSLOCK " );
	if( modifier & KMOD_MODE ) printf( "MODE " );
	if( modifier & KMOD_LCTRL ) printf( "LCTRL " );
	if( modifier & KMOD_RCTRL ) printf( "RCTRL " );
	if( modifier & KMOD_LSHIFT ) printf( "LSHIFT " );
	if( modifier & KMOD_RSHIFT ) printf( "RSHIFT " );
	if( modifier & KMOD_LALT ) printf( "LALT " );
	if( modifier & KMOD_RALT ) printf( "RALT " );
	if( modifier & KMOD_LMETA ) printf( "LMETA " );
	if( modifier & KMOD_RMETA ) printf( "RMETA " );
}

void SimulatorController::StopSimulation()
{
    m_bSimulatorRunning = false;
}

void SimulatorController::RunEventLoop()
{
    SDL_Event event;
    bool    bMouseLeftDown=false;

    bool bQuit = false;

    uint32_t numJoysticks = SDL_NumJoysticks();

    printf("Num Joysticks Detected %d\n", numJoysticks);

    SDL_Joystick* joystick = 0;
    uint32_t numJoystickAxes=0;
    uint32_t numJoystickButtons=0;
    uint32_t numJoystickBalls=0;
    uint32_t numJoystickHats=0;
    if (numJoysticks > 0)
    {
        joystick = SDL_JoystickOpen(0);
        SDL_JoystickEventState(SDL_ENABLE);

        numJoystickAxes = SDL_JoystickNumAxes(joystick);
        numJoystickButtons = SDL_JoystickNumButtons(joystick);
        numJoystickBalls = SDL_JoystickNumBalls(joystick);
        numJoystickHats = SDL_JoystickNumHats(joystick);


        printf("Using Joystick %s Axes %d Buttons %d Balls %d Hats %d\n",
               SDL_JoystickName(0), numJoystickAxes, numJoystickButtons, numJoystickBalls, numJoystickHats);

    }

    while(!bQuit) {
        //while(SDL_PollEvent(&event))
        SDL_WaitEvent(&event);

        {
            switch(event.type){
                case SDL_KEYDOWN:
                    break;
                case SDL_KEYUP:
                    {
                        SDL_KeyboardEvent eventKey = event.key;

                        switch(eventKey.keysym.sym){
                            case SDLK_p:
                                m_bPaused = !m_bPaused;
                                break;
                            case SDLK_r:
                                SimulatorWindow::Reset3DView();
                                break;
                            case SDLK_q:
                                m_bProgramRunning = false;
                                bQuit=true;
                                break;
							case SDLK_SPACE:
								if (!m_bProgramRunning)
                                {
                                    m_bProgramRunning = true;
                                    m_pProgram->ProgramStart(true);
                                }
								else
									m_bPaused = !m_bPaused;
                                break;
                            case SDLK_s:
                                if (m_bProgramRunning == false)
                                {
                                    m_bProgramRunning = true;
                                    m_pProgram->ProgramStart(true);
                                }
                                break;
                            case SDLK_v:
                                SimulatorWindow::Reset3DView();
                                break;
                            default:
                                break;
                        }

                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (bMouseLeftDown==true)
                    {
                        SimulatorWindow::RotateMouseMove(event.motion.x, event.motion.y,
                                                         event.motion.xrel, event.motion.yrel);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT && bMouseLeftDown)
                        bMouseLeftDown=false;
                    else if (event.button.button == SDL_BUTTON_WHEELUP)
                    {
                        SimulatorWindow::ScaleMouseMove(event.button.x, event.button.y, -1);
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        bMouseLeftDown=true;
                    } else if (event.button.button == SDL_BUTTON_WHEELDOWN)
                    {
                        SimulatorWindow::ScaleMouseMove(event.button.x, event.button.y, 1);
                    }
                    break;
                case SDL_VIDEORESIZE:
                    SimulatorWindow::ResizeWindow(event.resize.w, event.resize.h);
                    break;
                case SDL_QUIT:
                    m_bProgramRunning = false;
					bQuit=true;
                    break;
                case SDL_JOYAXISMOTION:
                    if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
                    {
                        static const double axisMultiplier = 1.0 / (32.0 * 1024.0);
                        double value = event.jaxis.value * axisMultiplier;
                        m_pXBoxController->SetAxisValue(event.jaxis.axis, value);
                    } else
                        m_pXBoxController->SetAxisValue(event.jaxis.axis, 0.0);
                    
                    break;
                case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
                    if (m_pXBoxController->isStartButton(event.jbutton.button))
                    {
                        if (m_bProgramRunning == false)
                        {
                            m_bProgramRunning = true;
                            m_pProgram->ProgramStart(false);
                        }
                    }
                    m_pXBoxController->SetButtonValue(event.jbutton.button, true);
                    break;
                case SDL_JOYBUTTONUP:  /* Handle Joystick Button Released */
                    if (event.jbutton.button != 10)
                    {
                        m_pXBoxController->SetButtonValue(event.jbutton.button, false);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (numJoysticks > 0)
    {
        SDL_JoystickClose(joystick);
    }

    m_bActive=false;
}
