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
 *  \file   DisplayDelegateTest.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/

#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_CNumberDisplayDelegate.h"
#include "AAX_CStringDisplayDelegate.h"
#include "AAX_CUnitDisplayDelegateDecorator.h"
#include "AAX_CUnitPrefixDisplayDelegateDecorator.h"
#include "AAX_CDecibelDisplayDelegateDecorator.h"
#include "AAX_CPercentDisplayDelegateDecorator.h"


SUITE(NumberDisplayDelegateTest) 
{
	TEST(FloatToStringSingleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 1>		numberDisplayDelegate;
		AAX_CString							valueString;
		
		// Test positive string equality
		numberDisplayDelegate.ValueToString(1.1f, &valueString);
		CHECK( strcmp("1.1", valueString.CString() ) == 0 );
		
		// Test rounding positive numbers to precision
		numberDisplayDelegate.ValueToString(100.255f, &valueString);
		CHECK( strcmp("100.3", valueString.CString() ) == 0 );

		// Test negative string equality
		numberDisplayDelegate.ValueToString(-5.0f, &valueString);
		CHECK( strcmp("-5.0", valueString.CString() ) == 0 );

		// Test rounding negative numbers to precision
		numberDisplayDelegate.ValueToString(-100.29f, &valueString);
		CHECK( strcmp("-100.3", valueString.CString() ) == 0 );

		// Test that positive median numbers round up correctly, rather than down
		numberDisplayDelegate.ValueToString(0.55f , &valueString);
		CHECK( strcmp("0.6", valueString.CString() ) == 0 );

		// Test that negative median numbers round up correctly, rather than down
		numberDisplayDelegate.ValueToString(-0.55f , &valueString);
		CHECK( strcmp("-0.6", valueString.CString() ) == 0 );
	}
	
	TEST(FloatToStringDoubleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 2>		numberDisplayDelegate;
		AAX_CString							valueString;
		
		numberDisplayDelegate.ValueToString(1.1f, &valueString);
		CHECK( strcmp("1.10", valueString.CString() ) == 0 );
		
		numberDisplayDelegate.ValueToString(100.256f, &valueString);
		CHECK( strcmp("100.26", valueString.CString() ) == 0 );

		numberDisplayDelegate.ValueToString(-5.0f, &valueString);
		CHECK( strcmp("-5.00", valueString.CString() ) == 0 );

		numberDisplayDelegate.ValueToString(-100.29f, &valueString);
		CHECK( strcmp("-100.29", valueString.CString() ) == 0 );

		// Test that positive median numbers round up correctly, rather than down
		numberDisplayDelegate.ValueToString(0.555f , &valueString);
		CHECK( strcmp("0.56", valueString.CString() ) == 0 );

		// Test that negative median numbers round up correctly, rather than down
		numberDisplayDelegate.ValueToString(-0.555f , &valueString);
		CHECK( strcmp("-0.56", valueString.CString() ) == 0 );
	}
	
	TEST(IntToString)
    {
		AAX_CNumberDisplayDelegate<int32_t, 0>		numberDisplayDelegate;
		AAX_CString							valueString;
		
		numberDisplayDelegate.ValueToString(1, &valueString);
		CHECK_EQUAL( "1", valueString.CString()  );
		
		numberDisplayDelegate.ValueToString(100, &valueString);
		CHECK_EQUAL( "100", valueString.CString() );

		numberDisplayDelegate.ValueToString(-5, &valueString);
		CHECK_EQUAL( "-5", valueString.CString() );

		numberDisplayDelegate.ValueToString(-100, &valueString);
		CHECK_EQUAL( "-100", valueString.CString()  );
	}
	
	TEST(SingleStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1>		numberDisplayDelegate;
		AAX_CString							valueString("1.1");
		float convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1.1f, convertedValue, 0.001f );
	}
	
	TEST(DoubleStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1>		numberDisplayDelegate;
		AAX_CString							valueString("-100.09");
		float convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( -100.09f, convertedValue, 0.001f );
	}
	
	TEST(StringToPositiveInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>			numberDisplayDelegate;
		AAX_CString							valueString("150");
		int32_t convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( 150, convertedValue );		
	}
	
	TEST(StringToNegativeInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>			numberDisplayDelegate;
		AAX_CString							valueString("-1");
		int32_t convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( -1, convertedValue );		
	}
	
	TEST(FloatStringToInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>			numberDisplayDelegate;
		AAX_CString							valueString("-50.4");
		int32_t convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( -50, convertedValue );		
	}
	
	TEST(SuffixStringToInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>		numberDisplayDelegate;
		AAX_CString							valueString("-1024 Hz");
		int32_t convertedValue = 0;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( -1024, convertedValue );		
	}
	
	TEST(BadStringToInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>		numberDisplayDelegate;
		AAX_CString							valueString("abc-1");
		int32_t convertedValue = -1;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
	
	TEST(BadStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1>		numberDisplayDelegate;
		AAX_CString							valueString("BadString");
		float convertedValue = -1;
		
		CHECK( numberDisplayDelegate.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
}	


		
SUITE(StringDisplayDelegateTest)
{
	TEST(IntToStringAndBack)
    {
		//Value String Map
		std::map<int32_t, AAX_CString>				stringMap;
		stringMap[-5] = "MinusFive";
		stringMap[0] = "Zero";
		stringMap[1] = "One";
		stringMap[2] = "Two";
		stringMap[3] = "Three";
		stringMap[4] = "Four";
		stringMap[5] = "Five";
		
		AAX_CStringDisplayDelegate<int32_t>			stringDisplayDelegate(stringMap);
		int32_t									value;
		AAX_CString							valueString;
				
		//value to string
		CHECK(stringDisplayDelegate.ValueToString(0, &valueString) );
		CHECK_EQUAL( "Zero", valueString.CString() );
		CHECK(stringDisplayDelegate.ValueToString(2, &valueString) );
		CHECK_EQUAL( "Two", valueString.CString() );
		CHECK(stringDisplayDelegate.ValueToString(4, &valueString) );
		CHECK_EQUAL( "Four", valueString.CString() );
		CHECK(stringDisplayDelegate.ValueToString(-5, &valueString) ); 
		CHECK_EQUAL( "MinusFive", valueString.CString() );
		
		//This is not in the map and should fail.
		CHECK(stringDisplayDelegate.ValueToString(-10, &valueString) == false);
		CHECK_EQUAL( "String Not Found", valueString.CString() );		
		
		//string to value
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString("Zero"), &value) );
		CHECK_EQUAL(0, value);
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString("One"), &value) );
		CHECK_EQUAL(1, value);
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString("Three"), &value) );
		CHECK_EQUAL(3, value);
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString("MinusFive"), &value) );
		CHECK_EQUAL(-5, value);
		
		
		//These aren't in the map and should just spit back 0.
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString("FiftyFour"), &value) == false);
		CHECK_EQUAL(0, value);
		
		CHECK(stringDisplayDelegate.StringToValue(AAX_CString(""), &value) == false);
		CHECK_EQUAL(0, value);
	}
}

		


