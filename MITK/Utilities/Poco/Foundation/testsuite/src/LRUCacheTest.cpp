//
// LRUCacheTest.cpp
//
// $Id$
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#include "LRUCacheTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Exception.h"
#include "Poco/LRUCache.h"
#include "Poco/Bugcheck.h"


using namespace Poco;


LRUCacheTest::LRUCacheTest(const std::string& name): CppUnit::TestCase(name)
{
}


LRUCacheTest::~LRUCacheTest()
{
}


void LRUCacheTest::testClear()
{
	LRUCache<int, int> aCache(3);
	assert (aCache.size() == 0);
	assert (aCache.getAllKeys().size() == 0);
	aCache.add(1, 2);
	aCache.add(3, 4);
	aCache.add(5, 6);
	assert (aCache.size() == 3);
	assert (aCache.getAllKeys().size() == 3);
	assert (aCache.has(1));
	assert (aCache.has(3));
	assert (aCache.has(5));
	assert (*aCache.get(1) == 2);
	assert (*aCache.get(3) == 4);
	assert (*aCache.get(5) == 6);
	aCache.clear();
	assert (!aCache.has(1));
	assert (!aCache.has(3));
	assert (!aCache.has(5));
}


void LRUCacheTest::testCacheSize0()
{
	// cache size 0 is illegal
	try
	{
		LRUCache<int, int> aCache(0);
		failmsg ("cache size of 0 is illegal, test should fail");
	}
	catch (Poco::InvalidArgumentException&)
	{
	}
}


void LRUCacheTest::testCacheSize1()
{
	LRUCache<int, int> aCache(1);
	aCache.add(1, 2);
	assert (aCache.has(1));
	assert (*aCache.get(1) == 2);

	aCache.add(3, 4); // replaces 1
	assert (!aCache.has(1));
	assert (aCache.has(3));
	assert (*aCache.get(3) == 4);

	aCache.add(5, 6);
	assert (!aCache.has(1));
	assert (!aCache.has(3));
	assert (aCache.has(5));
	assert (*aCache.get(5) == 6);

	aCache.remove(5);
	assert (!aCache.has(5));

	// removing illegal entries should work too
	aCache.remove(666);
}


void LRUCacheTest::testCacheSize2()
{
	// 3-1 represents the cache sorted by pos, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	LRUCache<int, int> aCache(2);
	aCache.add(1, 2); // 1
	assert (aCache.has(1));
	assert (*aCache.get(1) == 2);

	aCache.add(3, 4); // 3-1
	assert (aCache.has(1));
	assert (aCache.has(3));
	assert (*aCache.get(1) == 2); // 1-3
	assert (*aCache.get(3) == 4); // 3-1

	aCache.add(5, 6); // 5-3|1
	assert (!aCache.has(1));
	assert (aCache.has(3));
	assert (aCache.has(5));
	assert (*aCache.get(5) == 6);  // 5-3
	assert (*aCache.get(3) == 4);  // 3-5

	// test remove from the end and the beginning of the list
	aCache.remove(5); // 3
	assert (!aCache.has(5));
	assert (*aCache.get(3) == 4);  // 3
	aCache.add(5, 6); // 5-3
	assert (*aCache.get(3) == 4);  // 3-5
	aCache.remove(3); // 5
	assert (!aCache.has(3));
	assert (*aCache.get(5) == 6);  // 5

	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assert (!aCache.has(5));
}


void LRUCacheTest::testCacheSizeN()
{
		// 3-1 represents the cache sorted by pos, elements get replaced at the end of the list
	// 3-1|5 -> 5 gets removed
	LRUCache<int, int> aCache(3);
	aCache.add(1, 2); // 1
	assert (aCache.has(1));
	assert (*aCache.get(1) == 2);

	aCache.add(3, 4); // 3-1
	assert (aCache.has(1));
	assert (aCache.has(3));
	assert (*aCache.get(1) == 2); // 1-3
	assert (*aCache.get(3) == 4); // 3-1

	aCache.add(5, 6); // 5-3-1
	assert (aCache.has(1));
	assert (aCache.has(3));
	assert (aCache.has(5));
	assert (*aCache.get(5) == 6);  // 5-3-1
	assert (*aCache.get(3) == 4);  // 3-5-1

	aCache.add(7, 8); // 7-5-3|1
	assert (!aCache.has(1));
	assert (aCache.has(7));
	assert (aCache.has(3));
	assert (aCache.has(5));
	assert (*aCache.get(5) == 6);  // 5-7-3
	assert (*aCache.get(3) == 4);  // 3-5-7
	assert (*aCache.get(7) == 8);  // 7-3-5

	// test remove from the end and the beginning of the list
	aCache.remove(5); // 7-3
	assert (!aCache.has(5));
	assert (*aCache.get(3) == 4);  // 3-7
	aCache.add(5, 6); // 5-3-7
	assert (*aCache.get(7) == 8);  // 7-5-3
	aCache.remove(7); // 5-3
	assert (!aCache.has(7));
	assert (aCache.has(3));
	assert (*aCache.get(5) == 6);  // 5-3

	// removing illegal entries should work too
	aCache.remove(666);

	aCache.clear();
	assert (!aCache.has(5));
	assert (!aCache.has(3));
}


void LRUCacheTest::testDuplicateAdd()
{
	LRUCache<int, int> aCache(3);
	aCache.add(1, 2); // 1
	assert (aCache.has(1));
	assert (*aCache.get(1) == 2);
	aCache.add(1, 3);
	assert (aCache.has(1));
	assert (*aCache.get(1) == 3);
}


void LRUCacheTest::setUp()
{
}


void LRUCacheTest::tearDown()
{
}


CppUnit::Test* LRUCacheTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("LRUCacheTest");

	CppUnit_addTest(pSuite, LRUCacheTest, testClear);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize0);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize1);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSize2);
	CppUnit_addTest(pSuite, LRUCacheTest, testCacheSizeN);
	CppUnit_addTest(pSuite, LRUCacheTest, testDuplicateAdd);

	return pSuite;
}
