/*
 * ubloxm9n_rb.cc
 *
 *  Created on: May 20, 2025
 *      Author: tanmay
 */

#include "ubloxm9n_rb.h"

// Definition of the static member variable
ReadUbloxM9nRb* ReadUbloxM9nRb::ubloxm9n_rb_instance_handle_ = nullptr;

extern UART_HandleTypeDef huart2;

// Defined in main.h
extern "C" {
    void Error_Handler(void);
}

ReadUbloxM9nRb read_ubloxm9n_rb_task_instance_(&huart2);

ReadUbloxM9nRb::ReadUbloxM9nRb(UART_HandleTypeDef* huart):
TaskBase("UbloxM9nTask2", 1296, osPriorityAboveNormal),
gps_uart_(huart){

ubloxm9n_rb_instance_handle_ = this;

}

/**
 * @brief Initialize USART for GPS communication
 * @param baudrate: The desired baud rate for UART communication
 * @retval None
 */
void ReadUbloxM9nRb::UbloxM9nUartInit(uint32_t baudrate)
{
	// Initialize USART2 with specified parameters
	gps_uart_->Instance = GPSUART;
	gps_uart_->Init.BaudRate = baudrate;
	gps_uart_->Init.WordLength = UART_WORDLENGTH_8B;
	gps_uart_->Init.StopBits = UART_STOPBITS_1;
	gps_uart_->Init.Parity = UART_PARITY_NONE;
	gps_uart_->Init.Mode = UART_MODE_TX_RX;
	gps_uart_->Init.HwFlowCtl = UART_HWCONTROL_NONE;
	gps_uart_->Init.OverSampling = UART_OVERSAMPLING_16;
	gps_uart_->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	gps_uart_->Init.ClockPrescaler = UART_PRESCALER_DIV1;
	gps_uart_->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	// Initialize UART and set FIFO thresholds
	if (HAL_UART_Init(gps_uart_) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(gps_uart_, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(gps_uart_, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(gps_uart_) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief Set the baud rate for UART
 * @param baudrate: The desired baud rate
 */
void ReadUbloxM9nRb::SetBaudrate(const uint32_t baudrate){
	HAL_UART_DMAStop(gps_uart_);
	osDelay(5);
	UbloxM9nUartInit(baudrate);
	osDelay(10);
	FlushUartDataRegister();
	osDelay(10);
	HAL_UART_Receive_DMA(gps_uart_, rx_buffer_, MAX_BUFF_SIZE);
	// The DMA write pointer restarts at 0 with this fresh transfer; the read
	// cursor must restart with it. Left stale, ProcessUbloxFrame() would read
	// the wraparound branch and misreport most of a freshly-zeroed buffer as
	// "available", burning a pass parsing garbage before sync recovers.
	last_read_index_ = 0;
	osDelay(10);
}

void ReadUbloxM9nRb::RecoverUartDma(){
	HAL_UART_DMAStop(gps_uart_);
	FlushUartDataRegister();
	HAL_UART_Receive_DMA(gps_uart_, rx_buffer_, MAX_BUFF_SIZE);
	last_read_index_ = 0;
}

void ReadUbloxM9nRb::UartError(UART_HandleTypeDef* huart){
	ReadUbloxM9nRb* instance = ubloxm9n_rb_instance_handle_;
	if ((instance != nullptr) && (huart == instance->gps_uart_)) {
		instance->uart_error_pending_.store(true, std::memory_order_release);
	}
}

void ReadUbloxM9nRb::FlushUartDataRegister(){
    // Check if there is data in the data register by reading the status register
    while (__HAL_UART_GET_FLAG(gps_uart_, UART_FLAG_RXNE)) {
        // Read the data register to clear the RXNE flag
        volatile uint8_t temp = (uint8_t)(gps_uart_->Instance->RDR & 0xFF);
        (void)temp; // Use (void) to prevent compiler warnings about unused variable
    }

    // Clear any other relevant flags (optional, depending on your error handling needs)
    __HAL_UART_CLEAR_OREFLAG(gps_uart_); // Clear overrun error flag, if set
}

/**
 * @brief Calculate checksum for UBX messages
 * @param buffer: Pointer to the message buffer
 * @param length: Length of the message
 * @param ck_a: Pointer to store the first checksum byte
 * @param ck_b: Pointer to store the second checksum byte
 */
void ReadUbloxM9nRb::CalculateChecksum(uint8_t *buffer, uint16_t length, uint8_t *ck_a, uint8_t *ck_b) {
	*ck_a = 0;
	*ck_b = 0;

	for (uint16_t i = 0; i < length; i++) {
		*ck_a += buffer[i];
		*ck_b += *ck_a;
	}
}

/* @brief Send a UBX poll command over UART DMA
 * @param message: Pointer to the UBX message structure
 * @return true if successful, false otherwise
 */
bool ReadUbloxM9nRb::TxUartUbxPollCmd(const UbxMessage *message, const uint16_t wait_ms) {
	// Buffer to hold the complete UBX message
	uint8_t buffer[6 + message->length + 2];

	// Fill in the UBX header
	buffer[0] = UBX_SYNC_CHAR_1;
	buffer[1] = UBX_SYNC_CHAR_2;
	buffer[2] = message->class_id;
	buffer[3] = message->msg_id;
	buffer[4] = message->length & 0xFF; // Length LSB
	buffer[5] = (message->length >> 8) & 0xFF; // Length MSB

	// Copy the payload if it exists
	if (message->payload && message->length) {
		memcpy(&buffer[6], message->payload, message->length);
	}

	// Calculate and add checksum
	uint8_t ck_a, ck_b;
	CalculateChecksum(&buffer[2], message->length + 4, &ck_a, &ck_b);
	buffer[6 + message->length] = ck_a;
	buffer[7 + message->length] = ck_b;

	// Send the complete message over UART
	HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(gps_uart_, buffer, sizeof(buffer));
	osDelay(wait_ms);
	if(status == HAL_OK){
		return true;
	}else{
		DEBUG_PRINT("GPS Module: Transmit failed: %d\n", status);
		return false;
	}
}

bool ReadUbloxM9nRb::RxUartUbxPollMsg(const uint8_t class_id, const uint8_t msg_id, const uint16_t wait_ms){
	// Receive with DMA but wait
	uint16_t init_check_count = 0;
	while(init_check_count < wait_ms){
		if(ProcessUbloxFrame() && packet_.cls == class_id &&
		   packet_.id == msg_id){
			return true;
		}
		init_check_count++;
		osDelay(1);
	}
	return false;
}

bool ReadUbloxM9nRb::ResetGps(){
	UbloxM9nCfgRst ubx_cfg_rst_msg;

	ubx_cfg_rst_msg.nav_bbr_mask = 0x0001;//Warm Start
	ubx_cfg_rst_msg.reset_mode = 0x01;//Controlled software reset

	// Create a UbxMessage structure for the UBX-CFG-RST message
	UbxMessage ubx_cfg_rst;
	ubx_cfg_rst.class_id = CLASS_CFG;
	ubx_cfg_rst.msg_id = ID_RST;
	ubx_cfg_rst.length = sizeof(UbloxM9nCfgRst);
	ubx_cfg_rst.payload = (uint8_t *)&ubx_cfg_rst_msg;

	DEBUG_PRINT("GPS Module: Sending GPS reset command\n");

	// Send the UBX-CFG-RST message
	bool status  = TxUartUbxPollCmd(&ubx_cfg_rst, txrx_delay_ms_);

	// Reset command may not send ACK, just wait
	osDelay(3000);
	return status;
}

/**
 * @brief Save the current GPS configuration to non-volatile memory
 * @param save_mask: Bitmask indicating which configurations to save
 * @return true if successful, false otherwise
 */
bool ReadUbloxM9nRb::UbxSaveCfg(uint32_t save_mask){
	UbloxM9nCfgCfg ubx_cfg_cfg;

	ubx_cfg_cfg.clear_mask = 0x00000000;
	ubx_cfg_cfg.save_mask = save_mask;
	ubx_cfg_cfg.load_mask = 0x00000000;
	ubx_cfg_cfg.device_mask = 0x03;

	// Create a UbxMessage structure for the UBX-CFG-CFG message
	UbxMessage ubx_cfg_cfg_msg;
	ubx_cfg_cfg_msg.class_id = CLASS_CFG;
	ubx_cfg_cfg_msg.msg_id = ID_CFG;
	ubx_cfg_cfg_msg.length = sizeof(UbloxM9nCfgCfg);
	ubx_cfg_cfg_msg.payload = (uint8_t *)&ubx_cfg_cfg;

	// Send the UBX-CFG-CFG message
	bool status = TxUartUbxPollCmd(&ubx_cfg_cfg_msg, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: UbxSaveCfg - Transmit failure\n");
		return false;
	}

	if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
			packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_CFG){
		DEBUG_PRINT("GPS Module: Successfully saved the config\n");
		return true;
	}else{
		DEBUG_PRINT("GPS Module: Failed to save the config: [Class]: %02x, [Id]%02x, [Pld0]%02x, [Pld1]%02x\n",
				packet_.cls, packet_.id, packet_.payload[1], packet_.payload[1]);
		return false;
	}
}

/**
 * @brief Get the GPS module version information
 * @return true if successful, false otherwise
 */
bool ReadUbloxM9nRb::GetVersion(){
	UbxMessage ubx_mon_ver;

	// Request UBX-MON-VER message
	ubx_mon_ver.class_id = CLASS_MON;
	ubx_mon_ver.msg_id = ID_VER;
	ubx_mon_ver.length = 0;
	ubx_mon_ver.payload = NULL;
	// Send the UBX-MON-VER request
	TxUartUbxPollCmd(&ubx_mon_ver, txrx_delay_ms_);
	if(RxUartUbxPollMsg(CLASS_MON, ID_VER, txrx_delay_ms_)){
		UbloxM9nVer ublox_ver{};
		size_t copy_len = sizeof(ublox_ver);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ublox_ver, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: swVersion: %s\n", ublox_ver.sw_version);
		DEBUG_PRINT("GPS Module: hwVersion: %s\n", ublox_ver.hw_version);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext1);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext2);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext3);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext4);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext5);
		DEBUG_PRINT("GPS Module: %s\n", ublox_ver.ext6);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		return true;
	}
	return false;
}

