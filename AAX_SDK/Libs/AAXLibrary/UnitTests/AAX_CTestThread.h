//        Copyright 2016 by Avid Technology, Inc.
//
//  AAX_CTestThread.h
//  AAXLibrary
//
//  Thread class used for AAX Library unit tests
//
//  Created by Rob Majors on 6/8/15.
//
//

#ifndef AAXLibrary_AAX_CTestThread_h
#define AAXLibrary_AAX_CTestThread_h

// Unit Test Includes
#include "Digi_UnitTest/Digi_UT_Include.h"


//------------------------------------------------------------
#pragma mark - Helper functions declaration

namespace AAXThread
{
	static void SleepMS(double inMilliseconds);
	static void SurrenderCPU();
}


//------------------------------------------------------------
#pragma mark - AAX_CTestThread declaration

class AAX_CTestThread
{
public:
	class Functor
	{
	public:
		virtual ~Functor() {}
		virtual void Run(void) = 0;
	};
	
public:
	explicit AAX_CTestThread(Functor* inFunc);
	~AAX_CTestThread();
	
public:
	void Finish();
	
private:
	class CImpl;
	CImpl* pImpl;
};



//------------------------------------------------------------
#pragma mark - AAX_CTestThread definition

// GCC/LLVM
#if defined(__GNUC__)

// Platform Includes
#include <unistd.h>
#include <pthread.h>

/* static */
void AAXThread::SleepMS(double inMilliseconds)
{
	usleep((useconds_t)(inMilliseconds * 1000.));
}

/* static */
void AAXThread::SurrenderCPU()
{
	pthread_yield_np();
}

class AAX_CTestThread::CImpl
{
public:
	explicit CImpl(Functor* inFunc)
	: mThreadID(0)
	, mFunc(inFunc)
	{
		pthread_attr_init(&mThreadAttr);
		pthread_attr_setdetachstate(&mThreadAttr, PTHREAD_CREATE_JOINABLE);
		
		const int rc = pthread_create(&mThreadID, &mThreadAttr, DoAction, mFunc);
		CHECK_EQUAL(0, rc);
	}
	
	~CImpl()
	{
		pthread_cancel(mThreadID);
		pthread_attr_destroy(&mThreadAttr);
	}
	
public:
	void Finish()
	{
		void* status = (void*)0;
		const int res = pthread_join(mThreadID, &status);
		CHECK_EQUAL(0, res);
		CHECK_EQUAL((void*)0, status);
	}
	
private:
	static void* DoAction(void* inData)
	{
		Functor* const func = reinterpret_cast<Functor*>(inData);
		func->Run();
		pthread_exit(0);
	}
	
private:
	pthread_t mThreadID;
	pthread_attr_t mThreadAttr;
	Functor* mFunc;
};


// Visual C
#elif defined(_MSC_VER)

// Platform Includes
#include <windows.h>
#include <process.h>

// AAX Includes
#include "AAX_Atomic.h"


/* static */
void AAXThread::SleepMS(double inMilliseconds)
{
	::Sleep((1 > inMilliseconds) ? 1 : (DWORD)inMilliseconds);
}

/* static */
void AAXThread::SurrenderCPU(void)
{
	SwitchToThread();
}


class AAX_CTestThread::CImpl
{
public:
	explicit CImpl(Functor* inFunc)
	: mThreadHandle(0)
	, mFunc(inFunc)
	{
		mThreadHandle = ::CreateThread(NULL, 0, &DoAction, mFunc, 0, NULL);
		CHECK(NULL != mThreadHandle);
	}
	
	~CImpl()
	{
		// Try exiting for several seconds
		const bool finished = FinishWithTimeout(1000 * 10);
		CHECK(true == finished);
		
		if (false == finished)
		{
			::TerminateThread(mThreadHandle, -1);
		}

		::CloseHandle(mThreadHandle);
	}
	
public:
	void Finish()
	{
		// No timeout
		const bool finishSuccess = FinishWithTimeout(0);
		CHECK_EQUAL(true, finishSuccess);
	}
	
private:
	static DWORD WINAPI DoAction(LPVOID inData)
	{
		Functor* const func = reinterpret_cast<Functor*>(inData);
		func->Run();
		//AAX_Atomic_Exchange_32(mDone, 1);
		return 0;
	}
	
	bool FinishWithTimeout(DWORD inTimeoutMilliseconds)
	{
		const DWORD waitResult = ::WaitForSingleObject(mThreadHandle, (0 == inTimeoutMilliseconds) ? INFINITE : inTimeoutMilliseconds);
		return (WAIT_OBJECT_0 == waitResult);

		//const long int sleepMilliseconds = 10L;
		//const long int numIterations = inTimeoutMilliseconds / sleepMilliseconds;
		//
		//if (0 < inTimeoutMilliseconds)
		//{
		//	for (int i = 0; (true == AAX_Atomic_CompareAndExchange_32(mDone, 0, 0)) && (i < numIterations); ++i)
		//	{
		//		::Sleep(sleepMilliseconds);
		//	}
		//}
		//else
		//{
		//	// No timeout
		//	while (true == AAX_Atomic_CompareAndExchange_32(mDone, 0, 0))
		//	{
		//		::Sleep(sleepMilliseconds);
		//	}
		//}
		//
		//return AAX_Atomic_CompareAndExchange_32(mDone, 1, 1);
	}

private:
	HANDLE mThreadHandle;
	Functor* mFunc;
};

#else // Not Visual C or GCC/LLVM
#error Provide an atomic operation implementation for this compiler
#endif // Compiler version check


// Compiler-independent function definitions

AAX_CTestThread::AAX_CTestThread(Functor* inFunc)
: pImpl(new CImpl(inFunc))
{
}

AAX_CTestThread::~AAX_CTestThread()
{
	delete pImpl;
}

void AAX_CTestThread::Finish()
{
	pImpl->Finish();
}



#endif // AAXLibrary_AAX_CTestThread_h
