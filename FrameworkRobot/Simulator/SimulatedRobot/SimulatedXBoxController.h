/*
 *  SimulatedXBoxController.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 2/4/11.
 *
 */

#ifndef SIMULATED_XBOX_CONTROLLER_H
#define SIMULATED_XBOX_CONTROLLER_H

#include "Shared/CommonIncludes.h"
#include "Shared/Vector3.h"
#include "Shared/NumericalVector.hpp"
#include "StudentRobotProgram/XBoxDriveController.h"

class SimulatedXBoxDriveController : public XBoxDriveControl
{
public:

    explicit SimulatedXBoxDriveController();
    virtual ~SimulatedXBoxDriveController();

protected:

    friend class SimulatorController;

    void SetAxisValue(uint32_t axis, double value);
    void SetButtonValue(uint32_t button, bool bPressed);

    virtual bool getButton(XBoxDriveControl::Button b);
    virtual float getAxis(XBoxDriveControl::Axis a);

    bool    isStartButton(uint32_t b) const;

private:

    NumericalVector<XBoxDriveControl::NumAxes> m_axisValue;
    bool                                        m_buttonCurrState[XBoxDriveControl::NumButtons];

};

#endif
