/*
 * motor_protocol_config.h
 *
 * Single definition of the motor output protocol switch, so the driver
 * sources and the selection header agree without including each other.
 *
 * Production default is analog PWM. The CMake option ENABLE_DSHOT defines
 * MOTOR_PROTOCOL_DSHOT=1.
 */

#pragma once

#ifndef MOTOR_PROTOCOL_DSHOT
#define MOTOR_PROTOCOL_DSHOT 0
#endif

#if (MOTOR_PROTOCOL_DSHOT != 0) && (MOTOR_PROTOCOL_DSHOT != 1)
#error "MOTOR_PROTOCOL_DSHOT must be 0 or 1"
#endif
