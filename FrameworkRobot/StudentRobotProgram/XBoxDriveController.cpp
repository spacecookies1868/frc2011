#include "XBoxDriveController.h"
#include "Shared/NumericalVector.hpp"
#include "AcceleratorArcade.h"
#include <math.h>

void XBoxDriveControl::update()
{


	bool gearTarget = getButton(LB);

	if(m_gearActive && !gearTarget)
	{
		if(m_gearType == RobotModel::kHighGear)
			m_gearType = RobotModel::kLowGear;
		else
			m_gearType = RobotModel::kHighGear;
	}

	m_gearActive = gearTarget;

	NumericalVector<2> driveVec = m_arcade.update(-getAxis(LeftStickY),getAxis(RightStickX),m_gearType, false);
	if (getButton(RB))
	{
		driveVec[0] = -getAxis(LeftStickY);
		driveVec[1] = getAxis(RightStickY);
	}
	
	m_motorLeft = driveVec[0];
	m_motorRight = driveVec[1];
}

XBoxDriveControl::XBoxDriveControl()
: m_motorLeft(0)
, m_motorRight(0)
, m_gearType(RobotModel::kLowGear)
, m_gearActive(false)
, m_arcade(0.06)
{

}

RobotModel::GearSet XBoxDriveControl::getDesiredGear() const{
	return m_gearType;
}

XBoxDriveControl::~XBoxDriveControl()
{
}
