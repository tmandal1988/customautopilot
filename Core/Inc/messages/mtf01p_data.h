/*
 * mtf01p_data.h
 *
 *  Created on: Jul 27, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_MTF01P_DATA_H_
#define INC_MESSAGES_MTF01P_DATA_H_

// Instruct the compiler not to add any padding between the members of a structure
#define PACKED __attribute__((__packed__))

struct PACKED Mtf01pData{
	uint32_t  time_ms;		    // System time in ms
	uint32_t  distance;		    // distance(mm), 0 Indicates unavailable
	uint8_t   strength;	        // signal strength
	uint8_t   precision;	    // distance precision
	uint8_t   dis_status;	    // distance status
	int16_t   flow_vel_x;	    // optical flow velocity in x
	int16_t   flow_vel_y;	    // optical flow velocity in y
	uint8_t   flow_quality;	    // optical flow quality
	uint8_t   flow_status;	    // optical flow status
	uint64_t  timestamp_ms;
};

#endif /* INC_MESSAGES_MTF01P_DATA_H_ */
