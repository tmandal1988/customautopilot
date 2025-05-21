/*
 * gps_data.h
 *
 *  Created on: Apr 8, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_GPS_DATA_H_
#define INC_MESSAGES_GPS_DATA_H_

// Instruct the compiler not to add any padding between the members of a structure
#define PACKED __attribute__((__packed__))

struct PACKED GpsData{
		double latitude_rad;
		double longitude_rad;
		float altitude_m;
		float vn_mps;
		float ve_mps;
		float vd_mps;
		uint8_t valid;
		uint8_t fix_type;
		uint8_t flags;
		uint32_t i_tow;
		uint8_t checksum_valid;
		uint64_t timestamp_us;
};



#endif /* INC_MESSAGES_GPS_DATA_H_ */