/**
 * @brief Configure the GPS UART1 port
 * @param baudrate: Desired baud rate for UART1
 * @return true if successful, false otherwise
 */
bool ReadUbloxM9nRb::ConfigGpsUart1(const uint32_t baudrate){
	// Poll UBX-CFG-PRT message for UART1
	UbxMessage ubx_cfg_prt;
	UbloxM9nCfgPrt ubx_cfg_prt_msg {};

	DEBUG_PRINT("GPS Module: Querying Ublox UART1 PRT Config...\n");

	ubx_cfg_prt.class_id = CLASS_CFG;
	ubx_cfg_prt.msg_id = ID_PRT;
	ubx_cfg_prt.length = 1;
	uint8_t port_id = 1;
	ubx_cfg_prt.payload = &port_id;

	// Send the UBX-CFG-PRT poll request
	bool status = TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: ConfigGpsUart1 - Transmit failure - 1\n");
		return false;
	}

	if(RxUartUbxPollMsg(CLASS_CFG, ID_PRT, txrx_delay_ms_)){
		size_t copy_len = sizeof(ubx_cfg_prt_msg);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ubx_cfg_prt_msg, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: Port Id: %d\n", ubx_cfg_prt_msg.port_id);
		DEBUG_PRINT("GPS Module: Tx Ready: 0x%04x\n", ubx_cfg_prt_msg.tx_ready);
		DEBUG_PRINT("GPS Module: Mode: 0x%08" PRIx32 "\n", ubx_cfg_prt_msg.mode);
		DEBUG_PRINT("GPS Module: Baudrate: %lu\n", ubx_cfg_prt_msg.baudrate);
		DEBUG_PRINT("GPS Module: InProtoMask: 0x%04x\n", ubx_cfg_prt_msg.in_proto_mask);
		DEBUG_PRINT("GPS Module: OutProtoMask: 0x%04x\n", ubx_cfg_prt_msg.out_proto_mask);
		DEBUG_PRINT("GPS Module: Flags: 0x%04x\n", ubx_cfg_prt_msg.flags);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
	}else{
		ERROR_PRINT("GPS Module: CFG-PRT Poll request failed - 1\n");
		return false;
	}

	//It also sends an ACK receive it so that cicular buffer is stepped forward
	//No need to check the result as we have safeguards below.
	RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

	// Change the baudrate
	if(ubx_cfg_prt_msg.baudrate != baudrate ||
			ubx_cfg_prt_msg.in_proto_mask != 0x0001 ||
			ubx_cfg_prt_msg.out_proto_mask != 0x0001){
		ubx_cfg_prt_msg.baudrate = baudrate;
		// Change In and Out protocol
		ubx_cfg_prt_msg.in_proto_mask = 0x0001;
		ubx_cfg_prt_msg.out_proto_mask = 0x0001;

		// Create a new UbxMessage to send the updated configuration
		ubx_cfg_prt.length = sizeof(UbloxM9nCfgPrt);
		ubx_cfg_prt.payload = (uint8_t *)&ubx_cfg_prt_msg;

		// Send the UBX-CFG-PRT configuration command with the updated baud rate,
		// And check for ACK ACK
		bool status = TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigGpsUart1 - Transmit failure - 2\n");
			return false;
		}
		SetBaudrate(baudrate);
		// Read back the uart port config
		ubx_cfg_prt.length = 1;
		ubx_cfg_prt.payload = &port_id;
		// Send the UBX-CFG-PRT poll request
		TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);
		if(RxUartUbxPollMsg(CLASS_CFG, ID_PRT, txrx_delay_ms_)){
			UbloxM9nCfgPrt ubx_cfg_prt_msg {};
			size_t copy_len = sizeof(ubx_cfg_prt_msg);
			if (packet_.len < copy_len) {
				// if payload smaller than struct, copy only what's there
				copy_len = packet_.len;
			}
			std::memcpy(&ubx_cfg_prt_msg, packet_.payload, copy_len);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
			DEBUG_PRINT("GPS Module: After Configuring UART1\n");
			DEBUG_PRINT("GPS Module: Port Id: %d\n", ubx_cfg_prt_msg.port_id);
			DEBUG_PRINT("GPS Module: Tx Ready: 0x%04x\n", ubx_cfg_prt_msg.tx_ready);
			DEBUG_PRINT("GPS Module: Mode: 0x%08" PRIx32 "\n", ubx_cfg_prt_msg.mode);
			DEBUG_PRINT("GPS Module: Baudrate: %lu\n", ubx_cfg_prt_msg.baudrate);
			DEBUG_PRINT("GPS Module: InProtoMask: 0x%04x\n", ubx_cfg_prt_msg.in_proto_mask);
			DEBUG_PRINT("GPS Module: OutProtoMask: 0x%04x\n", ubx_cfg_prt_msg.out_proto_mask);
			DEBUG_PRINT("GPS Module: Flags: 0x%04x\n", ubx_cfg_prt_msg.flags);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
		}else{
			ERROR_PRINT("GPS Module: CFG-PRT Poll request failed  after updating the CFG-PRT for UART1\n");
			return false;
		}

		//It also sends an ACK receive it so that cicular buffer is stepped forward
		//No need to check the result as we have safeguards below.
		RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

		if(ubx_cfg_prt_msg.baudrate == baudrate){
			DEBUG_PRINT("GPS Module: Successfully Configured GPS Module To Communicate At %ld\n", baudrate);
			current_baudrate_ = baudrate;
		}else{
			ERROR_PRINT("GPS Module: Failed To Configure GPS Module To Communicate At %ld\n", baudrate);
		}
		// Save the port config
		status = UbxSaveCfg(0x00000001);
		return status;

	}else{
		return true;
	}
}

