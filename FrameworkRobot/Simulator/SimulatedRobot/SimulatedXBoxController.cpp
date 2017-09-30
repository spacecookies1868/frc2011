/*
 *  SimulatedXBoxController.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 2/4/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#include "SimulatedXBoxController.h"


SimulatedXBoxDriveController::SimulatedXBoxDriveController()
{
    memset(m_buttonCurrState, 0, sizeof(m_buttonCurrState));
}

SimulatedXBoxDriveController::~SimulatedXBoxDriveController()
{
}

#if defined(__APPLE__)
    enum XBox360Axes
    {
        kLeftStickXAxis=0,
        kLeftStickYAxis=1,
        kRightStickXAxis=2,
        kRightStickYAxis=3,
        kLeftTriggerAxis=4,
        kRightTriggerAxis=5,
        kNumAxes=6
    };

    enum XBox360Buttons
    {
        kUpButton=0,
        kDownButton=1,
        kLeftButton=2,
        kRightButton=3,
        kStartButton=4,
        kBackButton=5,
        kLeftStickButton=6,
        kRightStickButton=7,
        kLeftBumperButton=8,
        kRightBumperButton=9,
        kXBOXButton=10,
        kAButton=11,
        kBButton=12,
        kXButton=13,
        kYButton=14,
        kNumButtons=15
    };

#else

    enum XBox360Axes
    {
        kLeftStickXAxis=0,
        kLeftStickYAxis=1,
        kRightStickXAxis=4,
        kRightStickYAxis=3,
        kLeftTriggerAxis=2,
        kRightTriggerAxis=2
    };

    enum XBox360Buttons
    {
        kStartButton=7,
        kBackButton=6,
        kLeftStickButton=8,
        kRightStickButton=9,
        kLeftBumperButton=4,
        kRightBumperButton=5,
        kAButton=0,
        kBButton=1,
        kXButton=2,
        kYButton=3
    };
#endif

static XBoxDriveControl::Axis ConvertToAxis(uint32_t axis, double withVal)
{
    XBoxDriveControl::Axis outAxis=XBoxDriveControl::NumAxes;
    switch (axis)
    {
    case  kLeftStickXAxis:
        outAxis = XBoxDriveControl::LeftStickX;
        break;
    case kLeftStickYAxis:
        outAxis = XBoxDriveControl::LeftStickY;
        break;
    case kRightStickXAxis:
        outAxis = XBoxDriveControl::RightStickX;
        break;
    case kRightStickYAxis:
        outAxis = XBoxDriveControl::RightStickY;
        break;
    case kLeftTriggerAxis:
#if defined(__APPLE__)
    case kRightTriggerAxis:
#endif
        if (withVal >= 0)
            outAxis = XBoxDriveControl::LeftTrigger;
        else
            outAxis = XBoxDriveControl::RightTrigger;
        break;
    default:
        break;
    };
    return outAxis;
}

static XBoxDriveControl::Button ConvertToButton(uint32_t b)
{
    XBoxDriveControl::Button outButton = XBoxDriveControl::NumButtons;
    switch (b)
    {
        case kStartButton:
            outButton = XBoxDriveControl::START;
            break;
        case kBackButton:
            outButton = XBoxDriveControl::BACK;
            break;
        case kLeftStickButton:
            outButton = XBoxDriveControl::LStickClick;
            break;
        case kRightStickButton:
            outButton = XBoxDriveControl::RStickClick;
            break;
        case kLeftBumperButton:
            outButton = XBoxDriveControl::LB;
            break;
        case kRightBumperButton:
            outButton = XBoxDriveControl::RB;
            break;
        case kAButton:
            outButton = XBoxDriveControl::A;
            break;
        case kBButton:
            outButton = XBoxDriveControl::B;
            break;
        case kXButton:
            outButton = XBoxDriveControl::X;
            break;
        case kYButton:
            outButton = XBoxDriveControl::Y;
            break;
        default:
        break;
    };
    return outButton;
}

bool
SimulatedXBoxDriveController::isStartButton(uint32_t b) const
{
    return (b == kStartButton);
}

void
SimulatedXBoxDriveController::SetAxisValue(uint32_t a, double value)
{
    XBoxDriveControl::Axis axis = ConvertToAxis(a, value);
    if (axis != XBoxDriveControl::NumAxes)
        m_axisValue[axis] = value;
}

void
SimulatedXBoxDriveController::SetButtonValue(uint32_t b, bool bPressed)
{
    XBoxDriveControl::Button button = ConvertToButton(b);
    if (button != XBoxDriveControl::NumButtons)
        m_buttonCurrState[button] = bPressed;
}

bool
SimulatedXBoxDriveController::getButton(XBoxDriveControl::Button b)
{
    return m_buttonCurrState[b];
}

float
SimulatedXBoxDriveController::getAxis(XBoxDriveControl::Axis a)
{
    return m_axisValue[a];
}

