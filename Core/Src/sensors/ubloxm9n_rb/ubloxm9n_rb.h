/*
 * ubloxm9n_rb.h
 *
 *  Created on: May 20, 2025
 *      Author: tanmay
 */

#pragma once


#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "messages/gps_data.h"
#include "pubsub/publisher.h"
#include "constants.h"
#include "debug.h"

#include <cstring>  // Include this header for memcpy
#include <inttypes.h>


class ReadUbloxM9nRb : public TaskBase {
public:
	ReadUbloxM9nRb(UART_HandleTypeDef* huart);
	virtual ~ReadUbloxM9nRb(){
	}

	void Run() override;

	static ReadUbloxM9nRb* ubloxm9n_rb_instance_handle_;

private:
	static constexpr uint16_t READ_INTERVAL_MS = 25; // 40Hz
	// Tx and Rx max delay (ms) in polling mode
	static constexpr uint32_t txrx_delay_ms_ = 50;

	static constexpr uint32_t supported_baudrates_[] = {
	        9600U, 19200U, 38400U, 57600U, 115200U, 230400U, 460800U, 921600U
	};

	//Ublox M9N message definitions
	static constexpr uint8_t ACK_NAK_PAYLOAD_LENGTH 								= 2;
	static constexpr uint16_t MAX_BUFF_SIZE 										= 1024*4;
	static constexpr uint8_t UBX_SYNC_CHAR_1 										= 0xB5;
	static constexpr uint8_t UBX_SYNC_CHAR_2 										= 0x62;
	static constexpr uint8_t UBX_NAV_PVT_SIZE 		 								= 100;
	static constexpr uint16_t UBX_MAX_PAYLOAD										= 1024;

	static constexpr uint8_t CLASS_MON 												= 0x0A;
	static constexpr uint8_t ID_VER 												= 0x04;

	static constexpr uint8_t CLASS_CFG 												= 0x06;
	static constexpr uint8_t ID_MSG 												= 0x01;
	static constexpr uint8_t ID_PRT 												= 0x00;
	static constexpr uint8_t ID_RST 												= 0x04;
	static constexpr uint8_t ID_RATE 												= 0x08;
	static constexpr uint8_t ID_CFG 												= 0x09;
	static constexpr uint8_t ID_NAV5 												= 0x24;


	static constexpr uint8_t CLASS_ACK 												= 0x05;
	static constexpr uint8_t ID_NAK 												= 0x00;
	static constexpr uint8_t ID_ACK 												= 0x01;

	static constexpr uint8_t CLASS_NAV 												= 0x01;
	static constexpr uint8_t ID_PVT 												= 0x07;

	// Define an enum for ACK and NAK messages
	enum class UbxAckEnum{
		STATUS_NAK = 0,         // 0
		STATUS_ACK,          	// 1
		STATUS_FAIL	 			// 2
	};

	struct PACKED UbxMessage {
		uint8_t class_id;    	// Changed from 'class' to avoid conflict with C++ keyword
		uint8_t msg_id;			// Msg ID
		uint16_t length;    	// Length of the payload
		void* payload;      	// Pointer to the payload data
	};

	struct PACKED PollingMessage {
		uint8_t class_id;
		uint8_t msg_id;
	};

	struct PACKED UbloxM9nVer {
		unsigned char sw_version[30];
		unsigned char hw_version[10];
		unsigned char ext1[30];
		unsigned char ext2[30];
		unsigned char ext3[30];
		unsigned char ext4[30];
		unsigned char ext5[30];
		unsigned char ext6[30];
	};

	struct PACKED UbloxM9nCfgCfg {
		uint32_t clear_mask;
		uint32_t save_mask;
		uint32_t load_mask;
		uint8_t device_mask;
	};

	struct PACKED UbloxM9nCfgRst {
		uint16_t nav_bbr_mask;
		uint8_t reset_mode;
		uint8_t reserved;
	};

	struct PACKED UbloxM9nCfgPrt {
		uint8_t port_id;
		uint8_t reserved;
		uint16_t tx_ready;
		uint32_t mode;
		uint32_t baudrate;
		uint16_t in_proto_mask;
		uint16_t out_proto_mask;
		uint16_t flags;
		uint8_t reserved2[2];
	};

	struct PACKED UbloxM9nCfgMsg {
		uint8_t msg_class;
		uint8_t msg_id;
		uint8_t rate[6];
	};

	struct PACKED UbloxM9nCfgNav5 {
		uint16_t mask;
		uint8_t dyn_model;
		uint8_t fix_mode;
		int32_t fixed_alt;
		uint32_t fixed_alt_var;
		int8_t min_elev;
		uint8_t dr_limit;
		uint16_t p_dop;
		uint16_t t_dop;
		uint16_t p_acc;
		uint16_t t_acc;
		uint8_t static_hold_thresh;
		uint8_t dgnss_timeout;
		uint8_t cno_thresh_num_svs;
		uint8_t cno_thresh;
		uint8_t reserved[2];
		uint16_t static_hold_max_dist;
		uint8_t utc_standard;
		uint8_t reserved1[5];
	};

