/*
 * sd_utils.cc
 *
 *  Created on: Apr 12, 2025
 *      Author: tanmay
 */

#include "sd_utils.h"
#include "data_buffer.h"
#include "usb_console/usb_console.h"

SdUtils::SdUtils()
: TaskBase("SdWriteTask", 1820, osPriorityBelowNormal){
}

bool SdUtils::SdInit(){
  FRESULT f_res;
  // Mount the SD card
  f_res = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
  if (f_res != FR_OK) {
    ERROR_PRINT("SD Card mount failed (f_mount res=%d)\n", f_res);
    return false;
  }

  DEBUG_PRINT("SD Card mounted successfully\n");

  // Open file for write: always create a new file (overwrites if exists)
  f_res = f_open(&SDFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
  if (f_res != FR_OK) {
    ERROR_PRINT("Failed to open %s (res=%d)\n", file_name, f_res);
    return false;
  }

  DEBUG_PRINT("%s created\n", file_name);
  file_open_ = true;

  // Set active file pointer for logging
  file_ = &SDFile;
  f_sync(file_);
  return true;
}

void SdUtils::Run() {
	osDelay(500);
	SdInit();
	osDelay(500);
	UINT bytes_written;
//	int blink_counter = 0;
	CardState state_ = CardState::IDLEREADY;
	UsbCommand usb_cmd_{UsbCommand::NONE};
	while (true) {
//		if (++blink_counter >= 2) {
//			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 1820 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//			used, highWaterMark * sizeof(StackType_t), 1820);
//		}
		if(UsbConsole::Instance().HasNewCommand()){
			DIR dir;
			FILINFO fno;
			FRESULT res;
			usb_cmd_ = UsbConsole::Instance().GetLatestCommand();
			switch(usb_cmd_){
				case UsbCommand::STOP:
					state_ = CardState::IDLESTOP;
					break;

				case UsbCommand::START:
					state_ = CardState::IDLESTART;
					break;

				case UsbCommand::LIST:
					if(!file_open_){
						res = f_opendir(&dir, "/");  // Only able to list files in root dir
						if (res != FR_OK) {
							DEBUG_PRINT("Failed to open root dir\n");
						}
						while (1) {
							res = f_readdir(&dir, &fno);  // Read next entry
							if (res != FR_OK || fno.fname[0] == 0) break;  // Error or end

							if (fno.fattrib & AM_DIR) {
								//We don't handle directories
								__NOP();
							} else {
								// It's a file
								DEBUG_PRINT("[FILE] %-20s %lu B\r\n", fno.fname, (uint32_t)fno.fsize);
							}

							osDelay(1);  // Prevent USB CDC buffer overrun
						}
					}else{
						DEBUG_PRINT("SD Card write in progress, issue a STOP command first\n");
					}
					break;

				case UsbCommand::COPY:
					if(!file_open_){
						res = f_open(file_, file_name, FA_READ);
						if (res != FR_OK) {
							DEBUG_PRINT("Failed to open file: %s\r\n", file_name);
							return;
						}

						DEBUG_PRINT("== Contents of %s ==\r\n", file_name);
						UINT bytes_read;
						char buffer[128];  // Small buffer to fit USB CDC comfortably

						while (1) {
							res = f_read(file_, buffer, sizeof(buffer) - 1, &bytes_read);
							if (res != FR_OK || bytes_read == 0) break;

							buffer[bytes_read] = '\0';  // Null-terminate to be safe for printing
							DEBUG_PRINT("%s", buffer);  // Use your task-safe print

							osDelay(1);  // Let USB catch up
						}

						f_close(file_);
						DEBUG_PRINT("\r\n== End of file ==\r\n");
					}
					else{
						DEBUG_PRINT("SD Card write in progress, issue a STOP command first\n");
					}
					break;

				default:
					break;
			}
		}

		switch(state_){
			case CardState::IDLESTART:
				if(file_ != NULL){
					f_open(file_, file_name, FA_OPEN_APPEND | FA_WRITE);
					f_sync(file_);
					DEBUG_PRINT("Reopened the log file to append logs\n");
				}else{
					f_open(&SDFile, file_name, FA_CREATE_ALWAYS | FA_WRITE);
					DEBUG_PRINT("Creating the log file while restarting the log file\n");
					file_ = &SDFile;
					f_sync(file_);
				}
				file_open_ = true;
				state_ = CardState::IDLEREADY;
				break;

			case CardState::IDLEREADY:
				if(DataBuffer::buffer_full_){
					state_ = CardState::WRITING;
				}
				break;

			case CardState::WRITING:
				f_write(file_, DataBuffer::buffers_[DataBuffer::flush_buffer_index_], DataBuffer::buffer_offsets_[DataBuffer::flush_buffer_index_], &bytes_written);
				f_sync(file_);
//				DEBUG_PRINT("WRITE COMPLETE from buffer %u, %u bytes written\n", DataBuffer::flush_buffer_index_, DataBuffer::buffer_offsets_[DataBuffer::flush_buffer_index_]);
				DataBuffer::buffer_offsets_[DataBuffer::flush_buffer_index_] = 0;
				state_ = CardState::IDLEREADY;
				xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
				DataBuffer::buffer_full_ = false;
				xSemaphoreGive(DataBuffer::mutex_);
				break;

			case CardState::IDLESTOP:
				if(file_open_){
					f_close(file_);
					file_open_ = false;
					DEBUG_PRINT("Closed file for logging\n");
				}
		}
		// loop at desired rate
		osDelay(INTERVAL_MS);
	}
}

// Static instance for self-registration
SdUtils sdutils_task_instance_;
