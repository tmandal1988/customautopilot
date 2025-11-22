/*
 * baro_data.h
 *
 *  Created on: Apr 3, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_BARO_DATA_H_
#define INC_MESSAGES_BARO_DATA_H_

// Instruct the compiler not to add any padding between the members of a structure
#define PACKED __attribute__((__packed__))

struct PACKED BaroData{
	float press_pa;
	float temp_degc;
	uint64_t timestamp_ms;
};



#endif /* INC_MESSAGES_BARO_DATA_H_ */