bool ReadUbloxM9nRb::ConfigPrtProtocol(uint8_t port_id, uint16_t proto_mask){
	/*Configure a port for UBX
	 *
	 */

	// Poll UBX-CFG-PRT message
	UbxMessage ubx_cfg_prt;
	UbloxM9nCfgPrt ubx_cfg_prt_msg {};

	DEBUG_PRINT("GPS Module: Querying Config For Port Id: %d ...\n", port_id);

	ubx_cfg_prt.class_id = CLASS_CFG;
	ubx_cfg_prt.msg_id = ID_PRT;
	ubx_cfg_prt.length = 1;
	ubx_cfg_prt.payload = &port_id;

	// Send the UBX-CFG-PRT poll request
	bool status = TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: ConfigPrtProtocol - Transmit failure - %d\n", port_id);
		return false;
	}
	// UBC-CFG-PRT payload length = 20
	if(RxUartUbxPollMsg(CLASS_CFG, ID_PRT, txrx_delay_ms_)){
		size_t copy_len = sizeof(ubx_cfg_prt_msg);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ubx_cfg_prt_msg, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: Port Id: %d\n", ubx_cfg_prt_msg.port_id);
		DEBUG_PRINT("GPS Module: InProtoMask: 0x%04x\n", ubx_cfg_prt_msg.in_proto_mask);
		DEBUG_PRINT("GPS Module: OutProtoMask: 0x%04x\n", ubx_cfg_prt_msg.out_proto_mask);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
	}else{
		ERROR_PRINT("GPS Module: CFG-PRT Poll request failed - %d\n", port_id);
		return false;
	}

	//It also sends an ACK receive it so that cicular buffer is stepped forward
	//No need to check the result as we have safeguards below.
	RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

	// Change the in and out protocol
	if(ubx_cfg_prt_msg.in_proto_mask != proto_mask ||
			ubx_cfg_prt_msg.out_proto_mask != proto_mask){
		// Change In and Out protocol
		ubx_cfg_prt_msg.in_proto_mask = proto_mask;
		ubx_cfg_prt_msg.out_proto_mask = proto_mask;

		// Create a new UbxMessage to send the updated configuration
		ubx_cfg_prt.length = sizeof(UbloxM9nCfgPrt);
		ubx_cfg_prt.payload = (uint8_t *)&ubx_cfg_prt_msg;

		// Send the UBX-CFG-PRT configuration command with the updated proto mask
		TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);

		if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
					packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_PRT){
			DEBUG_PRINT("GPS Module: Configuration change acknowledged for Port Id: %d\n", port_id);
		}else{
			ERROR_PRINT("GPS Module: Configuration change not acknowledged for Port Id: %d\n", port_id);
			return false;
		}

		// Read back the port config
		ubx_cfg_prt.length = 1;
		ubx_cfg_prt.payload = &port_id;
		// Send the UBX-CFG-PRT poll request
		bool status = TxUartUbxPollCmd(&ubx_cfg_prt, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigPrtProtocol - Transmit failure - %d\n", port_id);
			return false;
		}

		// UBC-CFG-PRT payload length = 20
		if(RxUartUbxPollMsg(CLASS_CFG, ID_PRT, txrx_delay_ms_)){
			size_t copy_len = sizeof(ubx_cfg_prt_msg);
			if (packet_.len < copy_len) {
				// if payload smaller than struct, copy only what's there
				copy_len = packet_.len;
			}
			std::memcpy(&ubx_cfg_prt_msg, packet_.payload, copy_len);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
			DEBUG_PRINT("GPS Module: Port Id: %d\n", ubx_cfg_prt_msg.port_id);
			DEBUG_PRINT("GPS Module: InProtoMask: 0x%04x\n", ubx_cfg_prt_msg.in_proto_mask);
			DEBUG_PRINT("GPS Module: OutProtoMask: 0x%04x\n", ubx_cfg_prt_msg.out_proto_mask);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
		}else{
			ERROR_PRINT("GPS Module: CFG-PRT Poll request failed after port configuration - %d\n", port_id);
			return false;
		}

		//It also sends an ACK, receive it so that cicular buffer is stepped forward
		RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

		// Save the port config
		status = UbxSaveCfg(0x00000001);
		return status;
	}else{
		return true;
	}
}