SUITE(StandardUnitDisplayDelegateDecoratorTest)
{
	TEST(FloatToStringSingleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CString										unitString("Hz");
		AAX_CUnitDisplayDelegateDecorator<float>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString;
		
		unitDisplayDelegate.ValueToString(1.1f, &valueString);
		CHECK_EQUAL( "1.1 Hz", valueString.CString() );
			
		unitDisplayDelegate.ValueToString(100.255f, &valueString);
		CHECK_EQUAL( "100.3 Hz", valueString.CString() );

		unitDisplayDelegate.ValueToString(-5.0f, &valueString);
		CHECK_EQUAL( "-5.0 Hz", valueString.CString() );

		unitDisplayDelegate.ValueToString(-100.29f, &valueString);
		CHECK_EQUAL( "-100.3 Hz", valueString.CString() );
	}
	
	TEST(FloatToStringDoubleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 2, 1>		numberDisplayDelegate;
		AAX_CString										unitString("Hz");
		AAX_CUnitDisplayDelegateDecorator<float>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString;
		
		unitDisplayDelegate.ValueToString(1.1f, &valueString);
		CHECK_EQUAL( "1.10 Hz", valueString.CString() );
			
		unitDisplayDelegate.ValueToString(100.255f, &valueString);
		CHECK_EQUAL( "100.25 Hz", valueString.CString() );

		unitDisplayDelegate.ValueToString(-5.0f, &valueString);
		CHECK_EQUAL( "-5.00 Hz", valueString.CString() );

		unitDisplayDelegate.ValueToString(-100.29f, &valueString);
		CHECK_EQUAL( "-100.29 Hz", valueString.CString() );
	}
	
	TEST(IntToString)
    {
		AAX_CNumberDisplayDelegate<int32_t, 0, 1>						numberDisplayDelegate;
		AAX_CString										unitString("Cents");
		AAX_CUnitDisplayDelegateDecorator<int32_t>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString;
			
		unitDisplayDelegate.ValueToString(1, &valueString);
		CHECK_EQUAL( "1 Cents", valueString.CString()  );
		
		unitDisplayDelegate.ValueToString(100, &valueString);
		CHECK_EQUAL( "100 Cents", valueString.CString() );

		unitDisplayDelegate.ValueToString(-5, &valueString);
		CHECK_EQUAL( "-5 Cents", valueString.CString() );

		unitDisplayDelegate.ValueToString(-100, &valueString);
		CHECK_EQUAL( "-100 Cents", valueString.CString() );
	}
	
	TEST(SingleStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CString										unitString("Hz");
		AAX_CUnitDisplayDelegateDecorator<float>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("1.1 Hz");
		float convertedValue = 0;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1.1f, convertedValue, 0.001f );
	}
	
	TEST(ProblemStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CString										unitString("miles");		//problem because miles starts with m, which is also a prefix for milli.
		AAX_CUnitDisplayDelegateDecorator<float>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("-1.1 miles");
		float convertedValue = 0;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( -1.1f, convertedValue, 0.001f );
	}
	
	TEST(ProblemSpacedStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CString										unitString("miles");		//problem because miles starts with m, which is also a prefix for milli.
		AAX_CUnitDisplayDelegateDecorator<float>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("-1.1miles");	//problem because there is no space between the number and the units.
		float convertedValue = 0;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( -1.1f, convertedValue, 0.001f );
	}
	
	TEST(StringToPositiveInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CString										unitString("Cents");
		AAX_CUnitDisplayDelegateDecorator<int32_t>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("150 Cents");
		int32_t convertedValue = 0;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( 150, convertedValue );		
	}
	
	TEST(StringWithoutUnitsToNegativeInt)
	{
		//What if a user doesn't enter the units, it should detect this and still work.
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CString										unitString("C");
		AAX_CUnitDisplayDelegateDecorator<int32_t>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("150");
		int32_t convertedValue = 0;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( 150, convertedValue );			
	}
	
	TEST(BadStringToInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CString										unitString("C");
		AAX_CUnitDisplayDelegateDecorator<int32_t>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("abc-1");
		int32_t convertedValue = -1;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
	
	TEST(BadStringToFloat)
	{
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CString										unitString("C");
		AAX_CUnitDisplayDelegateDecorator<int32_t>		unitDisplayDelegate(numberDisplayDelegate, unitString);
		AAX_CString										valueString("BadString");
		int32_t convertedValue = -1;
		
		CHECK( unitDisplayDelegate.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
	

}



SUITE(UnitPrefixDisplayDelegateDecoratorTest)
{
	TEST(FloatToStringSingleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		prefixDecorator(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplay(prefixDecorator, AAX_CString("Hz"));
	
		AAX_CString										valueString;
		
		decoratedDisplay.ValueToString(1.1f, &valueString);
		CHECK_EQUAL( "1.1 Hz", valueString.CString() );
			
		decoratedDisplay.ValueToString(100.255f, &valueString);
		CHECK_EQUAL( "100.3 Hz", valueString.CString() );

		decoratedDisplay.ValueToString(1000.0f, &valueString);
		CHECK_EQUAL( "1.0 kHz", valueString.CString() );

		decoratedDisplay.ValueToString(1000000.0f, &valueString);
		CHECK_EQUAL( "1.0 MHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.050f, &valueString);
		CHECK_EQUAL( "50.0 mHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.0001f, &valueString);
		CHECK_EQUAL( "100.0 uHz", valueString.CString() );
	}
	
	TEST(IntToString)
    {
		AAX_CNumberDisplayDelegate<int32_t, 0, 1>						numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>				decoratedDisplay(numberDisplayDelegate, AAX_CString("Cents"));

		AAX_CString										valueString;
			
		decoratedDisplay.ValueToString(1, &valueString);
		CHECK_EQUAL( "1 Cents", valueString.CString()  );
		
		decoratedDisplay.ValueToString(1000, &valueString);
		CHECK_EQUAL( "1000 Cents", valueString.CString() );

		decoratedDisplay.ValueToString(-5, &valueString);
		CHECK_EQUAL( "-5 Cents", valueString.CString() );

		decoratedDisplay.ValueToString(-1000000, &valueString);
		CHECK_EQUAL( "-1000000 Cents", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, &valueString);
		CHECK_EQUAL( "0 Cents", valueString.CString() );
	}
	
	TEST(SingleStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		prefixDecorator(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplay(prefixDecorator, AAX_CString("Hz"));

		AAX_CString										valueString("1.1 Hz");
		float convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1.1f, convertedValue, 0.001f );
		
		valueString = AAX_CString("1.1 kHz");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1100.0f, convertedValue, 0.001f );

		valueString = AAX_CString("50.0 MHz");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 50000000, convertedValue, 0.001f );
		
		valueString = AAX_CString("35.1 mHz");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 0.0351f, convertedValue, 0.001f );
		
		valueString = AAX_CString("534.0 uHz");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 0.000534f, convertedValue, 0.001f );
		
		valueString = AAX_CString("53.5 Hz");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 53.5f, convertedValue, 0.001f );
	}
	
	TEST(ProblemStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		prefixDecorator(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplay(prefixDecorator, AAX_CString("miles"));

		AAX_CString										valueString("-1.1 miles");
		float convertedValue = 0;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( -1.1f, convertedValue, 0.001f );
	}
	
	TEST(ProblemSpacedStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		prefixDecorator(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplay(prefixDecorator, AAX_CString("miles"));

		AAX_CString										valueString("-1.1miles");	//problem because there is no space between the number and the units.
		float convertedValue = 0;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( -1.1f, convertedValue, 0.001f );
	}
	
	TEST(StringToPositiveInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>				decoratedDisplay(numberDisplayDelegate, AAX_CString("Cents"));

		AAX_CString										valueString("150 Cents");
		int32_t convertedValue = 0;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( 150, convertedValue );		
	}
	
	TEST(StringWithoutUnitsToNegativeInt)
	{
		//What if a user doesn't enter the units, it should detect this and still work.
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>				decoratedDisplay(numberDisplayDelegate, AAX_CString("C"));

		AAX_CString										valueString("150");
		int32_t convertedValue = 0;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_EQUAL( 150, convertedValue );			
	}
	
	TEST(BadStringToInt)
	{
		AAX_CNumberDisplayDelegate<int32_t>						numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>				decoratedDisplay(numberDisplayDelegate, AAX_CString("C"));

		AAX_CString										valueString("abc-1");
		int32_t convertedValue = -1;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
	
	TEST(BadStringToFloat)
	{
		AAX_CNumberDisplayDelegate<int32_t>				numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>		decoratedDisplay(numberDisplayDelegate, AAX_CString("C"));

		AAX_CString										valueString("BadString");
		int32_t convertedValue = -1;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
	
	TEST(NullStringConversion)
	{
		AAX_CNumberDisplayDelegate<int32_t>				numberDisplayDelegate;
		AAX_CUnitDisplayDelegateDecorator<int32_t>		decoratedDisplay(numberDisplayDelegate, AAX_CString("C"));

		AAX_CString										valueString("");
		int32_t convertedValue = -1;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 0, convertedValue );		
	}
}


SUITE(DecibelDisplayDelegateDecoratorTest)
{
	TEST(FloatToStringSingleFractional)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
	
		AAX_CString										valueString;
		
		decoratedDisplay.ValueToString(1.0f, &valueString);
		CHECK_EQUAL( "0.0 dB", valueString.CString() );
			
		decoratedDisplay.ValueToString(0, &valueString);
		CHECK_EQUAL( "-INF dB", valueString.CString() );

		decoratedDisplay.ValueToString(2, &valueString);
		CHECK_EQUAL( "6.0 dB", valueString.CString() );

		decoratedDisplay.ValueToString(10, &valueString);
		CHECK_EQUAL( "20.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, &valueString);
		CHECK_EQUAL( "60.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, &valueString);
		CHECK_EQUAL( "-40.0 dB", valueString.CString() );
	}
		
	TEST(SingleStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);

		AAX_CString										valueString("0.0 dB");
		float convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1, convertedValue, 0.001f );
		
		valueString = AAX_CString("6.0 dB");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1.995f, convertedValue, 0.001f );

		valueString = AAX_CString("60.0 dB");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1000, convertedValue, 0.001f );
		
		valueString = AAX_CString("20.0 dB");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 10, convertedValue, 0.001f );
		
		valueString = AAX_CString("-120 dB");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 0.000001, convertedValue, 0.001f );
		
		valueString = AAX_CString("-40.0 dB");
		convertedValue = 0;
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 0.01, convertedValue, 0.001f );
	}
	
	
	TEST(ProblemSpacedStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);

		AAX_CString										valueString("0.0dB");	//problem because there is no space between the number and the units.
		float convertedValue = 0;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == true );
		CHECK_CLOSE( 1, convertedValue, 0.001f );
	}
	
	
	TEST(BadStringToFloat)
	{
		AAX_CNumberDisplayDelegate<float, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);

		AAX_CString										valueString("BadString");
		float convertedValue = -1;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 1, convertedValue );		
	}
	
	TEST(NullStringConversion)
	{
		AAX_CNumberDisplayDelegate<float, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);

		AAX_CString										valueString("");
		float convertedValue = -1;
		
		CHECK( decoratedDisplay.StringToValue(valueString, &convertedValue) == false );
		CHECK_EQUAL( 1, convertedValue );		
	}
}


