//
// AtomicCounter.cpp
//
// $Id$
//
// Library: Foundation
// Package: Core
// Module:  AtomicCounter
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


#include "Poco/AtomicCounter.h"


namespace Poco {


#if POCO_OS == POCO_OS_WINDOWS_NT
//
// Windows
//
AtomicCounter::AtomicCounter():
	_counter(0)
{
}

	
AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
	_counter(counter.value())
{
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	InterlockedExchange(&_counter, counter.value());
	return *this;
}

	
AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	InterlockedExchange(&_counter, value);
	return *this;
}


#elif POCO_OS == POCO_OS_MAC_OS_X
//
// Mac OS X
//
AtomicCounter::AtomicCounter():
	_counter(0)
{
}

	
AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue):
	_counter(initialValue)
{
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter):
	_counter(counter.value())
{
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	_counter = counter.value();
	return *this;
}

	
AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	_counter = value;
	return *this;
}


#else
//
// Generic implementation based on FastMutex
//
AtomicCounter::AtomicCounter()
{
	_counter.value = 0;
}

	
AtomicCounter::AtomicCounter(AtomicCounter::ValueType initialValue)
{
	_counter.value = initialValue;
}


AtomicCounter::AtomicCounter(const AtomicCounter& counter)
{
	_counter.value = counter.value();
}


AtomicCounter::~AtomicCounter()
{
}


AtomicCounter& AtomicCounter::operator = (const AtomicCounter& counter)
{
	FastMutex::ScopedLock lock(_counter.mutex);
	_counter.value = counter.value();
	return *this;
}

	
AtomicCounter& AtomicCounter::operator = (AtomicCounter::ValueType value)
{
	FastMutex::ScopedLock lock(_counter.mutex);
	_counter.value = value;
	return *this;
}


#endif // POCO_OS


} // namespace Poco
