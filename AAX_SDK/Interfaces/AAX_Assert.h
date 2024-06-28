/*================================================================================================*/
/*
 *
 *	Copyright 2013-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *	
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 *
 */

/**  
	\file AAX_Assert.h



	\brief Declarations for cross-platform AAX_ASSERT, AAX_TRACE and related facilities

 	\details
	Enabled using the DTF_AAXPLUGINS DigiTrace facility

	AAX_ASSERT( condition ) - Debug-only for host and TI.  If the condition is
		false, triggers a DigiTrace including the condition, file, and line numbers.

	AAX_TRACE( iPriority, iMessageStr [, params...] ) - Debug-only; traces
		a printf-style message.

	AAX_TRACE_RELEASE( iPriority, iMessageStr [, params...] ) - Debug and release; 
		traces a printf-style message.

		inPriority: one of 
			\li \ref kAAX_Trace_Priority_Low
			\li \ref kAAX_Trace_Priority_Normal
			\li \ref kAAX_Trace_Priority_High

		These correspond to how the trace messages are filtered using
		\ref AAX_DigiTrace_Guide "DigiTrace".
		
		\note Disabling the <TT>DTF_AAXPLUGINS</TT> facility will slightly reduce the
		overhead of trace statements and chip communication on HDX systems.

==============================================================================*/


#ifndef AAX_ASSERT_H
#define AAX_ASSERT_H

#include "AAX_Enums.h"


/** \def AAX_TRACE
	\brief Print a trace statement to the log (debug builds only)
 
	\details
	Use this macro to print a trace statement to the log file from debug builds of a plug-in.
	
	<B>Notes </B>
	- This macro will be compiled out of release builds
	- This macro is compatible with bost host and embedded (AAX DSP) environments
	- Subject to a total line limit of 256 chars
	
	<B>Usage </B>
	Each invocation of this macro takes a trace priority and a <TT>printf</TT>-style logging string. For
	example:
	
		\code
		AAX_TRACE(kAAX_Trace_Priority_Normal, "My float: %f, My C-string: %s", myFloat, myCString);
		\endcode
	
	\sa AAX_DigiTrace_Guide
 */

/** \def AAX_TRACE_RELEASE
	\brief Print a trace statement to the log
 
	\details
	Use this macro to print a trace statement to the log file. This macro will be included in all builds
	of the plug-in.
 
	<B>Notes </B>
	- This macro is compatible with bost host and embedded (AAX DSP) environments
	- Subject to a total line limit of 256 chars
	
	<B>Usage </B>
	Each invocation of this macro takes a trace priority and a <TT>printf</TT>-style logging string.
	
	Because output from this macro will be enabled on end users' systems under certain tracing configurations,
	logs should always be formatted with some standard information to avoid confusion between logs from
	different plug-ins. This is the recommended formatting for AAX_TRACE_RELEASE logs:
 
		<DIV CLASS="TextInd1"><TT>[Manufacturer name] [Plug-in name] [Plug-in version][logging text (indented)] </TT></DIV>
 
	For example:
 
		 \code
		 AAX_TRACE_RELEASE(kAAX_Trace_Priority_Normal, "%s %s %s;\tMy float: %f, My C-string: %s",
		     "MyCompany", "MyPlugIn", "1.0.2", myFloat, myCString);
		 \endcode
 
	\sa AAX_DigiTrace_Guide
 */

/** \def AAX_ASSERT
	\brief Asserts that a condition is true, and logs an error if the condition is false. Debug builds only.
	
	\details
	<B>Notes </B>
	- This macro will be compiled out of release builds.
	- This macro is compatible with bost host and embedded (AAX DSP) environments.

	<B>Usage </B>
	Each invocation of this macro takes a single argument, which is interpreted as a <TT>bool</TT>.

		\code
		AAX_ASSERT(desiredValue == variableUnderTest);
		\endcode
 */

/** \def AAX_STACKTRACE
 \brief Print a stack trace statement to the log (debug builds only)
 
 \sa \ref AAX_TRACE
 */

/** \def AAX_TRACEORSTACKTRACE
 \brief Print a trace statement with an optional stack trace to the log (debug builds only)
 
 \param[in] iTracePriority
 The log priority at which the trace statement will be printed
 \param[in] iStackTracePriority
 The log priority at which the stack trace will be printed
 
 \sa \ref AAX_TRACE
 */