bool ReadUbloxM9nRb::ConfigAuxPrts(){
	/*Configure all ports for UBX
	 *
	 */

	// Config I2C for UBX only
	bool status = true;
	status &= ConfigPrtProtocol(0, 0x0001);
	if(status){
		DEBUG_PRINT("GPS Module: I2C port configured\n");
	}else{
		ERROR_PRINT("GPS Module: Failed to configure I2C port\n");
	}

	// Config UART2 for UBX only
	status &= ConfigPrtProtocol(2, 0x0001);
	if(status){
		DEBUG_PRINT("GPS Module: UART2 port configured\n");
	}else{
		ERROR_PRINT("GPS Module: Failed to configure UART2 port\n");
	}

	// Config USB for UBX and NMEA only
	status &= ConfigPrtProtocol(3, 0x0003);
	if(status){
		DEBUG_PRINT("GPS Module: USB port configured\n");
	}else{
		ERROR_PRINT("GPS Module: Failed to configure USB port\n");
	}

	// Config SPI for UBX
	status &= ConfigPrtProtocol(4, 0x0001);
	if(status){
		DEBUG_PRINT("GPS Module: SPI port configured\n");
	}else{
		ERROR_PRINT("GPS Module: Failed to configure SPI port\n");
	}

	return status;
}

bool ReadUbloxM9nRb::ConfigNav5(uint8_t dyn_model, uint8_t static_hold_thresh){
	// Poll UBX-CFG-NAV5 message
	UbxMessage ubx_cfg_nav5;
	UbloxM9nCfgNav5 ubx_cfg_nav5_msg {};

	ubx_cfg_nav5.class_id = CLASS_CFG;
	ubx_cfg_nav5.msg_id = ID_NAV5;
	ubx_cfg_nav5.length = 0;
	ubx_cfg_nav5.payload = NULL;

	// Send the UBX-CFG-NAV5 request
	bool status = TxUartUbxPollCmd(&ubx_cfg_nav5, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: ConfigNav5 - Transmit failure - 1\n");
	}

	// UBC-CFG-NAV5 payload length = 36
	if(RxUartUbxPollMsg(CLASS_CFG, ID_NAV5, txrx_delay_ms_)){
		size_t copy_len = sizeof(ubx_cfg_nav5_msg);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ubx_cfg_nav5_msg, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: Current Dynamic Model: %d\n", ubx_cfg_nav5_msg.dyn_model);
		DEBUG_PRINT("GPS Module: Current Fix Mode: %d\n", ubx_cfg_nav5_msg.fix_mode);
		DEBUG_PRINT("GPS Module: Current Static Hold Threshold: %d cm/s\n", ubx_cfg_nav5_msg.static_hold_thresh);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
	}else{
		ERROR_PRINT("GPS Module: CFG-NAV5 Poll request failed\n");
		return false;
	}

	//It also sends an ACK receive it so that cicular buffer is stepped forward
	//No need to check the result as we have safeguards below.
	RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

	if (ubx_cfg_nav5_msg.dyn_model != dyn_model ||
			ubx_cfg_nav5_msg.fix_mode	!= 2 ||
			ubx_cfg_nav5_msg.static_hold_thresh != static_hold_thresh){
		// Apply the configured dynamic model, force 3D-only fixes, and enable
		// static-hold jitter suppression below the configured speed threshold.
		// staticHoldMaxDist is deliberately left as whatever the poll above
		// read back, matching this driver's existing touch-only-what-changes
		// pattern for every other NAV5 field.
		ubx_cfg_nav5_msg.dyn_model = dyn_model;
		ubx_cfg_nav5_msg.fix_mode = 2;
		ubx_cfg_nav5_msg.static_hold_thresh = static_hold_thresh;
		// mask: bit0 (dyn) | bit2 (fixMode) | bit6 (staticHoldMask).
		ubx_cfg_nav5_msg.mask = 0x45;

		// Create a CFG message to send to Ublox
		ubx_cfg_nav5.length = sizeof(UbloxM9nCfgNav5);
		ubx_cfg_nav5.payload = (uint8_t*)&ubx_cfg_nav5_msg;

		// Send the UBX-CFG-PRT configuration command with the updated proto mask
		status = TxUartUbxPollCmd(&ubx_cfg_nav5, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigNav5 - Transmit failure - 2\n");
		}

		if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
					packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_NAV5){
			DEBUG_PRINT("GPS Module: Configuration change acknowledged for NAV5\n");
		}else{
			ERROR_PRINT("GPS Module: Configuration change not acknowledged for NAV5\n");
			return false;
		}

		ubx_cfg_nav5.length = 0;
		ubx_cfg_nav5.payload = NULL;

		// Send the UBX-CFG-NAV5 request
		status = TxUartUbxPollCmd(&ubx_cfg_nav5, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigNav5 - Transmit failure - 3\n");
		}

		// UBC-CFG-NAV5 payload length = 36, read back the NAV5 config
		if(RxUartUbxPollMsg(CLASS_CFG, ID_NAV5, txrx_delay_ms_)){
			size_t copy_len = sizeof(ubx_cfg_nav5_msg);
			if (packet_.len < copy_len) {
				// if payload smaller than struct, copy only what's there
				copy_len = packet_.len;
			}
			std::memcpy(&ubx_cfg_nav5_msg, packet_.payload, copy_len);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
			DEBUG_PRINT("GPS Module: Updated Dynamic Model: %d\n", ubx_cfg_nav5_msg.dyn_model);
			DEBUG_PRINT("GPS Module: Updated Fix Mode: %d\n", ubx_cfg_nav5_msg.fix_mode);
			DEBUG_PRINT("GPS Module: Updated Static Hold Threshold: %d cm/s\n", ubx_cfg_nav5_msg.static_hold_thresh);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
		}else{
			ERROR_PRINT("GPS Module: CFG-NAV5 Poll request failed after updating the CFG-NAV5\n");
			return false;
		}

		//It also sends an ACK receive it so that cicular buffer is stepped forward
		RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

		//Save the NAV5 config
		status = UbxSaveCfg(0x00000008);
		return status;
	}else{
		return true;
	}
}

