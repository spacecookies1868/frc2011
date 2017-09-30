/*
 *  RobotTuningTests.h
 *  RoboSimOSX
 *
 *  Created by Dmitriy Solomonov on 1/29/11.
 *  Copyright 2011 Apple Inc.. All rights reserved.
 *
 */

#ifndef ROBOT_TUNING_TESTS
#define ROBOT_TUNING_TESTS

class RobotModel;

/*  These tests are simple autonomous programs that implement their own wait time
 */

void FindAcceleration(double dir, RobotModel* pRobot);

void FindEncoderConstant(RobotModel* pRobot);

double FindStoppingDistance(double speed, RobotModel* pRobot);

double FindVelocity(RobotModel* pRobot);

void TempDriveStraight(double distance, double speed, RobotModel* pRobot);

void TestPID(RobotModel* pRobot, double speed, double driveTime);

void TurnInPlace(double degreesToTurn, RobotModel* pRobot);

void FindRotationTensor(double l, double r, RobotModel* pRobot);

void MotorPortTest(RobotModel* pRobot);

#endif
