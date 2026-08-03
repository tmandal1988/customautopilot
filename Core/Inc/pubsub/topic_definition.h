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
	PARAMETER_UPDATE,
#if RTOS_METRICS_LOGGING_ENABLE
	RTOS_METRICS,
#endif
    MAX_TOPICS
};

static_assert(static_cast<int>(TopicID::IST8310) == 10,
              "Topic 10 is part of the persisted binary log format");
static_assert(static_cast<int>(TopicID::PARAMETER_UPDATE) == 11,
              "Topic 11 is part of the persisted binary log format");
#if RTOS_METRICS_LOGGING_ENABLE
static_assert(static_cast<int>(TopicID::RTOS_METRICS) == 12,
              "Topic 12 is part of the persisted binary log format");
#endif

constexpr int MAX_SUBSCRIBERS = 32;
