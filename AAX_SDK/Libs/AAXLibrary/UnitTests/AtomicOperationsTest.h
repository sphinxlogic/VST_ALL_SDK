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
 *	\file AtomicOperationsTest.h
 *
 *	\author Rob Majors
 *
 */ 
/*================================================================================================*/

// Unit Test Includes
#include "Digi_UnitTest/Digi_UT_Include.h"

// AAX Includes
#include "AAX_Atomic.h"
#include "AAX_CTestThread.h"
#include "AAX.h"

// Standard Includes
#include <set>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>


SUITE(AtomicOperationsTest)
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
	
	TEST(SingleThreaded)
	{
		const size_t ptrSize = sizeof(void*);
#if AAX_PointerSize == AAXPointer_32bit
		CHECK_EQUAL(4U, ptrSize);
#elif AAX_PointerSize == AAXPointer_64bit
		CHECK_EQUAL(8U, ptrSize);
#else
#error Unsupported pointer size
#endif
		
		// AAX_Atomic_IncThenGet_32
		{
			uint32_t dataU32 = 0;
			
			CHECK_EQUAL(1U, AAX_Atomic_IncThenGet_32(dataU32));
			CHECK_EQUAL(1U, dataU32);
			CHECK_EQUAL(2U, AAX_Atomic_IncThenGet_32(dataU32));
			CHECK_EQUAL(2U, dataU32);
			dataU32 = UINT32_MAX-1;
			CHECK_EQUAL(UINT32_MAX, AAX_Atomic_IncThenGet_32(dataU32));
			CHECK_EQUAL(UINT32_MAX, dataU32);
			CHECK_EQUAL(0U, AAX_Atomic_IncThenGet_32(dataU32));
			CHECK_EQUAL(0U, dataU32);
		}
		
		// AAX_Atomic_DecThenGet_32
		{
			uint32_t dataU32 = 2;
			
			CHECK_EQUAL(1U, AAX_Atomic_DecThenGet_32(dataU32));
			CHECK_EQUAL(1U, dataU32);
			CHECK_EQUAL(0U, AAX_Atomic_DecThenGet_32(dataU32));
			CHECK_EQUAL(0U, dataU32);
			CHECK_EQUAL(UINT32_MAX, AAX_Atomic_DecThenGet_32(dataU32));
			CHECK_EQUAL(UINT32_MAX, dataU32);
			CHECK_EQUAL(UINT32_MAX-1, AAX_Atomic_DecThenGet_32(dataU32));
			CHECK_EQUAL(UINT32_MAX-1, dataU32);
		}
		
		// AAX_Atomic_Exchange_32
		{
			uint32_t dataU32_1 = 0;
			uint32_t dataU32_2 = 0;
			
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_32(dataU32_1, 1));
			CHECK_EQUAL(1U, dataU32_1);
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_32(dataU32_2, UINT32_MAX));
			CHECK_EQUAL(UINT32_MAX, dataU32_2);
			CHECK_EQUAL(1U, AAX_Atomic_Exchange_32(dataU32_1, dataU32_2));
			CHECK_EQUAL(UINT32_MAX, dataU32_1);
			CHECK_EQUAL(UINT32_MAX, dataU32_2);
		}
		
		// AAX_Atomic_Exchange_64
		{
			uint64_t dataU64_1 = 0;
			uint64_t dataU64_2 = 0;
			
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_64(dataU64_1, 1U));
			CHECK_EQUAL(1U, dataU64_1);
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_64(dataU64_2, UINT32_MAX));
			CHECK_EQUAL(UINT32_MAX, dataU64_2);
			CHECK_EQUAL(1U, AAX_Atomic_Exchange_64(dataU64_1, dataU64_2));
			CHECK_EQUAL(UINT32_MAX, dataU64_1);
			CHECK_EQUAL(UINT32_MAX, dataU64_2);
			
			dataU64_1 = 0;
			dataU64_2 = 0;
			
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_64(dataU64_2, UINT64_MAX));
			CHECK_EQUAL(UINT64_MAX, dataU64_2);
			CHECK_EQUAL(0U, AAX_Atomic_Exchange_64(dataU64_1, dataU64_2));
			CHECK_EQUAL(UINT64_MAX, dataU64_1);
			CHECK_EQUAL(UINT64_MAX, dataU64_2);
		}
		
		// AAX_Atomic_Exchange_Pointer
		{
			void* dataPtr_1 = (void*)0x0;
			void* dataPtr_2 = (void*)0x0;
			
			CHECK_EQUAL((void*)0U, AAX_Atomic_Exchange_Pointer(dataPtr_1, (void*)1U));
			CHECK_EQUAL((void*)1U, dataPtr_1);
			CHECK_EQUAL((void*)0U, AAX_Atomic_Exchange_Pointer(dataPtr_2, (void*)UINT32_MAX));
			CHECK_EQUAL((void*)UINT32_MAX, dataPtr_2);
			CHECK_EQUAL((void*)1U, AAX_Atomic_Exchange_Pointer(dataPtr_1, dataPtr_2));
			CHECK_EQUAL((void*)UINT32_MAX, dataPtr_1);
			CHECK_EQUAL((void*)UINT32_MAX, dataPtr_2);
#if AAX_PointerSize == AAXPointer_64bit
			dataPtr_1 = 0x0;
			dataPtr_2 = 0x0;
			CHECK_EQUAL((void*)0U, AAX_Atomic_Exchange_Pointer(dataPtr_2, (void*)UINT64_MAX));
			CHECK_EQUAL((void*)UINT64_MAX, dataPtr_2);
			CHECK_EQUAL((void*)0U, AAX_Atomic_Exchange_Pointer(dataPtr_1, dataPtr_2));
			CHECK_EQUAL((void*)UINT64_MAX, dataPtr_1);
			CHECK_EQUAL((void*)UINT64_MAX, dataPtr_2);
#endif
		}
		
		// AAX_Atomic_CompareAndExchange_32
		{
			struct Ints
			{
				uint32_t a;
				uint32_t b;
				uint32_t c;
				uint32_t d;
			} ints;
			
			ints.a = 0x00000000UL;
			ints.b = 0x00000001UL;
			ints.c = 0x80000000UL;
			ints.d = 0xFFFFFFFFUL;
			
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_32(ints.a, ints.b, ints.b));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_32(ints.a, ints.b, 0x00000000UL));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_32(ints.d, ints.b, ints.b));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_32(ints.d, ints.b, 0x00000000UL));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_32(ints.d, ints.b, 0xFFFFFFFFUL));
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.a, ints.a));
			CHECK_EQUAL(0x00000000UL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.d, ints.d, ints.d));
			CHECK_EQUAL(0xFFFFFFFFUL, ints.d);
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.a, 0xFFFFFFFFUL));
			CHECK_EQUAL(0xFFFFFFFFUL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.a, 0x0000FFFFUL));
			CHECK_EQUAL(0x0000FFFFUL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.a, 0x00000000UL));
			CHECK_EQUAL(0x00000000UL, ints.a);
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.a, ints.b));
			CHECK_EQUAL(ints.a, ints.b);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.b, ints.c));
			CHECK_EQUAL(ints.a, ints.c);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_32(ints.a, ints.c, ints.d));
			CHECK_EQUAL(ints.a, ints.d);
			CHECK_EQUAL(0x00000001UL, ints.b);
			CHECK_EQUAL(0x80000000UL, ints.c);
			CHECK_EQUAL(0xFFFFFFFFUL, ints.d);
		}
		
		// AAX_Atomic_CompareAndExchange_64
		{
			struct Ints
			{
				uint64_t a;
				uint64_t b;
				uint64_t c;
				uint64_t d;
			} ints;
			
			ints.a = 0x0000000000000000ULL;
			ints.b = 0x0000000000000001ULL;
			ints.c = 0x8000000000000000ULL;
			ints.d = 0xFFFFFFFFFFFFFFFFULL;
			
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_64(ints.a, ints.b, ints.b));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_64(ints.a, ints.b, 0x0000000000000000ULL));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_64(ints.d, ints.b, ints.b));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_64(ints.d, ints.b, 0x0000000000000000ULL));
			CHECK_EQUAL(false, AAX_Atomic_CompareAndExchange_64(ints.d, ints.b, 0xFFFFFFFFFFFFFFFFULL));
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.a, ints.a));
			CHECK_EQUAL(0x0000000000000000ULL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.d, ints.d, ints.d));
			CHECK_EQUAL(0xFFFFFFFFFFFFFFFFULL, ints.d);
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.a, 0xFFFFFFFFFFFFFFFFULL));
			CHECK_EQUAL(0xFFFFFFFFFFFFFFFFULL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.a, 0x00000000FFFFFFFFULL));
			CHECK_EQUAL(0x00000000FFFFFFFFULL, ints.a);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.a, 0x0000000000000000ULL));
			CHECK_EQUAL(0x0000000000000000ULL, ints.a);
			
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.a, ints.b));
			CHECK_EQUAL(ints.a, ints.b);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.b, ints.c));
			CHECK_EQUAL(ints.a, ints.c);
			CHECK_EQUAL(true, AAX_Atomic_CompareAndExchange_64(ints.a, ints.c, ints.d));
			CHECK_EQUAL(ints.a, ints.d);
			CHECK_EQUAL(0x0000000000000001ULL, ints.b);
			CHECK_EQUAL(0x8000000000000000ULL, ints.c);
			CHECK_EQUAL(0xFFFFFFFFFFFFFFFFULL, ints.d);
		}
		
		// AAX_Atomic_CompareAndExchange_Pointer
		// (not tested - just redirects to 32-bit or 64-bit cnx methods)
		
		// AAX_Atomic_Load_Pointer
		{
			void* dataPtr_1 = (void*)0x0;
			void* dataPtr_2 = (void*)0x1;
			
			CHECK_EQUAL((void*)0x0, AAX_Atomic_Load_Pointer(&dataPtr_1));
			CHECK_EQUAL((void*)0x0, dataPtr_1);
			CHECK_EQUAL((void*)0x1, AAX_Atomic_Load_Pointer(&dataPtr_2));
			CHECK_EQUAL((void*)0x1, dataPtr_2);

#if AAX_PointerSize == AAXPointer_64bit
			void* dataPtr_3 = (void*)0xFFFFFFFFFFFFFFFF;
			CHECK_EQUAL((void*)0xFFFFFFFFFFFFFFFF, AAX_Atomic_Load_Pointer(&dataPtr_3));
			CHECK_EQUAL((void*)0xFFFFFFFFFFFFFFFF, dataPtr_3);
			
			void* dataPtr_4 = (void*)0x0000000080000000;
			CHECK_EQUAL((void*)0x0000000080000000, AAX_Atomic_Load_Pointer(&dataPtr_4));
			CHECK_EQUAL((void*)0x0000000080000000, dataPtr_4);
			
			void* dataPtr_5 = (void*)0x0000000100000000;
			CHECK_EQUAL((void*)0x0000000100000000, AAX_Atomic_Load_Pointer(&dataPtr_5));
			CHECK_EQUAL((void*)0x0000000100000000, dataPtr_5);
#elif AAX_PointerSize == AAXPointer_32bit
			void* dataPtr_3 = (void*)0xFFFFFFFF;
			CHECK_EQUAL((void*)0xFFFFFFFF, AAX_Atomic_Load_Pointer(&dataPtr_3));
			CHECK_EQUAL((void*)0xFFFFFFFF, dataPtr_3);
			
			void* dataPtr_4 = (void*)0x00008000;
			CHECK_EQUAL((void*)0x00008000, AAX_Atomic_Load_Pointer(&dataPtr_4));
			CHECK_EQUAL((void*)0x00008000, dataPtr_4);
			
			void* dataPtr_5 = (void*)0x00010000;
			CHECK_EQUAL((void*)0x00010000, AAX_Atomic_Load_Pointer(&dataPtr_5));
			CHECK_EQUAL((void*)0x00010000, dataPtr_5);
#endif
		}
	}
	