SUITE(ShortenedDecibelDisplayTest)
{
	TEST(ShortenedDecibelLong)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString										valueString;
		
		decoratedDisplay.ValueToString(1.0f, 10, &valueString);
		CHECK_EQUAL( "0.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, 10, &valueString);
		CHECK_EQUAL( "-INF dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(2, 10, &valueString);
		CHECK_EQUAL( "6.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(10, 10, &valueString);
		CHECK_EQUAL( "20.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, 10, &valueString);
		CHECK_EQUAL( "60.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, 10, &valueString);
		CHECK_EQUAL( "-40.0 dB", valueString.CString() );
	}
	
	TEST(ShortenedDecibelMedium)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>				numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString											valueString;
		
		decoratedDisplay.ValueToString(1.0f, 6, &valueString);
		CHECK_EQUAL( "0.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, 6, &valueString);
		CHECK_EQUAL( "-INF", valueString.CString() );
		
		decoratedDisplay.ValueToString(2, 6, &valueString);
		CHECK_EQUAL( "6.0 dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(10, 6, &valueString);
		CHECK_EQUAL( "20.0dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, 6, &valueString);
		CHECK_EQUAL( "60.0dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, 6, &valueString);
		CHECK_EQUAL( "-40.0", valueString.CString() );
	}
	
	
	TEST(ShortenedDecibelShort)
    {
		AAX_CNumberDisplayDelegate<float, 1, 1>				numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString											valueString;
		
		decoratedDisplay.ValueToString(1.0f, 4, &valueString);
		CHECK_EQUAL( "0.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, 4, &valueString);
		CHECK_EQUAL( "-INF", valueString.CString() );
		
		decoratedDisplay.ValueToString(2, 4, &valueString);
		CHECK_EQUAL( "6.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(10, 4, &valueString);
		CHECK_EQUAL( "20.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, 4, &valueString);
		CHECK_EQUAL( "60.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, 4, &valueString);
		CHECK_EQUAL( "-40", valueString.CString() );
	}
	
	TEST(ShortenedDecibelMediumHigherPrecision)
    {
		AAX_CNumberDisplayDelegate<float, 2, 1>				numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString											valueString;
		
		decoratedDisplay.ValueToString(1.0f, 6, &valueString);
		CHECK_EQUAL( "0.00dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, 6, &valueString);
		CHECK_EQUAL( "-INF", valueString.CString() );
		
		decoratedDisplay.ValueToString(2, 6, &valueString);
		CHECK_EQUAL( "6.02dB", valueString.CString() );
		
		decoratedDisplay.ValueToString(10, 6, &valueString);
		CHECK_EQUAL( "20.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, 6, &valueString);
		CHECK_EQUAL( "60.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, 6, &valueString);
		CHECK_EQUAL( "-40.00", valueString.CString() );
	}
	
	
	TEST(ShortenedDecibelShortHigherPrecision)
    {
		AAX_CNumberDisplayDelegate<float, 2, 1>				numberDisplayDelegate;
		AAX_CDecibelDisplayDelegateDecorator<float>			decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString											valueString;
		
		decoratedDisplay.ValueToString(1.0f, 4, &valueString);
		CHECK_EQUAL( "0.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, 4, &valueString);
		CHECK_EQUAL( "-INF", valueString.CString() );
		
		decoratedDisplay.ValueToString(2, 4, &valueString);
		CHECK_EQUAL( "6.02", valueString.CString() );
		
		decoratedDisplay.ValueToString(10, 4, &valueString);
		CHECK_EQUAL( "20.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, 4, &valueString);
		CHECK_EQUAL( "60.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(0.01f, 4, &valueString);
		CHECK_EQUAL( "-40", valueString.CString() );
	}
	
		
}


SUITE(ShortenedkHzTest)
{
	TEST(ShortenedkHzLong)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 10;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.0 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.0 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500.0 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.0 kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.0 kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10.0 kHz", valueString.CString() );
	}

	TEST(ShortenedkHzMedium)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		 
		AAX_CString			valueString;
		const int32_t		cNumChars = 6;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.0 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.0 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.0kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.0kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10.0k", valueString.CString() );
	}


	TEST(ShortenedkHzShort)
	{
		AAX_CNumberDisplayDelegate<float, 1, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 4;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.0", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.0k", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.0k", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10k", valueString.CString() );
	}
	
	TEST(HighPrecisionShortenedkHzLong)
	{
		AAX_CNumberDisplayDelegate<float, 2, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 10;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.00 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.00 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500.00 Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.00 kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.00 kHz", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10.00 kHz", valueString.CString() );
	}
	
	TEST(HighPrecisionShortenedkHzMedium)
	{
		AAX_CNumberDisplayDelegate<float, 2, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 6;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.00Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.00Hz", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.00k", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.00k", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10.00k", valueString.CString() );
	}
	
	
	TEST(HighPrecisionShortenedkHzShort)
	{
		AAX_CNumberDisplayDelegate<float, 2, 1>					numberDisplayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>			prefixDecoratedDisplay(numberDisplayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>				decoratedDisplay(prefixDecoratedDisplay, AAX_CString("Hz"));
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 4;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "1.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.00", valueString.CString() );
		
		decoratedDisplay.ValueToString(500, cNumChars, &valueString);
		CHECK_EQUAL( "500", valueString.CString() );
		
		decoratedDisplay.ValueToString(1000, cNumChars, &valueString);
		CHECK_EQUAL( "1.0k", valueString.CString() );
		
		decoratedDisplay.ValueToString(5000, cNumChars, &valueString);
		CHECK_EQUAL( "5.0k", valueString.CString() );
		
		decoratedDisplay.ValueToString(10000, cNumChars, &valueString);
		CHECK_EQUAL( "10k", valueString.CString() );
	}
}


SUITE(ShortenedPercentageTest)
{
	TEST(ShortenedPercentageLong)
	{
		AAX_CNumberDisplayDelegate<float, 2, 0>					numberDisplayDelegate;
		AAX_CPercentDisplayDelegateDecorator<float>				decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 8;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "100.00%", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.00%", valueString.CString() );
		
		decoratedDisplay.ValueToString(5, cNumChars, &valueString);
		CHECK_EQUAL( "500.00%", valueString.CString() );
		
		decoratedDisplay.ValueToString(-.5f, cNumChars, &valueString);
		CHECK_EQUAL( "-50.00%", valueString.CString() );
	}
	
	TEST(ShortenedPercentageMedium)
	{
		AAX_CNumberDisplayDelegate<float, 2, 0>					numberDisplayDelegate;
		AAX_CPercentDisplayDelegateDecorator<float>				decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 6;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "100.0%", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.00%", valueString.CString() );
		
		decoratedDisplay.ValueToString(5, cNumChars, &valueString);
		CHECK_EQUAL( "500.0%", valueString.CString() );
		
		decoratedDisplay.ValueToString(-.5f, cNumChars, &valueString);
		CHECK_EQUAL( "-50.0%", valueString.CString() );
	}
	
	TEST(ShortenedPercentageShort)
	{
		AAX_CNumberDisplayDelegate<float, 2, 0>					numberDisplayDelegate;
		AAX_CPercentDisplayDelegateDecorator<float>				decoratedDisplay(numberDisplayDelegate);
		
		AAX_CString			valueString;
		const int32_t		cNumChars = 4;
		
		decoratedDisplay.ValueToString(1.0f, cNumChars, &valueString);
		CHECK_EQUAL( "100%", valueString.CString() );
		
		decoratedDisplay.ValueToString(0, cNumChars, &valueString);
		CHECK_EQUAL( "0.0%", valueString.CString() );
		
		decoratedDisplay.ValueToString(5, cNumChars, &valueString);
		CHECK_EQUAL( "500%", valueString.CString() );
		
		decoratedDisplay.ValueToString(-.5f, cNumChars, &valueString);
		CHECK_EQUAL( "-50%", valueString.CString() );
	}
}

		
