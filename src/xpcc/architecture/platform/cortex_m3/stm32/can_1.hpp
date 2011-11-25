// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_STM32__CAN1_HPP
#define XPCC_STM32__CAN1_HPP

#include <xpcc/driver/connectivity/can/message.hpp>

#include "device.h"
#include "can_filter.hpp"

namespace xpcc
{
	namespace stm32
	{
		/**
		 * @brief		Basic Extended CAN1
		 * 
		 * The Basic Extended CAN peripheral, named bxCAN, interfaces the CAN
		 * network. It supports the CAN protocols version 2.0A and B. It has
		 * been designed to manage a high number of incoming messages
		 * efficiently with a minimum CPU load.
		 * 
		 * <h2>Filter</h2>
		 * 
		 * For connectivity line devices there are 28 filter banks,
		 * i=0 .. 27, in other devices there are 14 filter banks i = 0 ..13.
		 * 
		 * The 28 filter banks for the connectivity line devices are
		 * shared between CAN1 and CAN2.
		 * 
		 * <h2>Configuration</h2>
		 * 
		 * \code
		 * STM32_CAN1_TX_BUFFER_SIZE
		 * STM32_CAN1_RX_BUFFER_SIZE
		 * \endcode
		 * 
		 * @author		Fabian Greif <fabian.greif@rwth-aachen.de>
		 * @ingroup		stm32
		 */
		class Can1
		{
		public:
			enum Remap
			{
				REMAP_PA11_PA12 = AFIO_MAPR_CAN_REMAP_REMAP1,	///< CANRX mapped to PA11, CANTX mapped to PA12
				REMAP_PB8_PB9 = AFIO_MAPR_CAN_REMAP_REMAP2,		///< CANRX mapped to PB8, CANTX mapped to PB9
				REMAP_PD0_PD1 = AFIO_MAPR_CAN_REMAP_REMAP3,		///< CANRX mapped to PD0, CANTX mapped to PD1
			};
			
			/**
			 * Remap CAN to other locations.
			 *
			 * Remap has to be done before calling initialize() otherwise
			 * the wrong pins are configured.
			 */
			static inline void
			remapPins(Remap mapping)
			{
				AFIO->MAPR = (AFIO->MAPR & ~AFIO_MAPR_CAN_REMAP) | mapping;
			}
			
		public:
			/**
			 * Enables the clock for the CAN controller and resets all settings
			 *
			 * \warning	Has to be called after remapPins and before any other
			 * 			function from this class!
			 */
			static bool
			initialize(can::Bitrate bitrate);
			
			/**
			 * The the operating mode.
			 * 
			 * Default after initialization is the normal mode.
			 */
			static void
			setMode(can::Mode mode);
			
		public:
			static bool
			isMessageAvailable();

			static bool
			getMessage(can::Message& message);

			/*
			 * The CAN controller has a free slot to send a new message.
			 * \return true if a slot is available, false otherwise
			 */
			static bool
			isReadyToSend();

			/*
			 * Send a message over the CAN.
			 * \return true if the message was send, false otherwise
			 */
			static bool
			sendMessage(const can::Message& message);

		public:
			/**
			 * Get Receive Error Counter.
			 *
			 * In case of an error during reception, this counter is
			 * incremented by 1 or by 8 depending on the error condition as
			 * defined by the CAN standard. After every successful reception
			 * the counter is decremented by 1 or reset to 120 if its value
			 * was higher than 128. When the counter value exceeds 127, the
			 * CAN controller enters the error passive state.
			 */
			static inline uint8_t
			getReceiveErrorCounter()
			{
				return (CAN1->ESR & CAN_ESR_REC) >> 24;
			}

			/**
			 * Get Transmit Error Counter.
			 *
			 */
			static inline uint8_t
			getTransmitErrorCounter()
			{
				return (CAN1->ESR & CAN_ESR_TEC) >> 16;
			}

			static can::BusState
			getBusState();
		};
	}
}

#endif	//  XPCC_STM32__CAN1_HPP