#if (AAX_PointerSize == AAXPointer_64bit)
	typedef uint64_t TUIntPtr;
#elif (AAX_PointerSize == AAXPointer_32bit)
	typedef uint32_t TUIntPtr;
#else
#error Unsupported pointer size
#endif
	
	template <int NUM_ITER_PER_THREAD>
	class IncFunc_32 : public AAX_CTestThread::Functor
	{
	public:
		virtual ~IncFunc_32() {}
		explicit IncFunc_32(uint32_t& inTestInt)
		: mTestInt(inTestInt)
		, mNumIncrements(0)
		{
		}
		
		virtual void Run(void)
		{
			for (int i = 0; i < NUM_ITER_PER_THREAD; ++i)
			{
				AAX_Atomic_IncThenGet_32(mTestInt);
				AAX_Atomic_IncThenGet_32(mNumIncrements);
			}
		}
		
		uint32_t GetNumIncrements() const
		{
			return mNumIncrements;
		}
		
	private:
		uint32_t& mTestInt;
		uint32_t mNumIncrements;
	};
	
	template <int NUM_ITER_PER_THREAD>
	class DecFunc_32 : public AAX_CTestThread::Functor
	{
	public:
		virtual ~DecFunc_32() {}
		explicit DecFunc_32(uint32_t& inTestInt)
		: mTestInt(inTestInt)
		, mNumDecrements(0)
		{
		}
		
		virtual void Run(void)
		{
			for (int i = 0; i < NUM_ITER_PER_THREAD; ++i)
			{
				AAX_Atomic_DecThenGet_32(mTestInt);
				AAX_Atomic_IncThenGet_32(mNumDecrements);
			}
		}
		
		uint32_t GetNumDecrements() const
		{
			return mNumDecrements;
		}
		
	private:
		uint32_t& mTestInt;
		uint32_t mNumDecrements;
	};
	
	// Test AAX_Atomic_Exchange_32
	// Create one functor per thread
	// All functors must use the same inTestValues set
	class ExcFunc_32 : public AAX_CTestThread::Functor
	{
	public:
		typedef uint32_t TInt;
		
	public:
		virtual ~ExcFunc_32() {}
		explicit ExcFunc_32(TInt& inTestInt, const std::set<TInt>& inTestValues, int inNumIterations)
		: mTestInt(inTestInt)
		, mTestValues(inTestValues)
		, mNumIterations(inNumIterations)
		{
			// use current time as seed for random generator
			static bool sRandSeedDone = false;
			if (false == sRandSeedDone)
			{
				sRandSeedDone = true;
				std::srand((unsigned)std::time(0));
			}
		}
		
		virtual void Run(void)
		{
			std::set<TInt>::const_iterator valIter = mTestValues.begin();
			
			for (int i = 0; i < mNumIterations; ++i)
			{
				// Go to a random element in mTestValues
				for (int incCount = std::rand() % (int)mTestValues.size();
					 incCount > 0;
					 --incCount)
				{
					if (mTestValues.end() == ++valIter)
					{
						valIter = mTestValues.begin();
					}
				}
				
				const TInt val = AAX_Atomic_Exchange_32(mTestInt, *valIter);
				CHECK(0 < mTestValues.count(val));
			}
		}
		
	private:
		TInt& mTestInt;
		const std::set<TInt>& mTestValues;
		int mNumIterations;
	};
	
	// Test AAX_Atomic_Exchange_64
	// Create one functor per thread
	// All functors must use the same inTestValues set
	class ExcFunc_64 : public AAX_CTestThread::Functor
	{
	public:
		typedef uint64_t TInt;
		
	public:
		virtual ~ExcFunc_64() {}
		explicit ExcFunc_64(TInt& inTestInt, const std::set<TInt>& inTestValues, int inNumIterations)
		: mTestInt(inTestInt)
		, mTestValues(inTestValues)
		, mNumIterations(inNumIterations)
		{
			// use current time as seed for random generator
			static bool sRandSeedDone = false;
			if (false == sRandSeedDone)
			{
				sRandSeedDone = true;
				std::srand((unsigned)std::time(0));
			}
		}
		
		virtual void Run(void)
		{
			std::set<TInt>::const_iterator valIter = mTestValues.begin();
			
			for (int i = 0; i < mNumIterations; ++i)
			{
				// Go to a random element in mTestValues
				for (int incCount = std::rand() % (int)mTestValues.size();
					 incCount > 0;
					 --incCount)
				{
					if (mTestValues.end() == ++valIter)
					{
						valIter = mTestValues.begin();
					}
				}
				
				const TInt val = AAX_Atomic_Exchange_64(mTestInt, *valIter);
				CHECK(0 < mTestValues.count(val));
			}
		}
		
	private:
		TInt& mTestInt;
		const std::set<TInt>& mTestValues;
		int mNumIterations;
	};
	
	// Test AAX_Atomic_CompareAndExchange_32
	// Create one functor per thread
	// All functors must use the same inTestValues set
	class CNXFunc_32 : public AAX_CTestThread::Functor
	{
	public:
		typedef uint32_t TInt;
		
	public:
		virtual ~CNXFunc_32() {}
		explicit CNXFunc_32(TInt& inTestInt, const std::set<TInt>& inTestValues, int inNumIterations)
		: mTestInt(inTestInt)
		, mLastVal(0)
		, mTestValues(inTestValues)
		, mNumExchanged(0)
		, mNumNotExchanged(0)
		, mNumIterations(inNumIterations)
		{
			// use current time as seed for random generator
			static bool sRandSeedDone = false;
			if (false == sRandSeedDone)
			{
				sRandSeedDone = true;
				std::srand((unsigned)std::time(0));
			}
		}
		
		virtual void Run(void)
		{
			std::set<TInt>::const_iterator valIter = mTestValues.begin();
			
			for (int i = 0; i < mNumIterations; ++i)
			{
				// Go to a random element in mTestValues
				for (int incCount = std::rand() % (int)mTestValues.size();
					 incCount > 0;
					 --incCount)
				{
					if (mTestValues.end() == ++valIter)
					{
						valIter = mTestValues.begin();
					}
				}
				
				const bool exchanged = AAX_Atomic_CompareAndExchange_32(mTestInt, mLastVal, *valIter);
				mLastVal = *valIter;
				
				if (true == exchanged)
				{
					++mNumExchanged;
				}
				else
				{
					++mNumNotExchanged;
				}
				
				// Sleep between 0 and 100 us, or the minimum
				// supported by this platform
				AAXThread::SleepMS((double)(std::rand() % 1000) / 1000.);
			}
		}
		
		void CheckResults(void) const
		{
			CHECK_EQUAL(mNumIterations, mNumExchanged + mNumNotExchanged); // Sanity check
			CHECK(0 < mNumExchanged); // If this fails then the test params should be adjusted
			CHECK(0 < mNumNotExchanged); // If this fails then the test params should be adjusted
		}
		
	private:
		TInt& mTestInt;
		TInt mLastVal;
		const std::set<TInt>& mTestValues;
		int mNumExchanged;
		int mNumNotExchanged;
		int mNumIterations;
	};
	
	// Test AAX_Atomic_CompareAndExchange_64
	// Create one functor per thread
	// All functors must use the same inTestValues set
	class CNXFunc_64 : public AAX_CTestThread::Functor
	{
	public:
		typedef uint64_t TInt;
		
	public:
		virtual ~CNXFunc_64() {}
		explicit CNXFunc_64(TInt& inTestInt, const std::set<TInt>& inTestValues, int inNumIterations)
		: mTestInt(inTestInt)
		, mLastVal(0)
		, mTestValues(inTestValues)
		, mNumExchanged(0)
		, mNumNotExchanged(0)
		, mNumIterations(inNumIterations)
		{
			// use current time as seed for random generator
			static bool sRandSeedDone = false;
			if (false == sRandSeedDone)
			{
				sRandSeedDone = true;
				std::srand((unsigned)std::time(0));
			}
		}
		
		virtual void Run(void)
		{
			std::set<TInt>::const_iterator valIter = mTestValues.begin();
			
			for (int i = 0; i < mNumIterations; ++i)
			{
				// Go to a random element in mTestValues
				for (int incCount = std::rand() % (int)mTestValues.size();
					 incCount > 0;
					 --incCount)
				{
					if (mTestValues.end() == ++valIter)
					{
						valIter = mTestValues.begin();
					}
				}
				
				const bool exchanged = AAX_Atomic_CompareAndExchange_64(mTestInt, mLastVal, *valIter);
				mLastVal = *valIter;
				
				if (true == exchanged)
				{
					++mNumExchanged;
				}
				else
				{
					++mNumNotExchanged;
				}
				
				// Sleep between 0 and 1000 us, or the minimum
				// supported by this platform
				AAXThread::SleepMS((double)(std::rand() % 1000)/1000.);
			}
		}
		
		void CheckResults(void) const
		{
			CHECK_EQUAL(mNumIterations, mNumExchanged + mNumNotExchanged); // Sanity check
			CHECK(0 < mNumExchanged); // If this fails then the test params should be adjusted
			CHECK(0 < mNumNotExchanged); // If this fails then the test params should be adjusted
		}
		
	private:
		TInt& mTestInt;
		TInt mLastVal;
		const std::set<TInt>& mTestValues;
		int mNumExchanged;
		int mNumNotExchanged;
		int mNumIterations;
	};
	
	// Test AAX_Atomic_Load_Pointer
	// Create one functor per thread
	//
	// In order for the test to be relevant, this functor checks that the pointer value has changed
	// at least once between two consecutive load calls made from the same thread.
	// * If the value has changed at least once then that is a good indication that there was a
	//   concurrent call to set the pointer value handled at the same time as the first load call.
	class LoadFunc : public AAX_CTestThread::Functor
	{
	public:
		virtual ~LoadFunc() {}
		explicit LoadFunc(TUIntPtr& inTestPtr, const std::set<TUIntPtr>& inTestValues, int inNumIterations)
		: mTestPtr(inTestPtr)
		, mTestValues(inTestValues)
		, mNumDifferent(0)
		, mNumIterations(inNumIterations)
        , MAX_TICK_COUNT(100000)
		{
			// use current time as seed for random generator
			static bool sRandSeedDone = false;
			if (false == sRandSeedDone)
			{
				sRandSeedDone = true;
				std::srand((unsigned)std::time(0));
			}
		}
		
		virtual void Run(void)
		{
            // Load and wait until someone change value
            int tickCount = 0;
			for (int i = 0; i < mNumIterations; ++i)
			{
                tickCount = 0;
				// Load the current value
				void* const val1 = AAX_Atomic_Load_Pointer((void* const *)&mTestPtr);
				// Spin while someone changes value from other thread
                while (val1 == AAX_Atomic_Load_Pointer((void* const *)&mTestPtr) && ++tickCount < MAX_TICK_COUNT)
                    ;

                // Integrity check - loaded value is in the set of test values
                CHECK(0UL < mTestValues.count((TUIntPtr)val1));
                //
                if (tickCount <= MAX_TICK_COUNT)
                    ++mNumDifferent;

				AAXThread::SurrenderCPU();
			}
		}
		
		void CheckResults(void) const
		{
			CHECK(0 < mNumDifferent);
		}
		
	private:
		TUIntPtr& mTestPtr;
		const std::set<TUIntPtr>& mTestValues;
		int mNumDifferent;
		int mNumIterations;
        const int MAX_TICK_COUNT;
	};
	
	TEST(IncThenGet_32)
	{
		static const int sNumThreads = 32;
		static const int sNumIterationsPerThread = 1 << 12;
		static uint32_t sTestInt = 0;
		
		IncFunc_32<sNumIterationsPerThread> func(sTestInt);
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::IncThenGet_32");
			
			AAX_CTestThread* thread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				thread[i] = new AAX_CTestThread(&func);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				thread[i]->Finish();
			}
			
			CHECK_EQUAL(static_cast<uint32_t>(sNumThreads * sNumIterationsPerThread), func.GetNumIncrements());
			CHECK_EQUAL(static_cast<uint32_t>(sNumThreads * sNumIterationsPerThread), sTestInt);
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete thread[i];
			}
		}
	}
	
	TEST(IncThenGet_And_DecThenGet_32)
	{
		static const int sNumThreads = 32;
		static const int sNumIterationsPerThread = 1 << 12;
		static uint32_t sTestInt = 0;
		
		IncFunc_32<sNumIterationsPerThread> incFunc(sTestInt);
		DecFunc_32<sNumIterationsPerThread> decFunc(sTestInt);
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::IncThenGet_And_DecThenGet_32");
			
			AAX_CTestThread* incThread[sNumThreads];
			AAX_CTestThread* decThread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				incThread[i] = new AAX_CTestThread(&incFunc);
				decThread[i] = new AAX_CTestThread(&decFunc);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				incThread[i]->Finish();
				decThread[i]->Finish();
			}
			
			CHECK_EQUAL(static_cast<uint32_t>(sNumThreads * sNumIterationsPerThread), incFunc.GetNumIncrements());
			CHECK_EQUAL(static_cast<uint32_t>(sNumThreads * sNumIterationsPerThread), decFunc.GetNumDecrements());
			CHECK_EQUAL(0UL, sTestInt);
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete incThread[i];
				delete decThread[i];
			}
		}
	}
	
	TEST(Exchange_32)
	{
		typedef uint32_t TInt;
		
		static const int sNumThreads = 32;
		static const int sNumIterationsPerThread = 1 << 12;
		static TInt sTestInt = 0;
		
		static std::set<TInt> sTestValues;
		{
			sTestValues.insert(0x00000001);
			sTestValues.insert(0x000000FF);
			sTestValues.insert(0x0000FFFF);
			sTestValues.insert(0x80000000);
			sTestValues.insert(0xFF000000);
			sTestValues.insert(0xFFFF0000);
			sTestValues.insert(0x00000000);
			sTestValues.insert(0xFFFFFFFF);
			sTestValues.insert(0x00FF00FF);
			sTestValues.insert(0xFF00FF00);
			sTestValues.insert(0xAAAAAAAA);
			sTestValues.insert(0x55555555);
		}
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::Exchange_32");
			
			ExcFunc_32* excFunc[sNumThreads];
			AAX_CTestThread* excThread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				excFunc[i] = new ExcFunc_32(sTestInt, sTestValues, sNumIterationsPerThread);
			}
			for (int i = 0; i < sNumThreads; ++i)
			{
				excThread[i] = new AAX_CTestThread(excFunc[i]);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				excThread[i]->Finish();
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete excThread[i];
				delete excFunc[i];
			}
		}
	}
	
	TEST(Exchange_64)
	{
		typedef uint64_t TInt;
		
		static const int sNumThreads = 32;
		static const int sNumIterationsPerThread = 1 << 12;
		static TInt sTestInt = 0;
		
		static std::set<TInt> sTestValues;
		{
			sTestValues.insert(0x0000000000000001);
			sTestValues.insert(0x00000000000000FF);
			sTestValues.insert(0x000000000000FFFF);
			sTestValues.insert(0x0000000080000000);
			sTestValues.insert(0x00000000FF000000);
			sTestValues.insert(0x00000000FFFF0000);
			sTestValues.insert(0x0000000000000000);
			sTestValues.insert(0x00000000FFFFFFFF);
			sTestValues.insert(0x0000000000FF00FF);
			sTestValues.insert(0x00000000FF00FF00);
			sTestValues.insert(0x00000000AAAAAAAA);
			sTestValues.insert(0x0000000055555555);
			sTestValues.insert(0x0000000100000000);
			sTestValues.insert(0x000000FF00000000);
			sTestValues.insert(0x0000FFFF00000000);
			sTestValues.insert(0x8000000000000000);
			sTestValues.insert(0xFF00000000000000);
			sTestValues.insert(0xFFFF000000000000);
			sTestValues.insert(0xFFFFFFFF00000000);
			sTestValues.insert(0x00FF00FF00000000);
			sTestValues.insert(0xFF00FF0000000000);
			sTestValues.insert(0xAAAAAAAA00000000);
			sTestValues.insert(0x5555555500000000);
			sTestValues.insert(0xFFFFFFFF00000001);
			sTestValues.insert(0xFFFFFFFF000000FF);
			sTestValues.insert(0xFFFFFFFF0000FFFF);
			sTestValues.insert(0xFFFFFFFF80000000);
			sTestValues.insert(0xFFFFFFFFFF000000);
			sTestValues.insert(0xFFFFFFFFFFFF0000);
			sTestValues.insert(0xFFFFFFFF00000000);
			sTestValues.insert(0xFFFFFFFFFFFFFFFF);
			sTestValues.insert(0xFFFFFFFF00FF00FF);
			sTestValues.insert(0xFFFFFFFFFF00FF00);
			sTestValues.insert(0xFFFFFFFFAAAAAAAA);
			sTestValues.insert(0xFFFFFFFF55555555);
			sTestValues.insert(0x00000001FFFFFFFF);
			sTestValues.insert(0x000000FFFFFFFFFF);
			sTestValues.insert(0x0000FFFFFFFFFFFF);
			sTestValues.insert(0x80000000FFFFFFFF);
			sTestValues.insert(0xFF000000FFFFFFFF);
			sTestValues.insert(0xFFFF0000FFFFFFFF);
			sTestValues.insert(0x00000000FFFFFFFF);
			sTestValues.insert(0x00FF00FFFFFFFFFF);
			sTestValues.insert(0xFF00FF00FFFFFFFF);
			sTestValues.insert(0xAAAAAAAAFFFFFFFF);
			sTestValues.insert(0x55555555FFFFFFFF);
			sTestValues.insert(0x00FF00FF00FF00FF);
			sTestValues.insert(0xFF00FF00FF00FF00);
			sTestValues.insert(0xAAAAAAAAAAAAAAAA);
			sTestValues.insert(0x5555555555555555);
		}
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::Exchange_64");
			
			ExcFunc_64* excFunc[sNumThreads];
			AAX_CTestThread* excThread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				excFunc[i] = new ExcFunc_64(sTestInt, sTestValues, sNumIterationsPerThread);
			}
			for (int i = 0; i < sNumThreads; ++i)
			{
				excThread[i] = new AAX_CTestThread(excFunc[i]);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				excThread[i]->Finish();
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete excThread[i];
				delete excFunc[i];
			}
		}
	}
	
	TEST(CompareAndExchange_32)
	{
		typedef uint32_t TInt;
		
		static const int sNumThreads = 8;
		static const int sNumIterationsPerThread = 1 << 13;
		static TInt sTestInt = 0;
		
		static std::set<TInt> sTestValues;
		{
			sTestValues.insert(0x00000001);
			sTestValues.insert(0x80000000);
			sTestValues.insert(0xAAAAAAAA);
			sTestValues.insert(0x55555555);
		}
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::CompareAndExchange_32");
			
			CNXFunc_32* cnxFunc[sNumThreads];
			AAX_CTestThread* cnxThread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxFunc[i] = new CNXFunc_32(sTestInt, sTestValues, sNumIterationsPerThread);
			}
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxThread[i] = new AAX_CTestThread(cnxFunc[i]);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxThread[i]->Finish();
			}
			
			// Check results
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxFunc[i]->CheckResults();
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete cnxThread[i];
				delete cnxFunc[i];
			}
		}
	}
	
	TEST(CompareAndExchange_64)
	{
		typedef uint64_t TInt;
		
		static const int sNumThreads = 8;
		static const int sNumIterationsPerThread = 1 << 13;
		static TInt sTestInt = 0;
		
		static std::set<TInt> sTestValues;
		{
			sTestValues.insert(0x0000000000000001);
			sTestValues.insert(0x8000000000000000);
			sTestValues.insert(0xAAAAAAAAAAAAAAAA);
			sTestValues.insert(0x5555555555555555);
		}
		
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::CompareAndExchange_64");
			
			CNXFunc_64* cnxFunc[sNumThreads];
			AAX_CTestThread* cnxThread[sNumThreads];
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxFunc[i] = new CNXFunc_64(sTestInt, sTestValues, sNumIterationsPerThread);
			}
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxThread[i] = new AAX_CTestThread(cnxFunc[i]);
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxThread[i]->Finish();
			}
			
			// Check results
			for (int i = 0; i < sNumThreads; ++i)
			{
				cnxFunc[i]->CheckResults();
			}
			
			for (int i = 0; i < sNumThreads; ++i)
			{
				delete cnxThread[i];
				delete cnxFunc[i];
			}
		}
	}

	TEST(Exchange_And_Load_Ptr)
	{
#if (AAX_PointerSize == AAXPointer_64bit)
		typedef ExcFunc_64 TExcFunc;
#elif (AAX_PointerSize == AAXPointer_32bit)
		typedef ExcFunc_32 TExcFunc;
#else
#error Unsupported pointer size
#endif
		
		static const int sNumExchangeThreads = 2;
		static const int sNumLoadThreads = 2;
		static const int sNumIterationsPerThread = 1 << 18;
		static TUIntPtr sTestPtr = 0x0;
		
		static std::set<TUIntPtr> sTestValues;
		{
#if (AAX_PointerSize == AAXPointer_64bit)
			sTestValues.insert(0x0000000000000001);
			sTestValues.insert(0x00000000000000FF);
			sTestValues.insert(0x000000000000FFFF);
			sTestValues.insert(0x0000000080000000);
			sTestValues.insert(0x00000000FF000000);
			sTestValues.insert(0x00000000FFFF0000);
			sTestValues.insert(0x0000000000000000);
			sTestValues.insert(0x00000000FFFFFFFF);
			sTestValues.insert(0x0000000000FF00FF);
			sTestValues.insert(0x00000000FF00FF00);
			sTestValues.insert(0x00000000AAAAAAAA);
			sTestValues.insert(0x0000000055555555);
			sTestValues.insert(0x0000000100000000);
			sTestValues.insert(0x000000FF00000000);
			sTestValues.insert(0x0000FFFF00000000);
			sTestValues.insert(0x8000000000000000);
			sTestValues.insert(0xFF00000000000000);
			sTestValues.insert(0xFFFF000000000000);
			sTestValues.insert(0xFFFFFFFF00000000);
			sTestValues.insert(0x00FF00FF00000000);
			sTestValues.insert(0xFF00FF0000000000);
			sTestValues.insert(0xAAAAAAAA00000000);
			sTestValues.insert(0x5555555500000000);
			sTestValues.insert(0xFFFFFFFF00000001);
			sTestValues.insert(0xFFFFFFFF000000FF);
			sTestValues.insert(0xFFFFFFFF0000FFFF);
			sTestValues.insert(0xFFFFFFFF80000000);
			sTestValues.insert(0xFFFFFFFFFF000000);
			sTestValues.insert(0xFFFFFFFFFFFF0000);
			sTestValues.insert(0xFFFFFFFF00000000);
			sTestValues.insert(0xFFFFFFFFFFFFFFFF);
			sTestValues.insert(0xFFFFFFFF00FF00FF);
			sTestValues.insert(0xFFFFFFFFFF00FF00);
			sTestValues.insert(0xFFFFFFFFAAAAAAAA);
			sTestValues.insert(0xFFFFFFFF55555555);
			sTestValues.insert(0x00000001FFFFFFFF);
			sTestValues.insert(0x000000FFFFFFFFFF);
			sTestValues.insert(0x0000FFFFFFFFFFFF);
			sTestValues.insert(0x80000000FFFFFFFF);
			sTestValues.insert(0xFF000000FFFFFFFF);
			sTestValues.insert(0xFFFF0000FFFFFFFF);
			sTestValues.insert(0x00000000FFFFFFFF);
			sTestValues.insert(0x00FF00FFFFFFFFFF);
			sTestValues.insert(0xFF00FF00FFFFFFFF);
			sTestValues.insert(0xAAAAAAAAFFFFFFFF);
			sTestValues.insert(0x55555555FFFFFFFF);
			sTestValues.insert(0x00FF00FF00FF00FF);
			sTestValues.insert(0xFF00FF00FF00FF00);
			sTestValues.insert(0xAAAAAAAAAAAAAAAA);
			sTestValues.insert(0x5555555555555555);
#elif (AAX_PointerSize == AAXPointer_32bit)
			sTestValues.insert(0x00000001);
			sTestValues.insert(0x000000FF);
			sTestValues.insert(0x0000FFFF);
			sTestValues.insert(0x80000000);
			sTestValues.insert(0xFF000000);
			sTestValues.insert(0xFFFF0000);
			sTestValues.insert(0x00000000);
			sTestValues.insert(0xFFFFFFFF);
			sTestValues.insert(0x00FF00FF);
			sTestValues.insert(0xFF00FF00);
			sTestValues.insert(0xAAAAAAAA);
			sTestValues.insert(0x55555555);
#else
#error Unsupported pointer size
#endif
		}
		
		//
		// Actual test operation
		//
		
		{
			StTestTimer timer("AtomicOperationsTest::Exchange_And_Load_Ptr");
			
			TExcFunc* excFunc[sNumExchangeThreads];
			AAX_CTestThread* excThread[sNumExchangeThreads];
			LoadFunc* loadFunc[sNumLoadThreads];
			AAX_CTestThread* loadThread[sNumLoadThreads];
			for (int i = 0; i < std::max<int>(sNumExchangeThreads, sNumLoadThreads); ++i)
			{
				if (i < sNumExchangeThreads)
				{
					excFunc[i] = new TExcFunc(sTestPtr, sTestValues, sNumIterationsPerThread);
				}
				
				if (i < sNumLoadThreads)
				{
					loadFunc[i] = new LoadFunc(sTestPtr, sTestValues, sNumIterationsPerThread);
				}
			}
			
			for (int i = 0; i < std::max<int>(sNumExchangeThreads, sNumLoadThreads); ++i)
			{
				if (i < sNumExchangeThreads)
				{
					excThread[i] = new AAX_CTestThread(excFunc[i]);
				}
				
				if (i < sNumLoadThreads)
				{
					loadThread[i] = new AAX_CTestThread(loadFunc[i]);
				}
			}
			
			for (int i = 0; i < sNumExchangeThreads; ++i)
			{
				excThread[i]->Finish();
			}
			for (int i = 0; i < sNumLoadThreads; ++i)
			{
				loadThread[i]->Finish();
			}
			
			for (int i = 0; i < sNumLoadThreads; ++i)
			{
				loadFunc[i]->CheckResults();
			}
			
			for (int i = 0; i < sNumExchangeThreads; ++i)
			{
				delete excThread[i];
				delete excFunc[i];
			}
			for (int i = 0; i < sNumLoadThreads; ++i)
			{
				delete loadThread[i];
				delete loadFunc[i];
			}
		}
	}
}
