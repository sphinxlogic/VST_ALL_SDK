/*================================================================================================*/
/*
 *	Copyright 2014-2015 by Avid Technology, Inc.
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
 *	\file  AAX_Denormal.h
 *
 *	\brief Signal processing utilities for denormal/subnormal floating point numbers
 *
 */ 
/*================================================================================================*/
#pragma once

#ifndef AAX_DENORMAL_H
#define AAX_DENORMAL_H

// AAX Includes
#include "AAX.h"
#include "AAX_PlatformOptimizationConstants.h"

#if ! defined( _TMS320C6X )
#include <limits>
#endif // ! defined( _TMS320C6X )


/** \def AAX_SCOPE_DENORMALS_AS_ZERO()
 Sets the run-time environment to treat denormal floats as zero within the scope of this macro
 */

#if defined( _TMS320C6X )

// TODO: Currently noop on TI
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#elif ((defined _MSC_VER) || defined(WINDOWS_VERSION))
// Visual Studio

// In Pro Tools 11, DAZ is turned on for all plug-in processing threads
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#elif defined(LINUX_VERSION)

#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#elif (defined (__GNUC__) || defined(MAC_VERSION))
// GCC / OS X

// Denormals are treated as zero within this macro's scope
#define AAX_SCOPE_DENORMALS_AS_ZERO() AAX_StDenormalAsZeroFlags denormalAsZeroFlagsScope ## __LINE__ ; do {} while (0)

#include <fenv.h>
struct AAX_StDenormalAsZeroFlags
{
public:
	AAX_StDenormalAsZeroFlags() { set_flags(); }
	~AAX_StDenormalAsZeroFlags() { reset_flags(); }
	
private:
	void set_flags() { fegetenv(&mPrevFlags); fesetenv(FE_DFL_DISABLE_SSE_DENORMS_ENV); }
	void reset_flags() { fesetenv(&mPrevFlags); }
	fenv_t mPrevFlags;
};



#else

#error AAX_SCOPE_DENORMALS_AS_ZERO is not defined for this compiler

// Just for Doxygen
#define AAX_SCOPE_DENORMALS_AS_ZERO() do {} while(0)

#endif // End compiler selection for AAX_SCOPE_DENORMALS_AS_ZERO definition


//#if ((defined _MSC_VER) && (_MSC_VER < 1800))
//// Pre-2013 Visual Studio
//
//#define _MM_FUNCTIONALITY
//#include <intrin.h>
//struct AAX_StDenormalAsZeroFlags
//{
//public:
//	AAX_StDenormalAsZeroFlags() { set_flags(); }
//	~AAX_StDenormalAsZeroFlags() { reset_flags(); }
//	
//private:
//	void set_flags()
//	{
//		// Sets the "denormals as zero" and "flush to zero" bits for SSE
//		static const int cDenormalBits = (1L << 6) | (1L << 15);
//		mOldMXCSR = _mm_getcsr(); // read the old MXCSR setting
//		_mm_setcsr( mOldMXCSR | cDenormalBits );	 // write an MXCSR setting with DAZ and FZ bits to the MXCSR
//	}
//	
//	void reset_flags() { _mm_setcsr(mOldMXCSR); }
//	int mOldMXCSR;
//};
//
//#endif



namespace AAX
{
	const double cDenormalAvoidanceOffset=3.0e-34;
	const float cFloatDenormalAvoidanceOffset=3.0e-20f;	//This number is emprically derived with minimal trial and error.

#if ! defined( _TMS320C6X )
	static const float cMinimumNormalFloat = std::numeric_limits<float>::min();
#endif
	
	/*!
	 *  \brief Clamps very small floating point values to zero
	 *
	 *  \details On Pentiums and Pentium IIs the generation of denormal floats causes enormous
	 *	performance losses. This routine removes denormals by clamping very small values to
	 *	zero. The clamping threshold is very small, but is not the absolute minimum. If
	 *  absolute minimum clamping is desired, use \ref AAX::DeDenormalFine()
	 *
	 */
	inline void DeDenormal(double &iValue)
	{
#if defined(WINDOWS_VERSION) || defined(MAC_VERSION)
		if(iValue < cDenormalAvoidanceOffset && iValue > -cDenormalAvoidanceOffset) iValue=0.0;
#endif
	}
	
	/*! \copydoc AAX::DeDenormal(double&)
	 */
	inline void DeDenormal(float &iValue)
	{
#if defined(WINDOWS_VERSION) || defined(MAC_VERSION)
		if(iValue < cFloatDenormalAvoidanceOffset && iValue > -cFloatDenormalAvoidanceOffset) iValue=0.0;
#endif 
	}
	
	/*!
	 * Similar to \ref AAX::DeDenormal(), but uses the minimum possible normal float value as
	 * the clamping threshold
	 */
	inline void DeDenormalFine(float &iValue)
	{
#if ! defined( _TMS320C6X )
		if(iValue < cMinimumNormalFloat && iValue > -cMinimumNormalFloat) iValue=0.0f;
#endif
	}
	
	/*!
	 *  \brief Round all denormal/subnormal samples in a buffer to zero
	 *
	 *
	 *  \param[in] inSamples
	 *		Samples to convert
	 *  \param[in] inLength
	 *		Number of samples in inSamples
	 *
	 */
	inline void FilterDenormals(float* inSamples, int32_t inLength)
	{
		//TODO: Implement optimized versions for TI and SSE
#if ! defined( _TMS320C6X ) // Not yet implemented for TI
		for( int32_t i = 0; i < inLength; ++i )
		{
			float curFloat = *inSamples;
			if( fabsf(curFloat) < cMinimumNormalFloat )
				curFloat = 0.0f;
			*(inSamples++) = curFloat;
		}
#endif
	}

} // namespace AAX

#endif // AAX_QUANTIZE_H
