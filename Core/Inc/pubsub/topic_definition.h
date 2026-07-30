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
	RESERVED_MAVLINKPARAMS,  // Keep persisted topic numbers stable.
	MTF01P,
	// Append persisted log topics here so existing numeric IDs stay stable.
	IST8310,
    MAX_TOPICS
};

static_assert(static_cast<int>(TopicID::IST8310) == 10,
              "Topic 10 is part of the persisted binary log format");

constexpr int MAX_SUBSCRIBERS = 32;
