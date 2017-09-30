/*
 *  RobotModel.cpp
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/23/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#include "RobotModel.h"

RobotModel::WheelValues RobotModel::GetWheelRadii() const
{
    WheelValues outValues;
    outValues[kLeftWheel] = GetWheelRadius(kLeftWheel);
    outValues[kRightWheel] = GetWheelRadius(kRightWheel);
    return outValues;
}

RobotModel::WheelValues RobotModel::GetWheelGearingRatios() const
{
    WheelValues outValues;
    outValues[kLeftWheel] = GetWheelGearingRatio(kLeftWheel);
    outValues[kRightWheel] = GetWheelGearingRatio(kRightWheel);
    return outValues;
}

RobotModel::WheelValues RobotModel::GetMaxMotorDrives() const
{
    WheelValues outValues;
    outValues[kLeftWheel] = GetMaxMotorDrive(kLeftWheel);
    outValues[kRightWheel] = GetMaxMotorDrive(kRightWheel);
    return outValues;
}

void    RobotModel::SetWheelMotorValues(const WheelValues& drive)
{
    SetWheelMotorValue(kLeftWheel, drive[kLeftWheel]);
    SetWheelMotorValue(kRightWheel, drive[kRightWheel]);
}

RobotModel::WheelValues RobotModel::GetWheelEncoderValues() const
{
    WheelValues outValues;
    outValues[kLeftWheel] = GetWheelEncoderValue(kLeftWheel);
    outValues[kRightWheel] = GetWheelEncoderValue(kRightWheel);
    return outValues;
}

void    RobotModel::EnableWheelEncoders()
{
    EnableWheelEncoder(kLeftWheel);
    EnableWheelEncoder(kRightWheel);
}

void    RobotModel::DisableWheelEncoders()
{
    DisableWheelEncoder(kLeftWheel);
    DisableWheelEncoder(kRightWheel);
}

void    RobotModel::ResetWheelEncoders()
{
    ResetWheelEncoder(kLeftWheel);
    ResetWheelEncoder(kRightWheel);
}

