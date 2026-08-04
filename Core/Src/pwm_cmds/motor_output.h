/*
 * motor_output.h
 *
 * Selects the motor output protocol at build time.
 *
 * Both drivers expose the same interface to ControlPipeline:
 *     InitializeOutputs()          - configure the timer and arm the ESCs
 *     ApplyPwmData(const PwmData&) - emit one frame of motor commands
 *
 * The choice is a build-time switch rather than a runtime one on purpose.
 * BLHeli_32 latches the input protocol during ESC power-up and keeps it until
 * the next power cycle, so a running aircraft could never act on a runtime
 * change. Resolving it at compile time also keeps the 250 Hz output path free
 * of a per-cycle branch.
 *
 * Default is analog PWM. Enable DShot with -DENABLE_DSHOT=ON.
 */

#pragma once

#include "motor_protocol_config.h"

#if MOTOR_PROTOCOL_DSHOT
#include "pwm_cmds_dshot.h"
using MotorOutput = PwmCmdsDshot;
#else
#include "pwm_cmds_analog.h"
using MotorOutput = PwmCmds;
#endif