bool ReadUbloxM9nRb::ConfigSbas(){
	// Poll UBX-CFG-SBAS message
	UbxMessage ubx_cfg_sbas;
	UbloxM9nCfgSbas ubx_cfg_sbas_msg {};

	ubx_cfg_sbas.class_id = CLASS_CFG;
	ubx_cfg_sbas.msg_id = ID_SBAS;
	ubx_cfg_sbas.length = 0;
	ubx_cfg_sbas.payload = NULL;

	// Send the UBX-CFG-SBAS request
	bool status = TxUartUbxPollCmd(&ubx_cfg_sbas, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: ConfigSbas - Transmit failure - 1\n");
		return false;
	}

	// UBX-CFG-SBAS payload length = 8
	if(RxUartUbxPollMsg(CLASS_CFG, ID_SBAS, txrx_delay_ms_)){
		size_t copy_len = sizeof(ubx_cfg_sbas_msg);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ubx_cfg_sbas_msg, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: Current SBAS Mode: 0x%02x, Usage: 0x%02x, MaxSBAS: %d\n",
				ubx_cfg_sbas_msg.mode, ubx_cfg_sbas_msg.usage, ubx_cfg_sbas_msg.max_sbas);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
	}else{
		ERROR_PRINT("GPS Module: CFG-SBAS Poll request failed\n");
		return false;
	}

	//It also sends an ACK receive it so that cicular buffer is stepped forward
	//No need to check the result as we have safeguards below.
	RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

	constexpr uint8_t kSbasEnableMode = 0x01; // enabled, not test mode
	// range + diffCorr only. Deliberately NOT setting bit2 (integrity, 0x04):
	// that flag restricts the nav solution to only GPS satellites for which
	// SBAS currently has valid integrity data, which is a moving/incomplete
	// subset in practice. On a airborne vehicle whose attitude constantly
	// changes, the antenna's view of the one low-elevation geostationary SBAS
	// satellite drops out far more than on a stationary/ground receiver,
	// which starves integrity coverage and causes the receiver to shed most
	// GPS satellites from the solution -- a documented u-blox failure mode
	// (their own support portal has reports titled "'Apply integrity
	// information' in UBX-CFG-SBAS causes FIX losses" and "Behavior of
	// 'Strict SBAS Integrity Mode' when no SBAS satellites are visible").
	// Range+diffCorr alone still gets the accuracy benefit without this risk.
	constexpr uint8_t kSbasUsage = 0x03;
	constexpr uint8_t kSbasMaxChannels = 3;   // track up to 3 SBAS satellites

	if (ubx_cfg_sbas_msg.mode != kSbasEnableMode ||
			ubx_cfg_sbas_msg.usage != kSbasUsage ||
			ubx_cfg_sbas_msg.max_sbas != kSbasMaxChannels){
		ubx_cfg_sbas_msg.mode = kSbasEnableMode;
		ubx_cfg_sbas_msg.usage = kSbasUsage;
		ubx_cfg_sbas_msg.max_sbas = kSbasMaxChannels;
		// All-zero PRN mask lets the receiver auto-search every supported
		// SBAS system (WAAS/EGNOS/MSAS/...) instead of requiring one fixed,
		// region-specific PRN list baked into firmware.
		ubx_cfg_sbas_msg.scan_mode2 = 0;
		ubx_cfg_sbas_msg.scan_mode1 = 0;

		ubx_cfg_sbas.length = sizeof(UbloxM9nCfgSbas);
		ubx_cfg_sbas.payload = (uint8_t*)&ubx_cfg_sbas_msg;

		status = TxUartUbxPollCmd(&ubx_cfg_sbas, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigSbas - Transmit failure - 2\n");
			return false;
		}

		if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
					packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_SBAS){
			DEBUG_PRINT("GPS Module: Configuration change acknowledged for SBAS\n");
		}else{
			ERROR_PRINT("GPS Module: Configuration change not acknowledged for SBAS\n");
			return false;
		}

		status = UbxSaveCfg(0x00000008);
		return status;
	}else{
		return true;
	}
}

