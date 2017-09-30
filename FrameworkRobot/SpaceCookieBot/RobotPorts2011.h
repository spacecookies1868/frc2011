#ifndef ROBOT_PORTS_2011_H
#define ROBOT_PORTS_2011_H

#include "Shared/CommonIncludes.h"

static const uint32_t LEFT_MOTOR_A_PWM_PORT = 3;
static const uint32_t LEFT_MOTOR_B_PWM_PORT = 4;

static const uint32_t RIGHT_MOTOR_A_PWM_PORT = 6;
static const uint32_t RIGHT_MOTOR_B_PWM_PORT = 5;

static const uint32_t ARM_MOTOR_A_PWM_PORT = 1;
static const uint32_t ARM_MOTOR_B_PWM_PORT = 2;

static const uint32_t LEFT_WHEEL_ENCODER_A_PWM_PORT = 3; //encoders on digital ports
static const uint32_t LEFT_WHEEL_ENCODER_B_PWM_PORT = 4;

static const uint32_t RIGHT_WHEEL_ENCODER_A_PWM_PORT = 5;
static const uint32_t RIGHT_WHEEL_ENCODER_B_PWM_PORT = 6;

static const uint32_t ARM_ENCODER_A_PWM_PORT = 13;
static const uint32_t ARM_ENCODER_B_PWM_PORT = 14;

static const uint32_t GYRO_PORT = 1;	//analog port

static const uint32_t GEAR_SHIFT_SOLENOID_CHAN=3;
static const uint32_t CLAW_OPEN_SOLENOID_CHAN = 1;
static const uint32_t CLAW_CLOSED_SOLENOID_CHAN = 6;
static const uint32_t ARM_RETRACTED_SOLENOID_CHAN = 4;
static const uint32_t ARM_EXTENDED_SOLENOID_CHAN = 5;
static const uint32_t MINIBOT_DEPLOY_SOLENOID_CHAN = 2;
static const uint32_t MINIBOT_LOWER_SOLENOID_CHAN = 7;
static const uint32_t MINIBOT_RAISE_SOLENOID_CHAN = 8;

static const uint32_t COMPRESSOR_PRESSURE_SWITCH_CHAN=1;
static const uint32_t COMPRESSOR_RELAY_CHAN=1;


#endif
