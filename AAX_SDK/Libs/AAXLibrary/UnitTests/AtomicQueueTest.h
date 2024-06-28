/*================================================================================================*/
/*
 *	Copyright 2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */

/**  
 *	\file AtomicQueueTest.h
 *
 *	\author Rob Majors
 *
 */ 
/*================================================================================================*/

// Unit Test Includes
#include "Digi_UnitTest/Digi_UT_Include.h"

// AAX Includes
#include "AAX_CAtomicQueue.h"
#include "AAX_CTestThread.h"

// Standard Includes
#include <iostream>
#include <iomanip>
#include <ctime>


SUITE(AtomicQueueTest)
{
	struct StTestTimer
	{
		explicit StTestTimer(const char* inTestName) : mStart(std::clock()), mTestName(inTestName) {}
		
		~StTestTimer()
		{
			const std::clock_t end = std::clock();
			double duration = (1000.0 * (end - mStart)) / CLOCKS_PER_SEC; // Doesn't handle wraparound
			std::cout << mTestName << " - " << std::fixed << std::setprecision(2) << "CPU time used: " << duration  << " ms" << std::endl;
		}
		
	private:
		std::clock_t mStart;
		const char* mTestName;
	};
	
	TEST(SingleThreaded_AddAndRemove)
	{
		static const size_t sSize = 32;
		AAX_CAtomicQueue<void, sSize> queue;
		AAX_IContainer::EStatus pushResult;
		void* pop;
		void* peek;
		
		// Peek without any elements
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x0, peek);
		
		// Add and remove one element
		pushResult = queue.Push((void*)0x1);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x1, pop);
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x0, peek);
		
		// Add and remove multiple elements
		pushResult = queue.Push((void*)0x1);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pushResult = queue.Push((void*)0x2);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pushResult = queue.Push((void*)0x3);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pushResult = queue.Push((void*)0x4);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pushResult = queue.Push((void*)0x5);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x1, pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x2, pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x3, pop);
		pushResult = queue.Push((void*)0x6);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pushResult = queue.Push((void*)0x7);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x4, peek);
		pushResult = queue.Push((void*)0x8);
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x4, pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x5, pop);
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x6, peek);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x6, pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x7, pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x8, pop);
	}
	
	TEST(UnsupportedOperationTest)
	{
		static const size_t sSize = 32;
		AAX_CAtomicQueue<void, sSize> queue;
		AAX_IContainer::EStatus pushResult;
		
		pushResult = queue.Push(NULL);
		CHECK_EQUAL(AAX_IContainer::eStatus_Unsupported, pushResult);
	}
	
	TEST(SingleThreaded_Overflow)
	{
		static const size_t sSize = 32;
		AAX_CAtomicQueue<void, sSize> queue;
		AAX_IContainer::EStatus pushResult;
		void* pop;
		void* peek;
		
		// Fill the queue
		for (size_t i = 0; i < sSize; ++i)
		{
			pushResult = queue.Push((void*)(i+1)); // Never push NULL onto the queue
			CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		}
		
		// Pop one element from the filled queue
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x1, pop);
		
		// Push a new element onto the end of the queue
		pushResult = queue.Push((void*)(sSize+2));
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		
		// Peek at the filled queue
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x2, peek);
		
		// Attempt to push an element onto the end of the full queue
		pushResult = queue.Push((void*)(sSize+3));
		CHECK_EQUAL(AAX_IContainer::eStatus_Overflow, pushResult);
		
		// Pop one element from the filled queue
		pop = queue.Pop();
		CHECK_EQUAL((void*)0x2, pop);
		
		// Push a new element onto the end of the queue
		pushResult = queue.Push((void*)(sSize+4));
		CHECK_EQUAL(AAX_IContainer::eStatus_Success, pushResult);
		
		// Peek at the filled queue
		peek = queue.Peek();
		CHECK_EQUAL((void*)0x3, peek);
		
		// Attempt to push multiple elements onto the end of the queue
		pushResult = queue.Push((void*)(sSize+5));
		CHECK_EQUAL(AAX_IContainer::eStatus_Overflow, pushResult);
		pushResult = queue.Push((void*)(sSize+6));
		CHECK_EQUAL(AAX_IContainer::eStatus_Overflow, pushResult);
		pushResult = queue.Push((void*)(sSize+7));
		CHECK_EQUAL(AAX_IContainer::eStatus_Overflow, pushResult);
		pushResult = queue.Push((void*)(sSize+8));
		CHECK_EQUAL(AAX_IContainer::eStatus_Overflow, pushResult);
		
		// Read all elements off of the queue
		for (size_t i = 3; i < sSize+1; ++i)
		{
			pop = queue.Pop();
			CHECK_EQUAL((void*)i, pop);
		}
		pop = queue.Pop();
		CHECK_EQUAL((void*)(sSize+2), pop);
		pop = queue.Pop();
		CHECK_EQUAL((void*)(sSize+4), pop);
	}
	
	TEST(SingleThreaded_Performance)
	{
		static const uint32_t sSize = 0xFFFF;
		AAX_CAtomicQueue<void, sSize> queue;
		
		{
			StTestTimer timer("AtomicQueueTest::SingleThreaded_Performance_Push");
			for (uint32_t i = 0; i < 0x10000; ++i)
			{
				queue.Push((void*)0x1);
			}
		}
		
		{
			StTestTimer timer("AtomicQueueTest::SingleThreaded_Performance_Peek");
			for (uint32_t i = 0; i < 0x10000; ++i)
			{
				queue.Peek();
			}
		}
		
		{
			StTestTimer timer("AtomicQueueTest::SingleThreaded_Performance_Pop");
			for (uint32_t i = 0; i < 0x10000; ++i)
			{
				queue.Pop();
			}
		}
		
		for (uint32_t i = 0; i < 0x10000; ++i)
		{
			queue.Push((void*)0x1);
		}
		
		{
			StTestTimer timer("AtomicQueueTest::SingleThreaded_Performance_PeekPop");
			for (uint32_t i = 0; i < 0x10000; ++i)
			{
				queue.Peek();
				queue.Pop();
			}
		}
		
		{
			StTestTimer timer("AtomicQueueTest::SingleThreaded_Performance_PushPeekPop");
			for (uint32_t i = 0; i < 0x10000; ++i)
			{
				queue.Push((void*)0x1);
				queue.Peek();
				queue.Pop();
			}
		}
	}
	
	template<int NUM_BITS>
	static
	uint32_t GetLSBMask(void)
	{
		static bool sInitialized = false;
		static uint32_t sLSBMask = 0xFFFFFFFF;
		if (false == sInitialized)
		{
			sLSBMask = sLSBMask >> (32 - NUM_BITS);
			sInitialized = true;
		}
		return sLSBMask;
	}
	
	template<int NUM_BITS>
	static
	uint32_t GetMSBMask(void)
	{
		static bool sInitialized = false;
		static uint32_t sMSBMask = 0xFFFFFFFF;
		if (false == sInitialized)
		{
			sMSBMask = sMSBMask << (32 - NUM_BITS);
			sInitialized = true;
		}
		return sMSBMask;
	}
	
	TEST(InternalUtilityCheck)
	{
		CHECK_EQUAL(0x1FFFFFFFUL, GetLSBMask<29>());
		CHECK_EQUAL(0x00000001UL, GetLSBMask<1>());
		CHECK_EQUAL(0x7FFFFFFFUL, GetLSBMask<31>());
		
		CHECK_EQUAL(0xE0000000UL, GetMSBMask<3>());
		CHECK_EQUAL(0xFFFFFFFEUL, GetMSBMask<31>());
		CHECK_EQUAL(0x80000000UL, GetMSBMask<1>());
	}
	
	template<bool DO_SURRENDER_READ_THREAD, int NUM_INDEX_BITS, int NUM_WRITE_THREAD_BITS, int QUEUE_SIZE_BITS>
	static
	void MultiThreadedTest()
	{
		// Debug logging
		static const bool sDoTestLogging = false;
		
		// Sanity checks
		CHECK(32 >= NUM_INDEX_BITS + NUM_WRITE_THREAD_BITS);
		CHECK(0 < NUM_INDEX_BITS);
		CHECK(0 < NUM_WRITE_THREAD_BITS);
		CHECK(0 < QUEUE_SIZE_BITS);
		
		static const bool sDoReadThreadSurrender = DO_SURRENDER_READ_THREAD;
		
		// Example:
		//
		// NUM_INDEX_BITS:        10
		// NUM_WRITE_THREAD_BITS:  4
		// QUEUE_SIZE_BITS:        5
		//
		// -
		// Index values
		//   sMaxIndex: 1023 (max possible index value to fit into NUM_INDEX_BITS)
		//   sIndexMask: 0x000003FF (sMaxIndex fits into this mask at the LSB location)
		//
		// -
		// Write threads
		//   sThreadShift: 28
		//   sNumWriteThreads: 15
		//   sThreadMask: 0xF0000000 (sNumWriteThreads fits into this mask at the MSB location)
		//
		// -
		// Queue size
		//   sSize: 32
		//
		// -
		// Test operation
		//   The test spawns 15 write threads. Each write thread pushes values onto the queue
		//   using a mask of the thread's index in the MSB location and the number of values
		//   which the thread has pushed onto the queue so far in the LSB location. Each thread
		//   pushes values until the number of the values fills the lowest 10 bits:
		//
		//     write thread 1 - push: 0x10000001
		//     write thread 1 - push: 0x10000002
		//     write thread 1 - push: 0x10000003
		//     write thread 2 - push: 0x20000001
		//     write thread 1 - push: 0x10000004
		//     write thread 7 - push: 0x70000001
		//     write thread 2 - push: 0x20000002
		//     ...
		//     write thread 6 - push: 0x600003FF
		//     write thread 1 - push: 0x100003FE
		//     write thread 2 - push: 0x200003FF
		//     write thread 1 - push: 0x100003FF
		//
		//   When the read thread pops a value off of the queue, it checks to make sure that
		//   the values received from each write thread are in order:
		//
		//     read thread - pop 0x10000001 (OK: this is the first value received for thread 1)
		//     read thread - pop 0x10000002 (OK: increments the last value received for thread 1)
		//     read thread - pop 0x10000003 (OK: increments the last value received for thread 1)
		//     read thread - pop 0x20000001 (OK: this is the first value received for thread 2)
		//     read thread - pop 0x10000004 (OK: increments the last value received for thread 1)
		//     read thread - pop 0x70000001 (OK: this is the first value received for thread 7)
		//     read thread - pop 0x20000002 (OK: increments the last value received for thread 2)
		//     ...
		//     read thread - pop 0x600003FF (mFinishedThreadBits: 0x00000040)
		//     read thread - pop 0x100003FE
		//     read thread - pop 0x200003FF (mFinishedThreadBits: 0x00000042)
		//     read thread - pop 0x100003FF (mFinishedThreadBits: 0x00000043)
		//
		//  Each time a 0x_00003FF value is received, the read thread sets a flag indicating that
		//  no more values are expected from the write thread indicated in the MSB location. Once
		//  this flag bitfield reaches 0x00007FFF (15 bits set) then the read thread expects that
		//  the queue is now empty and all work is complete.
		//
		
		
		static const int sNumIndexBits = NUM_INDEX_BITS;
		static const uint32_t sMaxIndex = (1 << sNumIndexBits) - 1; // sNumIndexBits LSB bits indicate index
		static const uint32_t sIndexMask = GetLSBMask<NUM_INDEX_BITS>(); // mask off everything except the sNumIndexBits LSB bits
		
		static const int sNumWriteThreadBits = NUM_WRITE_THREAD_BITS;
		static const int sThreadShift = 32 - sNumWriteThreadBits; // 32: num bits in uint32_t
		static const uint32_t sNumWriteThreads = (1 << sNumWriteThreadBits) - 1; // sNumWriteThreadBits MSB bits indicate thread number
		static const uint32_t sThreadMask = GetMSBMask<NUM_WRITE_THREAD_BITS>(); // mask off everything except the sNumWriteThreadBits MSB bits
		
		static const size_t sSize = 1 << QUEUE_SIZE_BITS;
		AAX_CAtomicQueue<void, sSize> queue;
		
		CHECK(32 >= sNumIndexBits + sNumWriteThreadBits);
		if (32 >= sNumIndexBits + sNumWriteThreadBits)
		{
			
			//
			// Thread operation definitions
			//
			
			class PushFunc : public AAX_CTestThread::Functor
			{
			public:
				virtual ~PushFunc() {}
				explicit PushFunc(AAX_IPointerQueue<void>& inQueue, uint32_t inThreadNum)
				: mQueue(inQueue)
				, mThreadBits(inThreadNum << sThreadShift) // see sNumWriteThreads
				, mNoActionCounter(0)
				{
					if (sDoTestLogging)
					{
						printf("Created thread: %lx, bits: %lx, masked bits: %lx, calculated thread: %lx\n",
							   (unsigned long)inThreadNum,
							   (unsigned long)mThreadBits,
							   (unsigned long)(mThreadBits & sThreadMask),
							   (unsigned long)((mThreadBits & sThreadMask) >> sThreadShift));
					}
					
					CHECK_EQUAL(0UL, mThreadBits & sIndexMask);
					CHECK_EQUAL(inThreadNum, (mThreadBits & sThreadMask) >> sThreadShift);
				}
				
				virtual void Run(void)
				{
					for (uint32_t i = 1; (sMaxIndex >= i) && (false == IsStuck()); /* increment in loop */)
					{
						CHECK_EQUAL((i & sIndexMask), i);
						const AAX_IContainer::EStatus pushStatus = mQueue.Push(reinterpret_cast<void*>(mThreadBits | (i & sIndexMask)));
						
						if (sDoTestLogging)
						{
							printf("(thread: %lx) pushed value %lu to queue (%#.8lx) (result: %ld)\n",
								   (unsigned long)(mThreadBits >> sThreadShift),
								   (unsigned long)i,
								   (unsigned long)(mThreadBits | (i & sIndexMask)),
								   (long)pushStatus);
						}
						
						// Only increment if push succeeded
						if (AAX_IContainer::eStatus_Success == pushStatus)
						{
							++i;
							mNoActionCounter = 0;
						}
						else if (UINT32_MAX > mNoActionCounter)
						{
							++mNoActionCounter;
						}

						AAXThread::SurrenderCPU();
					}
					
					CHECK_EQUAL(false, IsStuck()); // Detect hang
				}
				
			private:
				bool IsStuck() const
				{
					return UINT32_MAX <= mNoActionCounter;
				}
				
			private:
				AAX_IPointerQueue<void>& mQueue;
				uint32_t mThreadBits;
				uint32_t mNoActionCounter;
			};
			
			class PopFunc : public AAX_CTestThread::Functor
			{
			public:
				virtual ~PopFunc() {}
				explicit PopFunc(AAX_IPointerQueue<void>& inQueue)
				: mQueue(inQueue)
				, mFinishedThreadBits(0x0)
				, mAllFinishedBits(0x0)
				, mNoActionCounter(0)
				{
					for (uint64_t i = 0; i < sNumWriteThreads; ++i)
					{
						mAllFinishedBits |= (uint64_t)1 << i;
					}
				}
				
				virtual void Run(void)
				{
					uint64_t lastVal[sNumWriteThreads];
					for (uint32_t i = 0; i < sNumWriteThreads; ++i) { lastVal[i] = 0; }
					uint64_t curVal = 0;
					void* popped;
					void* peeked;

					do
					{
						if (sDoReadThreadSurrender)
						{
							AAXThread::SurrenderCPU();
						}
						
						peeked = mQueue.Peek();
						popped = mQueue.Pop();
						
						// There is only one read thread, so if peeked is non-null then popped must equal peeked
						CHECK((NULL == peeked) || (peeked == popped));

						if (NULL != popped)
						{
							const uint32_t poppedInt = static_cast<uint32_t>(reinterpret_cast<uint64_t>(popped));

							curVal = sIndexMask & poppedInt;
							const uint32_t threadNum = (poppedInt & sThreadMask) >> sThreadShift;

							if (sDoTestLogging)
							{
								printf("(thread: %lx) popped value %llu from queue (thread mask: %#.8lx, thread bits: %lx) (%#.8lx)\n",
									(unsigned long)threadNum,
									(unsigned long long)curVal,
									(unsigned long)sThreadMask,
									(unsigned long)(poppedInt & sThreadMask),
									(unsigned long)poppedInt);
							}

							CHECK(threadNum <= sNumWriteThreads);
							if (threadNum <= sNumWriteThreads)
							{
								CHECK_EQUAL(lastVal[threadNum] + 1, curVal);

								// Update lastVal using the value that we expected to see in order
								// to avoid a cascade of failures if curVal is unexpected
								lastVal[threadNum] = lastVal[threadNum] + 1; //curVal;

								if (sMaxIndex == curVal)
								{
									mFinishedThreadBits |= (uint64_t)(1 << threadNum);
								}
							}

							mNoActionCounter = 0;
						}
						else if (UINT32_MAX > mNoActionCounter)
						{
							++mNoActionCounter;
						}
					} while ((mAllFinishedBits != mFinishedThreadBits) && (false == IsStuck()));
					
					CHECK_EQUAL(false, IsStuck()); // Detect hang
				}
				
			private:
				bool IsStuck() const
				{
					return UINT32_MAX <= mNoActionCounter;
				}
				
			private:
				AAX_IPointerQueue<void>& mQueue;
				uint64_t mFinishedThreadBits;
				uint64_t mAllFinishedBits;
				uint32_t mNoActionCounter;
			} popFunc(queue);
			
			
			//
			// Actual test operation
			//
			
			{
				AAX_CTestThread readThread(&popFunc);
				PushFunc* pushFunc[sNumWriteThreads];
				AAX_CTestThread* writeThread[sNumWriteThreads];
				for (uint32_t i = 0; i < sNumWriteThreads; ++i)
				{
					pushFunc[i] = new PushFunc(queue, i);
					writeThread[i] = new AAX_CTestThread(pushFunc[i]);
				}
				
				for (uint32_t i = 0; i < sNumWriteThreads; ++i)
				{
					writeThread[i]->Finish();
				}
				
				readThread.Finish();
				
				for (uint32_t i = 0; i < sNumWriteThreads; ++i)
				{
					delete pushFunc[i];
					delete writeThread[i];
				}
			}
		}
	}
	
	// MultiThreadedTest<bool DO_SURRENDER_READ_THREAD, int NUM_INDEX_BITS, int NUM_WRITE_THREAD_BITS, int QUEUE_SIZE_BITS>

	TEST(MultiThreaded_AddAndRemove_SingleWriteThread_NoWraparound)
	{
		//Test both with and without surrender
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_SingleWriteThread_NoWraparound without read thread surrender");
			MultiThreadedTest<false, 11, 1, 14>();
		}
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_SingleWriteThread_NoWraparound with read thread surrender");
			MultiThreadedTest<true, 11, 1, 14>();
		}
	}
	
	TEST(MultiThreaded_AddAndRemove_SingleWriteThread_Wraparound)
	{
		//Test both with and without surrender
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_SingleWriteThread_Wraparound without read thread surrender");
			MultiThreadedTest<false, 11, 1, 5>();
		}
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_SingleWriteThread_Wraparound with read thread surrender");
			MultiThreadedTest<true, 11, 1, 5>();
		}
	}
	
	TEST(MultiThreaded_AddAndRemove_MultiWriteThreads_NoWraparound)
	{
		StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_MultiWriteThreads_NoWraparound");
		MultiThreadedTest<false, 10, 5, 16>();
	}
	
	TEST(MultiThreaded_AddAndRemove_MultiWriteThreads_Wraparound)
	{
		//Test both with and without surrender
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_MultiWriteThreads_Wraparound without read thread surrender");
			MultiThreadedTest<false, 13, 5, 5>();
		}
		{
			StTestTimer timer("AtomicQueueTest::MultiThreaded_AddAndRemove_MultiWriteThreads_Wraparound with read thread surrender");
			MultiThreadedTest<true, 10, 4, 5>();
		}
	}
	
}