#ifdef  _TMS320C6X	// TI-only

	#ifndef TI_SHELL_TRACING_H
	#include "TI_Shell_Tracing.h"
	#endif

	typedef AAX_ETracePriorityDSP EAAX_Trace_Priority;

	#define kAAX_Trace_Priority_None	AAX_eTracePriorityDSP_None
	#define kAAX_Trace_Priority_High	AAX_eTracePriorityDSP_High
	#define kAAX_Trace_Priority_Normal	AAX_eTracePriorityDSP_Normal
	#define kAAX_Trace_Priority_Low		AAX_eTracePriorityDSP_Low
	#define kAAX_Trace_Priority_Lowest	AAX_eTracePriorityDSP_Low

	//Note that the Message provided to AAX_TRACE must be a cons string available for indefinite time
	// because sending it to the host is done asynchronously.
	#define AAX_TRACE_RELEASE( ... ) TISHELLTRACE( __VA_ARGS__ )

	#define _STRINGIFY(x) #x
	#define _TOSTRING(x) _STRINGIFY(x)

	#if defined(_DEBUG)
		#define AAX_ASSERT( condition ) \
			{ \
				if( ! (condition) ) _DoTrace( AAX_eTracePriorityDSP_Assert, \
			         CAT(CAT( CAT(__FILE__, ":"), _TOSTRING(__LINE__) ) , CAT(" failed: ", #condition) ) );\
		    }
		
		#define AAX_TRACE( ... ) TISHELLTRACE( __VA_ARGS__ )
		
	#else		
		#define AAX_ASSERT( condition ) do { ; } while (0)
		#define AAX_TRACE( ... ) do { ; } while (0)
	#endif

	#define AAX_STACKTRACE( iPriority, ... ) do { ; } while (0)
	#define AAX_TRACEORSTACKTRACE( iPriority, ... ) do { ; } while (0)

#else // Host:

	#ifndef AAX_CHOSTSERVICES_H
	#include "AAX_CHostServices.h"
	#endif

	typedef AAX_ETracePriorityHost AAX_ETracePriority;

	#define kAAX_Trace_Priority_None		AAX_eTracePriorityHost_None
	#define kAAX_Trace_Priority_High		AAX_eTracePriorityHost_High
	#define kAAX_Trace_Priority_Normal		AAX_eTracePriorityHost_Normal
	#define kAAX_Trace_Priority_Low			AAX_eTracePriorityHost_Low
	#define kAAX_Trace_Priority_Lowest		AAX_eTracePriorityHost_Lowest


	#if defined(_DEBUG)
		#define AAX_ASSERT( condition ) \
			{ \
				if( ! ( condition ) ) { \
					AAX_CHostServices::Assert( __FILE__, __LINE__, #condition ); \
				} \
		    };
		
		#define AAX_TRACE( iPriority, ... ) \
			{ \
				AAX_CHostServices::Trace ( iPriority, __VA_ARGS__ ); \
		    };

		#define AAX_STACKTRACE( iPriority, ... ) \
		    { \
				AAX_CHostServices::StackTrace ( iPriority, iPriority, __VA_ARGS__ ); \
		    };

		#define AAX_TRACEORSTACKTRACE( iTracePriority, iStackTracePriority, ... ) \
		    { \
				AAX_CHostServices::StackTrace ( iTracePriority, iStackTracePriority, __VA_ARGS__ ); \
		    };

	#else
		#define AAX_ASSERT( condition ) do { ; } while (0)
		#define AAX_TRACE( iPriority, ... ) do { ; } while (0)
		#define AAX_STACKTRACE( iPriority, ... ) do { ; } while (0)
		#define AAX_TRACEORSTACKTRACE( iTracePriority, iStackTracePriority, ... ) do { ; } while (0)
	#endif

	//Note that the Message provided to AAX_TRACE must be a const string available for indefinite time
	// because sending it to the host is done asynchronously.
	#define AAX_TRACE_RELEASE( iPriority, ... ) \
		{ \
			AAX_CHostServices::Trace ( iPriority, __VA_ARGS__ ); \
		};

#endif 


#endif // include guard
// end of AAX_Assert.h
