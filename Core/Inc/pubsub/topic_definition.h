/*
 * topic_definition.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

// Enum for topic identification
enum class TopicID {
    ICM20948,
	BMP390L,
	UBLOXM9N,
	RCCHANNELS,
	EKF,
	PWM,
	FCSDEBUG,
	MAVLINK,
	MAVLINKPARAMS,
    MAX_TOPICS
};

constexpr int MAX_SUBSCRIBERS = 32;