	struct PACKED UbloxM9nNavPvt {
		uint32_t i_tow;              // GPS time of week of the navigation epoch (ms)
		uint16_t year;               // Year (UTC)
		uint8_t month;               // Month, range 1..12 (UTC)
		uint8_t day;                 // Day of month, range 1..31 (UTC)
		uint8_t hour;                // Hour of day, range 0..23 (UTC)
		uint8_t min;                 // Minute of hour, range 0..59 (UTC)
		uint8_t sec;                 // Seconds of minute, range 0..60 (UTC)
		uint8_t valid;               // Validity flags (see below for details)
		uint32_t t_acc;              // Time accuracy estimate (UTC) (ns)
		int32_t nano;                // Fraction of second (UTC) -1e9..1e9 (ns)
		uint8_t fix_type;            // GNSS fix type (0: no fix, 1: dead reckoning only, 2: 2D-fix, 3: 3D-fix, 4: GNSS + dead reckoning combined, 5: time only fix)
		uint8_t flags;               // Fix status flags
		uint8_t flags2;              // Additional flags
		uint8_t num_sv;              // Number of satellites used in navigation solution
		int32_t lon;                 // Longitude (1e-7 degrees)
		int32_t lat;                 // Latitude (1e-7 degrees)
		int32_t height;              // Height above ellipsoid (mm)
		int32_t h_msl;               // Height above mean sea level (mm)
		uint32_t h_acc;              // Horizontal accuracy estimate (mm)
		uint32_t v_acc;              // Vertical accuracy estimate (mm)
		int32_t vel_n;               // NED north velocity (mm/s)
		int32_t vel_e;               // NED east velocity (mm/s)
		int32_t vel_d;               // NED down velocity (mm/s)
		int32_t g_speed;             // Ground speed (2-D) (mm/s)
		int32_t heading;             // Heading of motion (2-D) (1e-5 degrees)
		uint32_t s_acc;              // Speed accuracy estimate (mm/s)
		uint32_t heading_acc;        // Heading accuracy estimate (1e-5 degrees)
		uint16_t p_dop;              // Position DOP
		uint8_t reserved1[6];        // Reserved for future use
		int32_t head_veh;            // Heading of vehicle (2-D) (1e-5 degrees)
		int16_t mag_dec;             // Magnetic declination (1e-2 degrees)
		uint16_t mag_acc;            // Magnetic declination accuracy (1e-2 degrees)
	};

	struct PACKED UbloxM9nCfgRate {
		uint16_t meas_rate;
		uint16_t nav_rate;
		uint16_t time_ref;
	};

	UART_HandleTypeDef* gps_uart_;  // UART handle to receive data from GPS
	uint32_t current_baudrate_;

	uint8_t rx_buffer_[MAX_BUFF_SIZE] = {0};
	size_t last_read_index_ = 0;
	uint8_t nav_pvt_raw_buff_[UBX_NAV_PVT_SIZE] = {0};
	UbloxM9nNavPvt nav_pvt_data_;

	bool new_nav_pvt_frame_ = false;

	GpsData gps_data_;

	enum ParserState{
		WAIT_SYNC1,
		WAIT_SYNC2,
		WAIT_CLASS,
		WAIT_ID,
		WAIT_LEN1,
		WAIT_LEN2,
		WAIT_PAYLOAD,
		WAIT_CK_A,
		WAIT_CK_B
	};

	// Generic UBX packet structure
	struct PACKED UbxPacket {
	    uint8_t cls;
	    uint8_t id;
	    uint16_t len;
	    uint8_t payload[UBX_MAX_PAYLOAD];
	};

	ParserState parser_state_ = ParserState::WAIT_SYNC1;
	UbxPacket packet_{};
	uint16_t payload_idx_ = 0;
	uint8_t ck_a_ = 0;
	uint8_t ck_b_ = 0;

	// Initializes the communication interface
	void GetCurrentBaudrate();
	bool InitGps(uint32_t baudrate, uint16_t time_bw_samples_ms, uint8_t nav_rate);
	bool ConfigGpsMeasRate(uint16_t time_bw_samples_ms, uint8_t nav_rate);
	void UbloxM9nUartInit(uint32_t baudrate);
	void FlushUartDataRegister();
	void SetBaudrate(const uint32_t baudrate);
	void CalculateChecksum(uint8_t *buffer, uint16_t length, uint8_t *ck_a, uint8_t *ck_b);
	bool TxUartUbxPollCmd(const UbxMessage *message, const uint16_t wait_ms);
	bool RxUartUbxPollMsg(const uint8_t class_id, const uint8_t msg_id, const uint16_t wait_ms);
	bool ResetGps();
	bool UbxSaveCfg(uint32_t save_mask);
	bool GetVersion();
	bool ConfigGpsUart1(const uint32_t baudrate);
	bool ConfigPrtProtocol(uint8_t port_id, uint16_t proto_mask);
	bool ConfigAuxPrts();
	bool ConfigNav5();
	bool EnableNavPvtMsg();
	bool DisableNavPvtMsg();

	// Function to extract UART data received by DMA
	bool ProcessUbloxFrame();
	// Function to parse the extracted UART data
	bool ParseUbx(uint8_t data);

	inline void UpdateChecksum(uint8_t byte) {
		ck_a_ += byte;
		ck_b_ += ck_a_;
	}
};