bool ReadUbloxM9nRb::EnableNavPvtMsg(){
	UbxMessage ubx_cfg_msg;
	UbloxM9nCfgMsg ubx_cfg_msg_msg = {0};
	ubx_cfg_msg_msg.msg_class = CLASS_NAV; // NAV class
	ubx_cfg_msg_msg.msg_id = ID_PVT;    // PVT message
	ubx_cfg_msg_msg.rate[0] = 0;      // I2C rate
	ubx_cfg_msg_msg.rate[1] = 1;      // UART1 rate
	ubx_cfg_msg_msg.rate[2] = 0;      // UART2 rate
	ubx_cfg_msg_msg.rate[3] = 0;      // USB rate
	ubx_cfg_msg_msg.rate[4] = 0;      // SPI rate
	ubx_cfg_msg_msg.rate[5] = 0;      // Reserved

	// Set up the UBX message
	ubx_cfg_msg.class_id = CLASS_CFG;           // CFG class
	ubx_cfg_msg.msg_id = ID_MSG;              // MSG ID
	ubx_cfg_msg.length = sizeof(UbloxM9nCfgMsg);
	ubx_cfg_msg.payload = (uint8_t*)&ubx_cfg_msg_msg;

	// Send the command
	bool status = TxUartUbxPollCmd(&ubx_cfg_msg, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: EnableNavPvtMsg - Transmit failure\n");
	}

	if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
			packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_MSG){
		DEBUG_PRINT("GPS Module: Successfully started NAV-PVT message for UART1\n");
		return true;
	}else{
		DEBUG_PRINT("GPS Module: Failed to start NAV-PVT message for UART1: [Class]: %02x, [Id]%02x, [Pld0]%02x, [Pld1]%02x\n",
				packet_.cls, packet_.id, packet_.payload[0], packet_.payload[1]);
	}

	return false;
}

bool ReadUbloxM9nRb::DisableNavPvtMsg(){
	UbxMessage ubx_cfg_msg;
	UbloxM9nCfgMsg ubx_cfg_msg_msg = {0};
	ubx_cfg_msg_msg.msg_class = CLASS_NAV; // NAV class
	ubx_cfg_msg_msg.msg_id = ID_PVT;    // PVT message
	ubx_cfg_msg_msg.rate[0] = 0;      // I2C rate
	ubx_cfg_msg_msg.rate[1] = 0;      // UART1 rate
	ubx_cfg_msg_msg.rate[2] = 0;      // UART2 rate
	ubx_cfg_msg_msg.rate[3] = 0;      // USB rate
	ubx_cfg_msg_msg.rate[4] = 0;      // SPI rate
	ubx_cfg_msg_msg.rate[5] = 0;      // Reserved

	// Set up the UBX message
	ubx_cfg_msg.class_id = CLASS_CFG;           // CFG class
	ubx_cfg_msg.msg_id = ID_MSG;              // MSG ID
	ubx_cfg_msg.length = sizeof(UbloxM9nCfgMsg);
	ubx_cfg_msg.payload = (uint8_t*)&ubx_cfg_msg_msg;

	// Send the command
	bool status = TxUartUbxPollCmd(&ubx_cfg_msg, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: DisableNavPvtMsg - Transmit failure\n");
	}

	if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
			packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_MSG){
		DEBUG_PRINT("GPS Module: Successfully stopped NAV-PVT message for UART1\n");
		return true;
	}else{
		DEBUG_PRINT("GPS Module: Failed to stop NAV-PVT message for UART1: %02x, %02x\n", packet_.cls, packet_.id);
	}

	return false;
}

void ReadUbloxM9nRb::GetCurrentBaudrate(){
	uint8_t num_baurates = sizeof(supported_baudrates_) / sizeof(supported_baudrates_[0]);
	for (size_t idx = 0; idx < num_baurates; idx++){
		SetBaudrate(supported_baudrates_[idx]);
		bool status = GetVersion();
		if(status){
			DEBUG_PRINT("GPS Module: Current GPS Module Baudrate is: %ld, Resetting GPS\n", supported_baudrates_[idx]);
			current_baudrate_ = supported_baudrates_[idx];
			status = ResetGps();
			return;
		}
	}

	DEBUG_PRINT("GPS Module: Unable to find the correct baudrate\n");
}

bool ReadUbloxM9nRb::InitGps(uint32_t baudrate, uint16_t time_bw_samples_ms, uint8_t nav_rate){
	bool status;
	GetCurrentBaudrate();
	status = DisableNavPvtMsg();
	if(status){
		//Config the desired baudrate on GPS module
		status &= ConfigGpsUart1(baudrate);
		//Set gps_uart_ baudrate
		SetBaudrate(baudrate);
		current_baudrate_ = baudrate;
		status &= ConfigAuxPrts();

		// GPS_DYN_MODEL/GPS_HOLD_THR are OnReboot QGC parameters: read the
		// currently configured value exactly once, here at GPS module boot.
		// A QGC edit stages a new configured value immediately but only
		// takes effect the next time this function runs (next GPS boot).
		// The generated compile-time default is the safe fallback if the
		// store read ever fails (e.g. an unexpected type/range mismatch).
		std::int32_t dyn_model_param =
				parameters::generated::kGpsParameterDefaults.dyn_model;
		std::int32_t static_hold_param = parameters::generated::
				kGpsParameterDefaults.static_hold_thresh_cmps;
		auto& param_store = parameters::ParameterStore::Instance();
		if (!param_store.ReadInt32(parameters::generated::ParameterId::GpsDynModel,
				&dyn_model_param)) {
			DEBUG_PRINT("GPS Module: Falling back to default GPS_DYN_MODEL\n");
		}
		if (!param_store.ReadInt32(parameters::generated::ParameterId::GpsHoldThr,
				&static_hold_param)) {
			DEBUG_PRINT("GPS Module: Falling back to default GPS_HOLD_THR\n");
		}

		status &= ConfigNav5(static_cast<uint8_t>(dyn_model_param),
				static_cast<uint8_t>(static_hold_param));
		status &= ConfigSbas();
		status &= ConfigGpsMeasRate(time_bw_samples_ms, nav_rate);
		FlushUartDataRegister();
		status &= EnableNavPvtMsg();
	}

	return status;
}

