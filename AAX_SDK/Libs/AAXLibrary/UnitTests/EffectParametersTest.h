/*================================================================================================*/
/*
 *	Copyright 2013-2015 by Avid Technology, Inc.
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
 *	\file EffectParametersTest.h
 *
 */ 
/*================================================================================================*/

#include "Digi_UnitTest/Digi_UT_Include.h"

#include "UnitTestAutomationDelegate.h"

#include "AAX_CEffectParameters.h"
#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CNumberDisplayDelegate.h"
#include "AAX_CParameter.h"
#include "AAX_CString.h"



class EffectParametersBasicTestObject : public AAX_CEffectParameters
{
public:
	EffectParametersBasicTestObject ( ) :
	AAX_CEffectParameters()
	{
	}
	
	virtual AAX_Result	EffectInit()
	{
		AAX_CLinearTaperDelegate<float>	taper(-100, 100);
		AAX_CNumberDisplayDelegate<float> display;
		AAX_CParameter<float>* param = new AAX_CParameter<float>("1", AAX_CString("LongName"), 0,  taper, display);
		param->AddShortenedName(AAX_CString("LongNm"));
		param->AddShortenedName(AAX_CString("LngNm"));
		
		mParameterManager.AddParameter(param);
		return AAX_SUCCESS;
	}
};


SUITE(EffectParametersTest)
{
	
	
	struct Fixture
	{
		Fixture()
		{
			automationDelegate = new UnitTestAutomationDelegate();
			effectParameters = new EffectParametersBasicTestObject();
			automationDelegate->SetEffectParameters(effectParameters);
			effectParameters->Initialize(automationDelegate);
		}
		
		~Fixture()
		{
			effectParameters->Uninitialize();
			delete effectParameters;
		}
		
		AAX_IEffectParameters*						effectParameters;
		UnitTestAutomationDelegate::SmartPtr		automationDelegate;
	};
	
	TEST_FIXTURE(Fixture, GetParameterNameOfLength)
	{
		AAX_CString abbreviatedName;
		const AAX_CParamID paramID = "1";
		
		// Verify parameter ID
		{
			int32_t paramIndex;
			AAX_CString paramIDForIndex;
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterIndex(paramID, &paramIndex));
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterIDFromIndex(paramIndex, &paramIDForIndex));
			CHECK_EQUAL(paramID, paramIDForIndex.CString());
			
			AAX_IParameter* param = NULL;
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameter(paramID, &param));
			CHECK(NULL != param);
		}
		
		// Method should never return a string that is longer than the requested length
		{
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 1023));
			CHECK_EQUAL(8U, abbreviatedName.Length());
			CHECK_EQUAL("LongName", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 31));
			CHECK_EQUAL(8U, abbreviatedName.Length());
			CHECK_EQUAL("LongName", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 8));
			CHECK_EQUAL(8U, abbreviatedName.Length());
			CHECK_EQUAL("LongName", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 7));
			CHECK_EQUAL(6U, abbreviatedName.Length());
			CHECK_EQUAL("LongNm", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 6));
			CHECK_EQUAL(6U, abbreviatedName.Length());
			CHECK_EQUAL("LongNm", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 5));
			CHECK_EQUAL(5U, abbreviatedName.Length());
			CHECK_EQUAL("LngNm", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 4));
			CHECK_EQUAL(4U, abbreviatedName.Length());
			CHECK_EQUAL("LngN", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 3));
			CHECK_EQUAL(3U, abbreviatedName.Length());
			CHECK_EQUAL("Lng", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 2));
			CHECK_EQUAL(2U, abbreviatedName.Length());
			CHECK_EQUAL("Ln", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 1));
			CHECK_EQUAL(1U, abbreviatedName.Length());
			CHECK_EQUAL("L", abbreviatedName.CString());
			
			CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNameOfLength(paramID, &abbreviatedName, 0));
			CHECK_EQUAL(0U, abbreviatedName.Length());
			CHECK_EQUAL("", abbreviatedName.CString());
		}
		
		// Method should fail if given a NULL string output parameter
		{
			CHECK(AAX_Result(AAX_SUCCESS) != effectParameters->GetParameterNameOfLength(paramID, NULL, 31));
			CHECK(AAX_Result(AAX_SUCCESS) != effectParameters->GetParameterNameOfLength(paramID, NULL, 0));
		}
	}
		
}
