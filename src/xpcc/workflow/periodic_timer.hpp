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
 *
 * $Id$
 */
// ----------------------------------------------------------------------------

#ifndef XPCC__PERIODIC_TIMER_HPP
#define XPCC__PERIODIC_TIMER_HPP

#include <xpcc/workflow/timeout.hpp>

namespace xpcc
{
	/**
	 * \brief		Software timer
	 * 
	 * \tparam	T	Used timer, default is xpcc::Clock() which should have
	 * 				a millisecond resolution.
	 * 
	 * \author	Fabian Greif
	 * \ingroup	workflow
	 */
	template<typename T = ::xpcc::Clock>
	class PeriodicTimer
	{
	public:
		/**
		 * \brief	Create and start the timer
		 */
		PeriodicTimer(const Timestamp interval);
		
		/// Stop the timer
		inline void
		stop();
		
		/// Check if the timer is running
		inline bool
		isRunning() const;
		
		/// Set a new interval
		void
		restart(const Timestamp interval);
		
		/**
		 * \brief	Check if a new period has started
		 * 
		 * This function can be used to easily write sections that are
		 * excuted at defined periods:
		 * \code
		 * xpcc::PeriodicTimer<> timer(50);
		 * while (1)
		 * {
		 *     if (timer.isExpired()) {
		 *         // will be executed every 50 ms
		 *     }
		 * }
		 * \endcode
		 * 
		 * \return	\c true if entering a new period, \c false otherwise
		 * 
		 * \todo	Find a better name for this function
		 */
		bool
		isExpired();
		
	private:
		Timeout<T> timer;
		
		Timestamp interval;
		bool isRunning_;
	};
}

#include "periodic_timer_impl.hpp"

#endif // XPCC__PERIODIC_TIMER_HPP