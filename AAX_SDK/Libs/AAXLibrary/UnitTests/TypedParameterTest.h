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
 *  \file   TypedParameterTest.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/

#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_CParameter.h"
#include "AAX_CBinaryDisplayDelegate.h"


SUITE(FloatTypedParameterTest) 
{	
	TEST(NameAndIdentifier)
    {
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 0,  taper, display);
		
		CHECK_EQUAL( "Linearparameter", param.Name().CString() );
		CHECK_EQUAL( "1", param.Identifier() );
		
		param.SetName(AAX_CString("NewName"));
		CHECK_EQUAL( "NewName", param.Name().CString() );
	}
	
	TEST(ShortenedName)
    {
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("LongName"), 0,  taper, display);
		
		//Before setting shortened names, it should always return long name.
		CHECK_EQUAL( "LongName", param.ShortenedName(12).CString() );
		CHECK_EQUAL( "LongName", param.ShortenedName(8).CString() );
		CHECK_EQUAL( "LongName", param.ShortenedName(4).CString() );
		CHECK_EQUAL( "LongName", param.ShortenedName(0).CString() );

		//Set a single shortened names...
		param.AddShortenedName(AAX_CString("LongNm"));
		CHECK_EQUAL( "LongName", param.ShortenedName(8).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(7).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(6).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(5).CString() );		//This should still return the shortest name available...  (edge case)
		CHECK_EQUAL( "LongNm", param.ShortenedName(0).CString() );		//This should still return the shortest name available...  (edge case)

		//Set a single shortened names...
		param.AddShortenedName(AAX_CString("LngNm"));
		CHECK_EQUAL( "LongName", param.ShortenedName(8).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(7).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(6).CString() );
		CHECK_EQUAL( "LngNm", param.ShortenedName(5).CString() );		
		CHECK_EQUAL( "LngNm", param.ShortenedName(0).CString() );		//This should still return the shortest name available...  (edge case)
		
		//Set a single shortened names...
		param.AddShortenedName(AAX_CString("L"));
		CHECK_EQUAL( "L", param.ShortenedName(0).CString() );		//This should still return the shortest name available...  (edge case)
		CHECK_EQUAL( "L", param.ShortenedName(1).CString() );		
		CHECK_EQUAL( "L", param.ShortenedName(2).CString() );		
		CHECK_EQUAL( "LngNm", param.ShortenedName(5).CString() );		
		CHECK_EQUAL( "LongNm", param.ShortenedName(6).CString() );
		CHECK_EQUAL( "LongNm", param.ShortenedName(7).CString() );
		CHECK_EQUAL( "LongName", param.ShortenedName(8).CString() );		
		CHECK_EQUAL( "LongName", param.ShortenedName(9).CString() );		
		
		
		param.SetName(AAX_CString("NewName"));
		param.ClearShortenedNames();

		//Before setting shortened names, it should always return long name.
		CHECK_EQUAL( "NewName", param.ShortenedName(12).CString() );
		CHECK_EQUAL( "NewName", param.ShortenedName(8).CString() );
		CHECK_EQUAL( "NewName", param.ShortenedName(4).CString() );
		CHECK_EQUAL( "NewName", param.ShortenedName(0).CString() );
	}
	
	
	TEST(InitializedValue)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10, taper, display);
		
		CHECK_EQUAL( 10, param.GetValue() );
	}
	
	TEST(SetValueFunction)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10, taper, display);
		
		//Check many valid values within range.
		CHECK_EQUAL( 10, param.GetValue() );
		for (float newValue = -100; newValue < 100; newValue += 0.1f)
		{
			param.SetValue(newValue);
			CHECK_CLOSE(newValue, param.GetValue(), 0.001f);
		}
		
		//Check outside of bounds setting.
		param.SetValue(-1000);
		CHECK_EQUAL( -100, param.GetValue());
		param.SetValue(1000);
		CHECK_EQUAL(100, param.GetValue());
	}
	
	TEST(SetToDefaultValueFunction)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10, taper, display);
		
		param.SetValue(25);
		CHECK_EQUAL( 25, param.GetValue() );
		param.SetToDefaultValue();
		CHECK_EQUAL( 10, param.GetValue() );
	}

	TEST(SetDefaultValueFunction)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10, taper, display);
		
		CHECK_EQUAL( 10, param.GetDefaultValue() );
		param.SetValue(25);
		param.SetToDefaultValue();
		CHECK_EQUAL( 10, param.GetValue() );
		param.SetDefaultValue(55);
		param.SetToDefaultValue();
		CHECK_EQUAL( 55, param.GetValue() );
		CHECK_EQUAL( 55, param.GetDefaultValue() );

		//Check negative out of bounds default values.
		param.SetDefaultValue(-200);
		CHECK_EQUAL( -100, param.GetDefaultValue() );	//default should clamp to the bounds.
		param.SetToDefaultValue();						//This should set to the clamped value.
		CHECK_EQUAL( -100, param.GetValue() );

		//Check positive out of bounds default values.
		param.SetDefaultValue(200);
		CHECK_EQUAL( 100, param.GetDefaultValue() );	//default should clamp to the bounds.
		param.SetToDefaultValue();						//This should set to the clamped value.
		CHECK_EQUAL( 100, param.GetValue() );
	}

	TEST(SetNormalizedDefaultValueFunction)
	{
		AAX_CLinearTaperDelegate<float>	taper(0, 10);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		typedParam("1", AAX_CString("Linearparameter"), 5, taper, display);
		AAX_IParameter*	param = static_cast<AAX_IParameter*>(&typedParam);
		
		CHECK_EQUAL( 0.5, param->GetNormalizedDefaultValue() );
		param->SetNormalizedValue(1.0);
		param->SetToDefaultValue();
		CHECK_EQUAL( 0.5, param->GetNormalizedValue() );
		param->SetNormalizedDefaultValue(0.9);
		param->SetToDefaultValue();
		CHECK_EQUAL( 0.9, param->GetNormalizedValue() );
		CHECK_EQUAL( 0.9, param->GetNormalizedDefaultValue() );

		//Check negative out of bounds default values.
		param->SetNormalizedDefaultValue(-2);
		CHECK_EQUAL( 0.0, param->GetNormalizedDefaultValue() );	//default should clamp to the bounds.
		param->SetToDefaultValue();						//This should set to the clamped value.
		CHECK_EQUAL( 0.0, param->GetNormalizedValue() );

		//Check positive out of bounds default values.
		param->SetNormalizedDefaultValue(2);
		CHECK_EQUAL( 1.0, param->GetNormalizedDefaultValue() );	//default should clamp to the bounds.
		param->SetToDefaultValue();						//This should set to the clamped value.
		CHECK_EQUAL( 1.0, param->GetNormalizedValue() );
	}
	
	TEST(NumberOfSteps)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"),0, taper, display);
		
		CHECK_EQUAL(uint32_t(AAX_CParameter<float>::eParameterDefaultNumStepsContinuous), param.GetNumberOfSteps());
		param.SetNumberOfSteps(10);
		CHECK_EQUAL(uint32_t(10), param.GetNumberOfSteps());
		
	}
	
	TEST(NormalizedValueFunctions)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"),0, taper, display);
		
		//Check many valid values within range.
		CHECK_EQUAL( 0.5, param.GetNormalizedValue() );
		for (float newNormalizedValue = 0; newNormalizedValue < 1; newNormalizedValue += 0.01f)
		{
			param.SetNormalizedValue(newNormalizedValue);
			CHECK_CLOSE(newNormalizedValue, param.GetNormalizedValue(), 0.001f);
		}
		
		//Check outside of bounds setting.
		param.SetNormalizedValue(-1);
		CHECK_EQUAL( 0, param.GetNormalizedValue() );
		CHECK_EQUAL( -100, param.GetValue());
		param.SetNormalizedValue(3);
		CHECK_EQUAL( 1, param.GetNormalizedValue() );
		CHECK_EQUAL(100, param.GetValue());		
	
	}
		
	TEST(IntegerPrecision)
	{
		AAX_CLinearTaperDelegate<int32_t, 1>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<int32_t> display;
		AAX_CParameter<int32_t>		param("1", AAX_CString("Linearparameter"), 0, taper, display);
		param.SetValue(24);
		CHECK_EQUAL( 24, param.GetValue() );
		param.SetValue(int32_t(-1.5));
		CHECK_EQUAL( -1, param.GetValue() );
		param.SetValue(int32_t(1.5));
		CHECK_EQUAL(1, param.GetValue() );
		param.SetValue(int32_t(15.00000005));
		CHECK_EQUAL(15, param.GetValue() );
		
		//Through Interface
		AAX_IParameter*		iparam = dynamic_cast<AAX_IParameter*> (&param);
		iparam->SetValueWithInt32(24);
		int32_t value;
		CHECK_EQUAL(true, iparam->GetValueAsInt32(&value));
		CHECK_EQUAL( 24, value );
		CHECK_EQUAL(true, iparam->SetValueWithInt32(int32_t(-1.5)));
		CHECK_EQUAL(true, iparam->GetValueAsInt32(&value));
		CHECK_EQUAL( -1, value );
		CHECK_EQUAL(true, iparam->SetValueWithInt32(int32_t(1.5)));
		CHECK_EQUAL(true, iparam->GetValueAsInt32(&value));
		CHECK_EQUAL( 1, value );
		CHECK_EQUAL(true, iparam->SetValueWithInt32(int32_t(15.00000005)));
		CHECK_EQUAL(true, iparam->GetValueAsInt32(&value));
		CHECK_EQUAL( 15, value );
		
		//Wrong Type through interface (should return false that the call didn't succeed)
		bool bValue;
		float fValue;
		CHECK_EQUAL(false, iparam->SetValueWithBool(true));
		CHECK_EQUAL(false, iparam->SetValueWithFloat(1.0f));
		CHECK_EQUAL(false, iparam->GetValueAsBool(&bValue));
		CHECK_EQUAL(false, iparam->GetValueAsFloat(&fValue));
	}
	
	TEST(FloatPrecision)
	{
		AAX_CLinearTaperDelegate<float, 10>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float>	display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 0, taper, display);
		param.SetValue(24.167f);
		CHECK_CLOSE( 24.2, param.GetValue(), 0.001 );
		param.SetValue(-0.5111f);
		CHECK_CLOSE( -.5, param.GetValue(), 0.001 );
		param.SetValue(0.49f);
		CHECK_CLOSE(.5, param.GetValue(), 0.001 );
		param.SetValue(15.00000005f);
		CHECK_CLOSE(15, param.GetValue(), 0.001 );
		
		//Through Interface
		AAX_IParameter*		iparam = dynamic_cast<AAX_IParameter*> (&param);
		iparam->SetValueWithFloat(24.167f);
		float value;
		CHECK_EQUAL(true, iparam->GetValueAsFloat(&value));
		CHECK_CLOSE( 24.2,  value , 0.001);
		CHECK_EQUAL(true, iparam->SetValueWithFloat(-0.5111f));
		CHECK_EQUAL(true, iparam->GetValueAsFloat(&value));
		CHECK_CLOSE( -.5, value, 0.001 );
		CHECK_EQUAL(true, iparam->SetValueWithFloat(.49f));
		CHECK_EQUAL(true, iparam->GetValueAsFloat(&value));
		CHECK_CLOSE( .5, value , 0.001);
		CHECK_EQUAL(true, iparam->SetValueWithFloat(15.00000005f));
		CHECK_EQUAL(true, iparam->GetValueAsFloat(&value));
		CHECK_CLOSE( 15, value, 0.001 );
		
		//Wrong Type through interface (should return false that the call didn't succeed)
		bool bValue;
		int32_t iValue;
		CHECK_EQUAL(false, iparam->SetValueWithBool(true));
		CHECK_EQUAL(false, iparam->SetValueWithInt32(1));
		CHECK_EQUAL(false, iparam->GetValueAsBool(&bValue));
		CHECK_EQUAL(false, iparam->GetValueAsInt32(&iValue));
		
	}
	
	TEST(BoolFunctions)
	{
		AAX_CBinaryTaperDelegate<bool>	taper;
		AAX_CBinaryDisplayDelegate<bool>	display("false", "true");
		AAX_CParameter<bool>		param("1", AAX_CString("BoolParameter"), false, taper, display);
		param.SetValue(true);
		CHECK_EQUAL( true, param.GetValue() );
		param.SetValue(false);
		CHECK_EQUAL( false, param.GetValue() );
		
		//Through Interface
		AAX_IParameter*		iparam = dynamic_cast<AAX_IParameter*> (&param);
		bool value;
		CHECK_EQUAL(true, iparam->SetValueWithBool(true));
		CHECK_EQUAL(true, iparam->GetValueAsBool(&value));
		CHECK_EQUAL( true, value );
		CHECK_EQUAL(true, iparam->SetValueWithBool(false));
		CHECK_EQUAL(true, iparam->GetValueAsBool(&value));
		CHECK_EQUAL( false, value );
		
		//Wrong Type through interface (should return false that the call didn't succeed)
		int32_t iValue;
		float fValue;
		CHECK_EQUAL(false, iparam->SetValueWithInt32(1));
		CHECK_EQUAL(false, iparam->SetValueWithFloat(1.0f));
		CHECK_EQUAL(false, iparam->GetValueAsInt32(&iValue));
		CHECK_EQUAL(false, iparam->GetValueAsFloat(&fValue));
		
	}
	
	
	TEST(FloatToStringAndBack)
	{
		AAX_CLinearTaperDelegate<float>			taperDelegate(-50, 30);
		AAX_CNumberDisplayDelegate<float, 100>	displayDelegate;
		AAX_CParameter<float>				param("1", AAX_CString("Linearparameter"), 10, taperDelegate, displayDelegate);
		
		//Check many valid values within range and outside of range.
		for (float newValue = -100; newValue < 100; newValue += 2.1f)
		{
			param.SetValue(newValue);
			AAX_CString	valueString;
			CHECK(param.GetValueString(&valueString));
			param.SetToDefaultValue();		//make sure to change it in between.
			CHECK(param.SetValueFromString(valueString));
			if (newValue < -50)
				CHECK_EQUAL(-50, param.GetValue());
			else if (newValue > 30)
				CHECK_EQUAL(30, param.GetValue());
			else
			CHECK_CLOSE(newValue, param.GetValue(), 0.001f);
		}
	}
	
	TEST(FrequecyToStringAndBack)
	{
		AAX_CLinearTaperDelegate<float>						taperDelegate(-50, 30);
		AAX_CNumberDisplayDelegate<float, 100>				displayDelegate;
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplayDelegate(displayDelegate, "Hz");
		AAX_CParameter<float>				param("1", AAX_CString("Linearparameter"), 10, taperDelegate, decoratedDisplayDelegate);
		
		//Check many valid values within range and outside of range.
		for (float newValue = -100; newValue < 100; newValue += 4.6f)
		{
			param.SetValue(newValue);
			AAX_CString	valueString;
			CHECK(param.GetValueString(&valueString));
			param.SetToDefaultValue();		//make sure to change it in between.
			CHECK(param.SetValueFromString(valueString));
			if (newValue < -50)
				CHECK_EQUAL(-50, param.GetValue());
			else if (newValue > 30)
				CHECK_EQUAL(30, param.GetValue());
			else
			CHECK_CLOSE(newValue, param.GetValue(), 0.001f);
		}
	}
	
	TEST(FrequecyLargeRangeToStringAndBack)
	{
		AAX_CLinearTaperDelegate<float>						taperDelegate(20, 40000);
		AAX_CNumberDisplayDelegate<float, 100>				displayDelegate;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		prefixDecorator(displayDelegate);
		AAX_CUnitDisplayDelegateDecorator<float>		decoratedDisplayDelegate(prefixDecorator, "Hz");
		AAX_CParameter<float>							param("1", AAX_CString("Linearparameter"), 1000, taperDelegate, decoratedDisplayDelegate);
		
		//Check many valid values within range and outside of range.
		for (float newValue = 20.0f; newValue < 40000.0f; newValue += 100.1f)
		{
			param.SetValue(newValue);
			AAX_CString	valueString;
			CHECK(param.GetValueString(&valueString));
			param.SetToDefaultValue();		//make sure to change it in between.
			CHECK(param.SetValueFromString(valueString));
			CHECK_CLOSE(newValue, param.GetValue(), 0.01f);
		}
	}
	
	TEST(IntToStringAndBack)
    {
		//Value String Map
		std::map<int32_t, AAX_CString>				stringMap;
		stringMap[-5] = "MinusFive";
		stringMap[0] = "Zero";
		stringMap[1] = "One";
		
		AAX_CStringDisplayDelegate<int32_t>		stringDelegate(stringMap);
		AAX_CLinearTaperDelegate<int32_t>		taperDelegate(-5, 1);
		AAX_CParameter<int32_t>				param("1", AAX_CString("Linearparameter"), 0, taperDelegate, stringDelegate);
		AAX_CString						valueString;
				
		//value to string
		param.SetValue(0);
		CHECK(param.GetValueString(&valueString) );
		CHECK_EQUAL( "Zero", valueString.CString() );
		param.SetValue(1);
		CHECK(param.GetValueString(&valueString) );
		CHECK_EQUAL( "One", valueString.CString() );
		param.SetValue(-5);
		CHECK(param.GetValueString(&valueString) ); 
		CHECK_EQUAL( "MinusFive", valueString.CString() );
		
		//This should clamp at the min value of -5.
		param.SetValue(-10);
		CHECK(param.GetValueString(&valueString));
		CHECK_EQUAL( "MinusFive", valueString.CString() );		

		//This should clamp at the max value of 1.
		param.SetValue(10);
		CHECK(param.GetValueString(&valueString));
		CHECK_EQUAL( "One", valueString.CString() );		
		
		//This should not find a control string for this in range value.
		param.SetValue(-2);
		CHECK(param.GetValueString(&valueString) == false);
		CHECK_EQUAL( "String Not Found", valueString.CString() );		
		
		//string to value
		CHECK(param.SetValueFromString(AAX_CString("Zero")) );
		CHECK_EQUAL(0, param.GetValue());
		CHECK(param.SetValueFromString(AAX_CString("One")) );
		CHECK_EQUAL(1, param.GetValue());
		CHECK(param.SetValueFromString(AAX_CString("MinusFive")) );
		CHECK_EQUAL(-5, param.GetValue());
		
		//bad string value
		param.SetValue(-2);
		CHECK(param.SetValueFromString(AAX_CString("MinusOne")) == false);
		CHECK_EQUAL(-2, param.GetValue());		//don't change the value if the string was invalid.
	}
	
	TEST(SetTaperDelegate)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CLinearTaperDelegate<float> newTaper(-200, 20);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10,  taper, display);
		
		CHECK_EQUAL( 10, param.GetValue() );
		param.SetTaperDelegate(newTaper);
		CHECK_EQUAL( 10, param.GetValue() );	//Value shouldn't change if it's in range.
		param.SetValue(-150);
		param.SetTaperDelegate(taper);
		CHECK_EQUAL( -100, param.GetValue() );	//Value should be clamped here since it was out of range of the new taper.
		param.SetValue(100);
		param.SetTaperDelegate(newTaper);
		CHECK_EQUAL( 20, param.GetValue() );	//Value should be clamped here since it was out of range of the new taper.		
	}
	
	TEST(SetDisplayDelegate)
	{
		AAX_CLinearTaperDelegate<float>						taper(-10000, 10000);
		AAX_CNumberDisplayDelegate<float, 0>				display;
		AAX_CUnitPrefixDisplayDelegateDecorator<float>		newDisplay(display);		//no worries about this, display is cloned.
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 1000, taper, display);
		
		AAX_CString	valueString;
		param.GetValueString(&valueString);
		CHECK_EQUAL( "1000", valueString.CString() );
		param.SetDisplayDelegate(newDisplay);
		param.GetValueString(&valueString);
		CHECK_EQUAL( "1k", valueString.CString());	
		param.SetValueFromString(AAX_CString("2k"));
		param.SetDisplayDelegate(display);
		param.GetValueString(&valueString);
		CHECK_EQUAL( "2000", valueString.CString() );	
		param.SetValue(-8000);
		param.SetDisplayDelegate(newDisplay);
		param.GetValueString(&valueString);
		CHECK_EQUAL( "-8k", valueString.CString());	
		param.SetToDefaultValue();
		param.GetValueString(&valueString);
		CHECK_EQUAL( "1k", valueString.CString());	
	}

	TEST(NumSteps)
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>		param("1", AAX_CString("Linearparameter"), 10, taper, display);
		
		//Check the actual setting and getting of the numSteps.
		uint32_t numSteps = param.GetNumberOfSteps();
		CHECK_EQUAL(uint32_t(AAX_CParameter<float>::eParameterDefaultNumStepsContinuous), numSteps);		//unitialized value
		param.SetNumberOfSteps(10);
		numSteps = param.GetNumberOfSteps();
		CHECK_EQUAL(uint32_t(10), numSteps);
		param.SetNumberOfSteps(10000);
		numSteps = param.GetNumberOfSteps();
		CHECK_EQUAL(uint32_t(10000), numSteps);
	}

	TEST(ConvertFloatStringToNormalizedValue)
	{
		AAX_CLinearTaperDelegate<float>			taperDelegate(-50, 30);
		AAX_CNumberDisplayDelegate<float, 100>	displayDelegate;
		AAX_CParameter<float>				param("1", AAX_CString("Linearparameter"), 10, taperDelegate, displayDelegate);
		
		//Check many valid values within range and outside of range.
		AAX_CString valueString;
		for (float newValue = -100; newValue < 100; newValue += 5.1f)
		{
			valueString.Clear();
			valueString.AppendNumber(newValue, 1);
			double convertedNumber;
			if (newValue < -50)
			{
				CHECK(!param.GetNormalizedValueFromString(valueString, &convertedNumber));  //should return false if it is out of range.
			}
			else if (newValue > 30)
			{
				CHECK(!param.GetNormalizedValueFromString(valueString, &convertedNumber));  //should return false if it is out of range.
			}
			else
			{
				CHECK(param.GetNormalizedValueFromString(valueString, &convertedNumber));
				CHECK_CLOSE( ((newValue+50.0)/80.0) , convertedNumber, 0.000001);		//compare with a hand converted normalized value.
			}
		}
	}

}