bool ReadUbloxM9nRb::ConfigGpsMeasRate(uint16_t time_bw_samples_ms, uint8_t nav_rate){
	/* Configures GPS measurement rate and nav rate
	 *
	 */
	UbxMessage ubx_cfg_rate;
	UbloxM9nCfgRate ubx_cfg_rate_msg {};

	ubx_cfg_rate.class_id = CLASS_CFG;
	ubx_cfg_rate.msg_id = ID_RATE;
	ubx_cfg_rate.length = 0;
	ubx_cfg_rate.payload = NULL;

	// Send the UBX-CFG-RATE request
	bool status = TxUartUbxPollCmd(&ubx_cfg_rate, txrx_delay_ms_);
	if(!status){
		DEBUG_PRINT("GPS Module: ConfigGpsMeasRate - Transmit failure - 1\n");
		return false;
	}
	// UBC-CFG-RATE payload length = 6
	if(RxUartUbxPollMsg(CLASS_CFG, ID_RATE, txrx_delay_ms_)){
		size_t copy_len = sizeof(ubx_cfg_rate_msg);
		if (packet_.len < copy_len) {
			// if payload smaller than struct, copy only what's there
			copy_len = packet_.len;
		}
		std::memcpy(&ubx_cfg_rate_msg, packet_.payload, copy_len);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
		DEBUG_PRINT("GPS Module: Current Time Bw Samples [ms]: %d\n", ubx_cfg_rate_msg.meas_rate);
		DEBUG_PRINT("GPS Module: Current Nav Rate: %d\n", ubx_cfg_rate_msg.nav_rate);
		DEBUG_PRINT("GPS Module: ***********************************************\n");
	}else{
		ERROR_PRINT("GPS Module: CFG-RATE Poll request failed - 1\n");
		return false;
	}

	//It also sends an ACK receive it so that cicular buffer is stepped forward
	//No need to check the result as we have safeguards below.
	RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

	if (ubx_cfg_rate_msg.meas_rate != time_bw_samples_ms ||
			ubx_cfg_rate_msg.nav_rate	!= nav_rate){
		// Change meas and nav rate to desired values
		ubx_cfg_rate_msg.meas_rate = time_bw_samples_ms;
		ubx_cfg_rate_msg.nav_rate = nav_rate;

		// Create a CFG message to send to Ublox
		ubx_cfg_rate.length = sizeof(UbloxM9nCfgRate);
		ubx_cfg_rate.payload = (uint8_t*)&ubx_cfg_rate_msg;

		// Send the UBX-CFG-RATE configuration command
		status = TxUartUbxPollCmd(&ubx_cfg_rate, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigGpsMeasRate - Transmit failure - 2\n");
			return false;
		}

		if(RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_) &&
							packet_.payload[0] == CLASS_CFG && packet_.payload[1] == ID_RATE){
			DEBUG_PRINT("GPS Module: Configuration change acknowledged for RATE\n");
		}else{
			ERROR_PRINT("GPS Module: Configuration change not acknowledged for RATE\n");
			return false;
		}

		ubx_cfg_rate.length = 0;
		ubx_cfg_rate.payload = NULL;

		// Send the UBX-CFG-RATE request
		status = TxUartUbxPollCmd(&ubx_cfg_rate, txrx_delay_ms_);
		if(!status){
			DEBUG_PRINT("GPS Module: ConfigGpsMeasRate - Transmit failure - 2\n");
			return false;
		}

		// UBC-CFG-NAV5 payload length = 6, read back the NAV5 config
		if(RxUartUbxPollMsg(CLASS_CFG, ID_RATE, txrx_delay_ms_)){
			size_t copy_len = sizeof(ubx_cfg_rate_msg);
			if (packet_.len < copy_len) {
				// if payload smaller than struct, copy only what's there
				copy_len = packet_.len;
			}
			std::memcpy(&ubx_cfg_rate_msg, packet_.payload, copy_len);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
			DEBUG_PRINT("GPS Module: Updated Time Bw Samples [ms]: %d\n", ubx_cfg_rate_msg.meas_rate);
			DEBUG_PRINT("GPS Module: Updated Nav Rate: %d\n", ubx_cfg_rate_msg.nav_rate);
			DEBUG_PRINT("GPS Module: ***********************************************\n");
		}else{
			ERROR_PRINT("GPS Module: CFG-RATE Poll request failed after updating the CFG-RATE\n");
			return false;
		}

		//It also sends an ACK receive it so that cicular buffer is stepped forward
		RxUartUbxPollMsg(CLASS_ACK, ID_ACK, txrx_delay_ms_);

		// Save the Rate config
		status = UbxSaveCfg(0x00000008);
		return status;

	}else{
		return true;
	}
}

bool ReadUbloxM9nRb::ProcessUbloxFrame(){
	size_t current_write_index = MAX_BUFF_SIZE - __HAL_DMA_GET_COUNTER(gps_uart_->hdmarx);
	size_t available_bytes = 0;
	if (current_write_index >= last_read_index_) {
		available_bytes = current_write_index - last_read_index_;
	} else {
		available_bytes = MAX_BUFF_SIZE - last_read_index_ + current_write_index;
	}

	if(available_bytes == 0) return false;
	size_t bytes_processed = 0;
	while (bytes_processed < available_bytes) {
		size_t buffer_index = (last_read_index_ + bytes_processed) % MAX_BUFF_SIZE;
		if(ParseUbx(rx_buffer_[buffer_index])){
			last_read_index_ = (last_read_index_ + bytes_processed + 1) % MAX_BUFF_SIZE;
			return true;
		}
		++bytes_processed;
	}
	last_read_index_ = (last_read_index_ + bytes_processed) % MAX_BUFF_SIZE;
	return false;
}

bool ReadUbloxM9nRb::ParseUbx(uint8_t byte)
{
    switch(parser_state_)
    {
		case WAIT_SYNC1:
			if (byte == UBX_SYNC_CHAR_1) parser_state_ = WAIT_SYNC2;
			break;

		case WAIT_SYNC2:
			if (byte == UBX_SYNC_CHAR_2) parser_state_ = WAIT_CLASS;
			else parser_state_ = WAIT_SYNC1;
			break;

		case WAIT_CLASS:
			packet_.cls = byte;
			ck_a_ = ck_b_ = 0;
			UpdateChecksum(byte);
			parser_state_ = WAIT_ID;
			break;

		case WAIT_ID:
			packet_.id = byte;
			UpdateChecksum(byte);
			parser_state_ = WAIT_LEN1;
			break;

		case WAIT_LEN1:
			packet_.len = byte;
			UpdateChecksum(byte);
			parser_state_ = WAIT_LEN2;
			break;

		case WAIT_LEN2:
			packet_.len |= (uint16_t)byte << 8;
			UpdateChecksum(byte);
			if (packet_.len > UBX_MAX_PAYLOAD) {
				parser_state_ = WAIT_SYNC1;
			} else if (packet_.len == 0) {
				parser_state_ = WAIT_CK_A;
			} else {
				payload_idx_ = 0;
				parser_state_ = WAIT_PAYLOAD;
			}
			break;

		case WAIT_PAYLOAD:
			packet_.payload[payload_idx_++] = byte;
			UpdateChecksum(byte);
			if (payload_idx_ >= packet_.len) {
				parser_state_ = WAIT_CK_A;
			}
			break;

		case WAIT_CK_A:
			if (byte == ck_a_) {
				parser_state_ = WAIT_CK_B;
			} else {
				if (byte == UBX_SYNC_CHAR_1){
					parser_state_ = WAIT_SYNC2;
				}else{
					parser_state_ = WAIT_SYNC1;
				}
			}
			break;

		case WAIT_CK_B:
			if (byte == ck_b_) {
				parser_state_ = WAIT_SYNC1;
				return true; // complete valid packet
			} else {
				parser_state_ = WAIT_SYNC1;
			}
			break;

		default:
			parser_state_ = WAIT_SYNC1;
			break;
    }

    return false;
}

