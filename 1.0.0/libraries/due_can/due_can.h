/*
  Copyright (c) 2013 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* Reference for how this struct is defined:
See file can.h from the libsam files
*/

#ifndef _CAN_LIBRARY_
#define _CAN_LIBRARY_

#include "sn65hvd234.h"

#define SIZE_RX_BUFFER	32 //RX incoming ring buffer is this big
#define SIZE_TX_BUFFER	16 //TX ring buffer is this big

//This is architecture specific. DO NOT USE THIS UNION ON ANYTHING OTHER THAN THE CORTEX M3 / Arduino Due
//UNLESS YOU DOUBLE CHECK THINGS!
typedef union {
    uint64_t value;
	struct {
		uint32_t low;
		uint32_t high;
	};
	struct {
        uint16_t s0;
		uint16_t s1;
		uint16_t s2;
		uint16_t s3;
    };
	uint8_t bytes[8];
} BytesUnion;

typedef struct
{
	uint32_t id;		// EID if ide set, SID otherwise
	uint32_t fid;		// family ID
	uint8_t rtr;		// Remote Transmission Request
	uint8_t priority;	// Priority but only important for TX frames and then only for special uses.
	uint8_t extended;	// Extended ID flag
	uint8_t length;		// Number of data bytes
	BytesUnion data;	// 64 bits - lots of ways to access it.
} CAN_FRAME;

class CANRaw
{
  protected:
    /* CAN peripheral, set by constructor */
    Can* m_pCan ;

	int numTXBoxes; //There are 8 mailboxes, anything not TX will be set RX

	volatile CAN_FRAME rx_frame_buff[SIZE_RX_BUFFER];
	volatile CAN_FRAME tx_frame_buff[SIZE_TX_BUFFER];

	volatile uint16_t rx_buffer_head, rx_buffer_tail;
	volatile uint16_t tx_buffer_head, tx_buffer_tail;
	void mailbox_int_handler(uint8_t mb, uint32_t ul_status);

  private:

  public:
    /* CAN Transceiver */
    SSN65HVD234 Transceiver;

    // Constructor
    CANRaw( Can* pCan);

    /**
 * \defgroup sam_driver_can_group Controller Area Network (CAN) Driver
 *
 * See \ref sam_can_quickstart.
 *
 * \par Purpose
 *
 * The CAN controller provides all the features required to implement
 * the serial communication protocol CAN defined by Robert Bosch GmbH,
 * the CAN specification. This is a driver for configuration, enabling,
 * disabling and use of the CAN peripheral.
 *
 * @{
 */

int setRXFilter(uint32_t id, uint32_t mask, bool extended);
int setRXFilter(uint8_t mailbox, uint32_t id, uint32_t mask, bool extended);
void setNumTXBoxes(int txboxes);
int findFreeRXMailbox();
uint8_t mailbox_get_mode(uint8_t uc_index);
uint32_t mailbox_get_id(uint8_t uc_index);
uint32_t getMailboxIer(int8_t mailbox);
uint32_t set_baudrate(uint32_t ul_baudrate);
uint32_t init(uint32_t ul_baudrate);
void enable();
void disable();
void disable_low_power_mode();
void enable_low_power_mode();
void disable_autobaud_listen_mode();
void enable_autobaud_listen_mode();
void disable_overload_frame();
void enable_overload_frame();
void set_timestamp_capture_point(uint32_t ul_flag);
void disable_time_triggered_mode();
void enable_time_triggered_mode();
void disable_timer_freeze();
void enable_timer_freeze();
void disable_tx_repeat();
void enable_tx_repeat();
void set_rx_sync_stage(uint32_t ul_stage);
void enable_interrupt(uint32_t dw_mask);
void disable_interrupt(uint32_t dw_mask);
uint32_t get_interrupt_mask();
uint32_t get_status();
uint32_t get_internal_timer_value();
uint32_t get_timestamp_value();
uint8_t get_tx_error_cnt();
uint8_t get_rx_error_cnt();
void reset_internal_timer();
void global_send_transfer_cmd(uint8_t uc_mask);
void global_send_abort_cmd(uint8_t uc_mask);
void mailbox_set_timemark(uint8_t uc_index, uint16_t us_cnt);
uint32_t mailbox_get_status(uint8_t uc_index);
void mailbox_send_transfer_cmd(uint8_t uc_index);
void mailbox_send_abort_cmd(uint8_t uc_index);
void mailbox_init(uint8_t uc_index);
uint32_t mailbox_read(uint8_t uc_index, volatile CAN_FRAME *rxframe);
uint32_t mailbox_tx_frame(uint8_t uc_index);
void mailbox_set_id(uint8_t uc_index, uint32_t id, bool extended);
void mailbox_set_priority(uint8_t uc_index, uint8_t pri);
void mailbox_set_accept_mask(uint8_t uc_index, uint32_t mask, bool ext);
void mailbox_set_mode(uint8_t uc_index, uint8_t mode);
void mailbox_set_databyte(uint8_t uc_index, uint8_t bytepos, uint8_t val);
void mailbox_set_datalen(uint8_t uc_index, uint8_t dlen);
void mailbox_set_datal(uint8_t uc_index, uint32_t val);
void mailbox_set_datah(uint8_t uc_index, uint32_t val);
void sendFrame(CAN_FRAME& txFrame);

void reset_all_mailbox();
void interruptHandler();
bool rx_avail();
uint32_t get_rx_buff(CAN_FRAME &);
};

#ifdef PINS_CAN0
extern CANRaw CAN;
#endif

#ifdef PINS_CAN1
extern CANRaw CAN2;
#endif

#endif // _CAN_LIBRARY_
