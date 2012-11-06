// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2009, Roboterclub Aachen e.V.
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

#ifndef XPCC_ATMEGA__UART0_HPP
#define XPCC_ATMEGA__UART0_HPP

#include <stdint.h>
#include <xpcc/architecture/utils.hpp>

namespace xpcc
{
	namespace atmega
	{
		/**
		 * \brief		UART0
		 * 
		 * Example:
		 * \include uart/basic/main.cpp
		 * 
		 * This implementation uses a ringbuffer. The size of the ringbuffer
		 * can be changed in the \c project.cfg file.
		 * 
		 * Just add add a value for UART0_TX_BUFFER_SIZE and/or 
		 * UART0_RX_BUFFER_SIZE in the \c defines section. The size can
		 * be any value between 1 and 254.
		 * 
		 * Example:
		 * \code
		 * [defines]
		 * UART0_TX_BUFFER_SIZE = 20
		 * UART0_RX_BUFFER_SIZE = 30
		 * \endcode
		 * 
		 * \ingroup		atmega
		 */
		class BufferedUart0 INHERIT_DOCUMENTATION(::xpcc::BufferedUart)
		{
		public:
			BufferedUart0(uint32_t baudrate)
			{
				this->setBaudrate(baudrate);
			}
			
			/**
			 * \brief	Set baudrate
			 *
			 * If this function is called with a constant value as parameter
			 * all the calculation is done by the compiler. This way no 32-bit
			 * arithmetic is needed at run-time!
			 *
			 * \param	baudrate	desired baud rate
			 * \param	u2x			enabled double speed mode
			 */
			static ALWAYS_INLINE void
			setBaudrate(uint32_t baudrate, bool u2x = false)
			{
				uint16_t ubrr;
				if (u2x) {
					ubrr  = (F_CPU / (baudrate * 8l)) - 1;
					ubrr |= 0x8000;
				}
				else {
					ubrr = (F_CPU / (baudrate * 16l)) - 1;
				}
				setBaudrateRegister(ubrr);
			}
			
			// documentation inherited
			static bool
			write(uint8_t data);
			
			// documentation inherited
			static bool
			read(uint8_t& c);
			
			// documentation inherited
			static std::size_t
			write(const uint8_t *data, std::size_t length);

			// documentation inherited
			static std::size_t
			read(uint8_t *buffer, std::size_t length);

			// documentation inherited
			static bool
			isWriteFinished();
			
			/**
			 * \brief	Check whether any errors occurred during receiving
			 *			Be aware that these indicate an error that occurred
			 *			somewhere since resetting the Error Flags (with 
			 *			resetErrorFlags()), so you cannot tell which byte had
			 *			the error.
			 *
			 * \return	\c 0 if no errors occurred, otherwise a value that
			 *			corresponds to the Error Flags in register A.
			 */
			static uint8_t
			getErrorFlags();
			
			static void
			acknowledgeErrorFlags();
			
			/**
			 * \brief	Empty the receive FIFO queue and UART buffer.
			 *
			 * \return	the size of the deleted FIFO queue.
			 */
			static std::size_t
			flushReceiveBuffer();
			
			static std::size_t
			flushTransmitBuffer();
			
		protected:
			static void
			setBaudrateRegister(uint16_t ubrr);
		};
	}
}

#endif // XPCC_ATMEGA__UART0_HPP