void ReadUbloxM9nRb::Run() {
	Publisher<GpsData> ubloxm9n_pub(TopicID::UBLOXM9N);
	osDelay(100);
	// 40 ms measurement period (25 Hz nav solution): shortens the inter-update
	// gap the EKF dead-reckons across between GPS corrections, versus the
	// previous 50 ms/20 Hz. ConfigGpsMeasRate() reads back the applied rate,
	// so an unsupported value for this module's enabled constellation set is
	// self-diagnosing via the existing DEBUG_PRINT readback, not a hazard.
	bool gps_status = InitGps(921600U, 40, 1);
	(void)gps_status;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);

	osDelay(250);
	// Initialize the periodic schedule after the startup delay.
	xLastWakeTime = xTaskGetTickCount();
	// A failed/absent InitGps() above starts this at the current tick too, so
	// the watchdog below fires on schedule instead of assuming success.
	last_valid_frame_tick_ = xLastWakeTime;
	UbloxM9nNavPvt nav_pvt_data_{};
//	int blink_counter = 0;
    /* Infinite loop */
    for (;;) {
//    	if (++blink_counter >= 20) {
//			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 1296 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("GPS Module: Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//			used, highWaterMark * sizeof(StackType_t), 1296);
//    	}

    	// Recovery runs here, in the task's own context, never in the ISR.
    	if (uart_error_pending_.exchange(false, std::memory_order_acq_rel)) {
    		DEBUG_PRINT("GPS Module: UART error observed, restarting DMA reception\n");
    		RecoverUartDma();
    	}

    	if(ProcessUbloxFrame() && packet_.cls == CLASS_NAV && packet_.id == ID_PVT){
    		last_valid_frame_tick_ = xTaskGetTickCount();
    		memcpy(&nav_pvt_data_, packet_.payload, sizeof(UbloxM9nNavPvt));
    		gps_data_.i_tow = nav_pvt_data_.i_tow;
			gps_data_.valid = nav_pvt_data_.valid;
			gps_data_.fix_type = nav_pvt_data_.fix_type;
			gps_data_.flags = nav_pvt_data_.flags;
			gps_data_.latitude_rad = nav_pvt_data_.lat * 1e-7 * DEG2RAD;
			gps_data_.longitude_rad = nav_pvt_data_.lon * 1e-7 * DEG2RAD;
			gps_data_.altitude_m = nav_pvt_data_.height * 1e-3;
			gps_data_.vn_mps = nav_pvt_data_.vel_n * 1e-3;
			gps_data_.ve_mps = nav_pvt_data_.vel_e * 1e-3;
			gps_data_.vd_mps = nav_pvt_data_.vel_d * 1e-3;
			gps_data_.num_sv = nav_pvt_data_.num_sv;
			gps_data_.g_speed_mps = nav_pvt_data_.g_speed * 1e-3f;
			gps_data_.cog_deg = nav_pvt_data_.heading * 1e-5f;
			gps_data_.hacc_m = nav_pvt_data_.h_acc * 1e-3f;
			gps_data_.vacc_m = nav_pvt_data_.v_acc * 1e-3f;
			gps_data_.s_acc_mps = nav_pvt_data_.s_acc * 1e-3f;
			gps_data_.heading_acc_deg = nav_pvt_data_.heading_acc * 1e-5f;
			gps_data_.p_dop = nav_pvt_data_.p_dop;
			gps_data_.head_veh_deg = nav_pvt_data_.head_veh * 1e-5;
			gps_data_.checksum_valid = true;

    		ubloxm9n_pub.publish(gps_data_);
//    		DEBUG_PRINT("GPS Module: iTOW: %lu, fix_type: %d, lat_rad: %g, lon_rad: %g, alt_m: %g\n",
//    					gps_data_.i_tow, gps_data_.fix_type, gps_data_.latitude_rad, gps_data_.longitude_rad, gps_data_.altitude_m);
    		new_nav_pvt_frame_ = false;
    	}

    	// No valid NAV-PVT for GPS_STALE_TIMEOUT_MS: either the module was
    	// never successfully initialized (InitGps()'s result at boot is
    	// otherwise silently discarded), or it went away later (unplugged, or
    	// wedged past what UART error recovery alone fixes -- EnableNavPvtMsg()
    	// is never persisted to the module's NVM, so a power-cycle reliably
    	// stops NAV-PVT output even if the link itself recovers). Re-running
    	// the full sequence blocks only this task; nothing else depends on it
    	// synchronously, so a multi-second retry here is real-time safe.
    	if ((xTaskGetTickCount() - last_valid_frame_tick_) >
    			pdMS_TO_TICKS(GPS_STALE_TIMEOUT_MS)) {
    		DEBUG_PRINT("GPS Module: No valid NAV-PVT for %lu ms, reinitializing\n",
    				(unsigned long)GPS_STALE_TIMEOUT_MS);
    		const bool reinit_status = InitGps(921600U, 40, 1);
    		(void)reinit_status;
    		// Restart the countdown regardless of outcome: a fresh window to
    		// see whether data resumes, rather than retrying every 25 ms tick
    		// while the link stays down.
    		last_valid_frame_tick_ = xTaskGetTickCount();
    	}

    	// Wait until the next cycle
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}


