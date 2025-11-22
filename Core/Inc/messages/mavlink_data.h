/*
 * mavlink_data.h
 *
 *  Created on: Jun 17, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_MAVLINK_DATA_H_
#define INC_MESSAGES_MAVLINK_DATA_H_

#define PACKED __attribute__((__packed__))

struct PACKED MavlinkData {
  uint8_t arm_cmd_issued;
  uint64_t timestamp_ms;
};

static_assert(sizeof(MavlinkData) == 9, "Mavlink Data size should be 9 bytes!");



#endif /* INC_MESSAGES_MAVLINK_DATA_H_ */
