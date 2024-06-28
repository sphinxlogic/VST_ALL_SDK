/*================================================================================================*/
/*
 *	Copyright 2009-2015 by Avid Technology, Inc.
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
 *  \file   TaperDelegateTest.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/

#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_ITaperDelegate.h"
#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_CPieceWiseLinearTaperDelegate.h"

double utround(double number);
double utround(double number)
{
    return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

SUITE(TaperDelegateTest) 
{
	TEST(LinearFloatTaper)
    {
		const AAX_CLinearTaperDelegate<float>		linearFloatTaper(-23, 18);
		
		CHECK_EQUAL( -23, linearFloatTaper.GetMinimumValue() );
		CHECK_EQUAL( 18, linearFloatTaper.GetMaximumValue() );
		CHECK_EQUAL( -23, linearFloatTaper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 18, linearFloatTaper.ConstrainRealValue(100) );
		
		for (float v = -23.0f; v <= 18.0f; v+=0.1f)
		{		
			CHECK_CLOSE( v, linearFloatTaper.ConstrainRealValue(v), 0.001 );
			double normalizedValue = linearFloatTaper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			float outV = linearFloatTaper.NormalizedToReal(normalizedValue);
			CHECK_CLOSE( v, outV, 0.001 );
		}
	}
    
    TEST(LinearFloatTaperWithPrecision)
    {
		const AAX_CLinearTaperDelegate<float,10>		linearFloatTaper(-23, 18);
		
		CHECK_EQUAL( -23, linearFloatTaper.GetMinimumValue() );
		CHECK_EQUAL( 18, linearFloatTaper.GetMaximumValue() );
		CHECK_EQUAL( -23, linearFloatTaper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 18, linearFloatTaper.ConstrainRealValue(100) );
		
		for (float v = -23.0f; v <= 18.0f; v+=0.01f)
		{		
			double normalizedValue = linearFloatTaper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			float outV = linearFloatTaper.NormalizedToReal(normalizedValue);
			CHECK_CLOSE( utround(v*10)/10.0, outV, 0.001 );
		}
	}
	
	TEST(LinearFloatTaperEdgeCases)
	{
		const AAX_CLinearTaperDelegate<float>		linearFloatTaper(-23, 18);
		CHECK_EQUAL( -23, linearFloatTaper.NormalizedToReal(-10) );		//input normalized out of bounds
		CHECK_EQUAL( 18, linearFloatTaper.NormalizedToReal(100) );		//input normalized out of bounds
		CHECK_EQUAL( 1.0f, linearFloatTaper.RealToNormalized(1000) );		//input real is out of bounds
		CHECK_EQUAL( 0.0f, linearFloatTaper.RealToNormalized(-300) );		//input real is out of bounds
	}
	
	TEST(LinearDoubleTaper)
	{
		const AAX_CLinearTaperDelegate<double>		linearFloatTaper(-23, 18);
		
		CHECK_EQUAL( -23, linearFloatTaper.GetMinimumValue() );
		CHECK_EQUAL( 18, linearFloatTaper.GetMaximumValue() );
		CHECK_EQUAL( -23, linearFloatTaper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 18, linearFloatTaper.ConstrainRealValue(100) );
		
		for (double v = -23; v <= 18; v+=0.1)
		{		
			CHECK_CLOSE( v, linearFloatTaper.ConstrainRealValue(v), 0.001 );
			double normalizedValue = linearFloatTaper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			double outV = linearFloatTaper.NormalizedToReal(normalizedValue);
			CHECK_CLOSE( v, outV, 0.001 );
		}		
	}
	
	TEST(LinearDoubleTaperEdgeCases)
	{
		const AAX_CLinearTaperDelegate<double>		linearFloatTaper(-23, 18);
		CHECK_EQUAL( -23, linearFloatTaper.NormalizedToReal(-10) );		//input normalized out of bounds
		CHECK_EQUAL( 18, linearFloatTaper.NormalizedToReal(100) );		//input normalized out of bounds
		CHECK_EQUAL( 1.0f, linearFloatTaper.RealToNormalized(1000) );		//input real is out of bounds
		CHECK_EQUAL( 0.0f, linearFloatTaper.RealToNormalized(-300) );		//input real is out of bounds
	}
	
	TEST(LinearSignedIntegerTaper)
	{
		const AAX_CLinearTaperDelegate<int32_t,1000>		linearFloatTaper(-23, 18);
		
		CHECK_EQUAL( -23, linearFloatTaper.GetMinimumValue() );
		CHECK_EQUAL( 18, linearFloatTaper.GetMaximumValue() );
		CHECK_EQUAL( -23, linearFloatTaper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 18, linearFloatTaper.ConstrainRealValue(100) );
		
		for (int32_t v = -23; v <= 18; v+=1)
		{		
			CHECK_EQUAL( v, linearFloatTaper.ConstrainRealValue(v) );
			double normalizedValue = linearFloatTaper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			int32_t outV = linearFloatTaper.NormalizedToReal(normalizedValue);
			CHECK_EQUAL( v, outV );
		}		
	}
	
	TEST(LinearSignedIntegerTaperEdgeCases)
	{
		const AAX_CLinearTaperDelegate<int32_t>		linearFloatTaper(-23, 18);
		CHECK_EQUAL( -23, linearFloatTaper.NormalizedToReal(-10) );		//input normalized out of bounds
		CHECK_EQUAL( 18, linearFloatTaper.NormalizedToReal(100) );		//input normalized out of bounds
		CHECK_EQUAL( 1.0f, linearFloatTaper.RealToNormalized(1000) );		//input real is out of bounds
		CHECK_EQUAL( 0.0f, linearFloatTaper.RealToNormalized(-300) );		//input real is out of bounds
	}
	
	TEST(LinearUnsignedIntegerTaper)
	{
		const AAX_CLinearTaperDelegate<uint32_t>		linearTaper(5, 18);
		
		CHECK_EQUAL( uint32_t(5), linearTaper.GetMinimumValue() );
		CHECK_EQUAL( uint32_t(18), linearTaper.GetMaximumValue() );
		CHECK_EQUAL(uint32_t(5), linearTaper.ConstrainRealValue(2) );
		CHECK_EQUAL( uint32_t(18), linearTaper.ConstrainRealValue(100) );
		
		for (uint32_t v = 5; v <= 18; v+=1)
		{		
			CHECK_EQUAL( v, linearTaper.ConstrainRealValue(v) );
			double normalizedValue = linearTaper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			uint32_t outV = linearTaper.NormalizedToReal(normalizedValue);
			CHECK_EQUAL( v, outV );
		}		
	}
	
	TEST(LinearUnsignedIntegerTaperEdgeCases)
	{
		const AAX_CLinearTaperDelegate<uint32_t>		linearTaper(5, 18);
		CHECK_EQUAL( uint32_t(5), linearTaper.NormalizedToReal(0) );				//input normalized out of bounds
		CHECK_EQUAL( uint32_t(18), linearTaper.NormalizedToReal(100) );			//input normalized out of bounds
		CHECK_EQUAL( 1.0f, linearTaper.RealToNormalized(1000) );		//input real is out of bounds
		CHECK_EQUAL( 0.0f, linearTaper.RealToNormalized(1) );			//input real is out of bounds
	}
	
	TEST(LinearFloatIntegerPrecision)
	{
		AAX_CLinearTaperDelegate<float,1>	taper(-100, 100);
		double normValue = taper.RealToNormalized(10.1111f);
		float realValue = taper.NormalizedToReal(normValue); 
		CHECK_EQUAL( 10, realValue );
	}
	
	TEST(LinearFloatTensPrecision)
	{
		AAX_CLinearTaperDelegate<float,10>	taper(-100, 100);
		double normValue = taper.RealToNormalized(10.14f);
		float realValue = taper.NormalizedToReal(normValue); 
		CHECK_CLOSE( 10.1, realValue, 0.000001 );
	}

	TEST(BoolBinaryTaper)
	{
		AAX_CBinaryTaperDelegate<bool>	taper;
		double normValue = taper.RealToNormalized(true);
		CHECK_EQUAL( 1, normValue);
		normValue = taper.RealToNormalized(false);
		CHECK_EQUAL( 0, normValue);
		bool realValue = taper.NormalizedToReal(0);
		CHECK_EQUAL( false, realValue);
		realValue = taper.NormalizedToReal(0.1);
		CHECK_EQUAL( true, realValue);
		realValue = taper.NormalizedToReal(0.5);
		CHECK_EQUAL( true, realValue);
		realValue = taper.NormalizedToReal(1.0);
		CHECK_EQUAL( true, realValue);
		realValue = taper.NormalizedToReal(-1);		//out of bounds, but should still work
		CHECK_EQUAL( false, realValue);
		realValue = taper.NormalizedToReal(100);	//out of bounds, but should still work
		CHECK_EQUAL( true, realValue);
	}
	
	TEST(FloatSimplePieceWiseLinear)
	{
		const double normalized[] = {0.0, 1.0};
		const float real[] = {0.0f, 2.0f};
		AAX_CPieceWiseLinearTaperDelegate<float> taper(normalized, real, 2);
		
		CHECK_EQUAL( 0.0f, taper.GetMinimumValue() );
		CHECK_EQUAL( 2.0f, taper.GetMaximumValue() );
		CHECK_EQUAL( 0.0f, taper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 2.0f, taper.ConstrainRealValue(100) );
		
		//Normalized To Real
		for (double v = 0.0f; v <= 1.0f; v+=0.1f)
		{		
			float realValue = taper.NormalizedToReal(v);
			CHECK_CLOSE(2.0*v, realValue, 0.0001);
		}
		
		//Real to Normalized
		for (float v = 0.0f; v <= 2.0f; v+=0.1f)
		{		
			CHECK_CLOSE( v, taper.ConstrainRealValue(v), 0.001 );
			double normalizedValue = taper.RealToNormalized(v);
			CHECK(normalizedValue >= 0);
			CHECK(normalizedValue <= 1);
			CHECK_CLOSE(.5f*v, normalizedValue, 0.001);
		}
	}
	
	TEST(FloatComplexPieceWiseLinear)
	{
		const double normalized[] = {0.0, 0.25, 0.5, 1.0};
		const float real[] = {-10.0f, 0.0f, 4.0f, 9.0f};
		AAX_CPieceWiseLinearTaperDelegate<float> taper(normalized, real, 4);
		
		CHECK_EQUAL( -10.0f, taper.GetMinimumValue() );
		CHECK_EQUAL( 9.0f, taper.GetMaximumValue() );
		CHECK_EQUAL( -10.0f, taper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 9.0f, taper.ConstrainRealValue(100) );
		
		//Normalized To Real
		CHECK_CLOSE(-10.0f, taper.NormalizedToReal(-0.5), 0.1);		//outside of range should be clamped
		CHECK_CLOSE(-10.0f, taper.NormalizedToReal(0.0), 0.1);
		CHECK_CLOSE(-5.0f, taper.NormalizedToReal(0.125), 0.1);
		CHECK_CLOSE(2.0f, taper.NormalizedToReal(0.375), 0.1);
		CHECK_CLOSE(6.5f, taper.NormalizedToReal(0.75), 0.1);
		CHECK_CLOSE(9.0f, taper.NormalizedToReal(1.0), 0.1);
		CHECK_CLOSE(9.0f, taper.NormalizedToReal(2.0), 0.1);			//outside of range should be clamped

		//Real to Normalized
		CHECK_CLOSE(0.0, taper.RealToNormalized(-20.0f), 0.0001);		//outside of range should be clamped
		CHECK_CLOSE(0.0, taper.RealToNormalized(-10.0f), 0.0001);
		CHECK_CLOSE(0.125, taper.RealToNormalized(-5.0f), 0.0001);
		CHECK_CLOSE(0.375, taper.RealToNormalized(2.0f), 0.0001);
		CHECK_CLOSE(0.75, taper.RealToNormalized(6.5f), 0.0001);
		CHECK_CLOSE(1.0, taper.RealToNormalized(9.0f), 0.0001);
		CHECK_CLOSE(1.0, taper.RealToNormalized(20.0f), 0.0001);		//outside of range should be clamped
	}
	
	TEST(FloatInvertedComplexPieceWiseLinear)
	{
		const double normalized[] = {0.0, 0.25, 0.5, 1.0};
		const float real[] = {10.0f, 0.0f, -4.0f, -9.0f};
		AAX_CPieceWiseLinearTaperDelegate<float> taper(normalized, real, 4);
		
		CHECK_EQUAL( -9.0f, taper.GetMinimumValue() );
		CHECK_EQUAL( 10.0f, taper.GetMaximumValue() );
		CHECK_EQUAL( -9.0f, taper.ConstrainRealValue(-50) );
		CHECK_EQUAL( 10.0f, taper.ConstrainRealValue(100) );
		
		//Normalized To Real
		CHECK_CLOSE(10.0f, taper.NormalizedToReal(-0.5), 0.0001);		//outside of range should be clamped
		CHECK_CLOSE(10.0f, taper.NormalizedToReal(0.0), 0.0001);
		CHECK_CLOSE(5.0f, taper.NormalizedToReal(0.125), 0.0001);
		CHECK_CLOSE(-2.0f, taper.NormalizedToReal(0.375), 0.0001);
		CHECK_CLOSE(-6.5f, taper.NormalizedToReal(0.75), 0.0001);
		CHECK_CLOSE(-9.0f, taper.NormalizedToReal(1.0), 0.0001);
		CHECK_CLOSE(-9.0f, taper.NormalizedToReal(2.0), 0.0001);			//outside of range should be clamped
		
		//Real to Normalized
		CHECK_CLOSE(0.0, taper.RealToNormalized(20.0f), 0.0001);		//outside of range should be clamped
		CHECK_CLOSE(0.0, taper.RealToNormalized(10.0f), 0.0001);
		CHECK_CLOSE(0.125, taper.RealToNormalized(5.0f), 0.0001);
		CHECK_CLOSE(0.375, taper.RealToNormalized(-2.0f), 0.0001);
		CHECK_CLOSE(0.75, taper.RealToNormalized(-6.5f), 0.0001);
		CHECK_CLOSE(1.0, taper.RealToNormalized(-9.0f), 0.0001);
		CHECK_CLOSE(1.0, taper.RealToNormalized(-20.0f), 0.0001);		//outside of range should be clamped
	}
	
};

