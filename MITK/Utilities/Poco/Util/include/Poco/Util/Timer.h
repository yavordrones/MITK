//
// Timer.h
//
// $Id$
//
// Library: Util
// Package: Timer
// Module:  Timer
//
// Definition of the Timer class.
//
// Copyright (c) 2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Util_Timer_INCLUDED
#define Util_Timer_INCLUDED


#include "Poco/Util/Util.h"
#include "Poco/Util/TimerTask.h"
#include "Poco/TimedNotificationQueue.h"
#include "Poco/Thread.h"
#include "Poco/Runnable.h"


namespace Poco {
namespace Util {


class Util_API Timer: protected Poco::Runnable
	/// A Timer allows to schedule tasks (TimerTask objects) for future execution 
	/// in a background thread. Tasks may be scheduled for one-time execution, 
	/// or for repeated execution at regular intervals. 
	///
	/// The Timer object creates a thread that executes all scheduled tasks
	/// sequentially. Therefore, tasks should complete their work as quickly
	/// as possible, otherwise subsequent tasks may be delayed.
	///
	/// Timer is save for multithreaded use - multiple threads can schedule
	/// new tasks simultaneously.
	///
	/// Acknowledgement: The interface of this class has been inspired by
	/// the java.util.Timer class from Java 1.3.
{
public:
	Timer();
		/// Creates the Timer.
	
	explicit Timer(Poco::Thread::Priority priority);
		/// Creates the Timer, using a timer thread with
		/// the given priority.
	
	~Timer();
		/// Destroys the Timer, cancelling all pending tasks.
		
	void cancel();
		/// Cancels all pending tasks.
		///
		/// If a task is currently running, it is allowed to finish.
	
	void schedule(TimerTask::Ptr pTask, Poco::Timestamp time);
		/// Schedules a task for execution at the specified time.
		///
		/// If the time lies in the past, the task is executed
		/// immediately.
		
	void schedule(TimerTask::Ptr pTask, long delay, long interval);
		/// Schedules a task for periodic execution.
		///
		/// The task is first executed after the given delay.
		/// Subsequently, the task is executed periodically with
		/// the given interval in milliseconds between invocations.

	void schedule(TimerTask::Ptr pTask, Poco::Timestamp time, long interval);
		/// Schedules a task for periodic execution.
		///
		/// The task is first executed at the given time.
		/// Subsequently, the task is executed periodically with
		/// the given interval in milliseconds between invocations.
		
	void scheduleAtFixedRate(TimerTask::Ptr pTask, long delay, long interval);
		/// Schedules a task for periodic execution at a fixed rate.
		///
		/// The task is first executed after the given delay.
		/// Subsequently, the task is executed periodically 
		/// every number of milliseconds specified by interval.
		///
		/// If task execution takes longer than the given interval,
		/// further executions are delayed.

	void scheduleAtFixedRate(TimerTask::Ptr pTask, Poco::Timestamp time, long interval);
		/// Schedules a task for periodic execution at a fixed rate.
		///
		/// The task is first executed at the given time.
		/// Subsequently, the task is executed periodically 
		/// every number of milliseconds specified by interval.
		///
		/// If task execution takes longer than the given interval,
		/// further executions are delayed.

protected:
	void run();
		
private:
	Timer(const Timer&);
	Timer& operator = (const Timer&);
	
	Poco::TimedNotificationQueue _queue;
	Poco::Thread _thread;
};


} } // namespace Poco::Util


#endif // Util_Timer_INCLUDED
