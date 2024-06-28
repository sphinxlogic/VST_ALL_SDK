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
 *
 */

/** 
 *  \file   ParameterManagerTest.h
 *
 *  \author Dave Tremblay
 */
/*================================================================================================*/
#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_CParameterManager.h"
#include "AAX_CParameter.h"

#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_CNumberDisplayDelegate.h"
#include "AAX_CBinaryDisplayDelegate.h"

SUITE(ParameterManagerTest)
{
	struct Fixture
	{
		Fixture() 
		{
			parameterManager = new AAX_CParameterManager();
		}
		
		~Fixture()
		{
			delete parameterManager;
		}
		
		AAX_CParameterManager*									parameterManager;
	};
	
	TEST_FIXTURE(Fixture, AddParameter)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		CHECK_EQUAL( int32_t(1), parameterManager->NumParameters() );
		parameterManager->AddParameter(intParameter);
		CHECK_EQUAL( int32_t(2), parameterManager->NumParameters() );
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( int32_t(3), parameterManager->NumParameters() );
	}


	TEST_FIXTURE(Fixture, RemoveParameter)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( int32_t(3), parameterManager->NumParameters() );
		parameterManager->RemoveParameter(floatParameter);
		CHECK_EQUAL( int32_t(2), parameterManager->NumParameters() );
		parameterManager->RemoveParameter(intParameter);		
		CHECK_EQUAL( int32_t(1), parameterManager->NumParameters() );
		parameterManager->RemoveParameter(controlStringParameter);
		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
	}
	
	TEST_FIXTURE(Fixture, RemoveParameterByIdentifier)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( int32_t(3), parameterManager->NumParameters() );
		parameterManager->RemoveParameterByID("1");
		CHECK_EQUAL( int32_t(2), parameterManager->NumParameters() );
		parameterManager->RemoveParameterByID("1");		//try a double removal.
		CHECK_EQUAL( int32_t(2), parameterManager->NumParameters() );
		parameterManager->RemoveParameterByID("2");		
		CHECK_EQUAL( int32_t(1), parameterManager->NumParameters() );
		parameterManager->RemoveParameterByID("3");
		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
	}	
	
	TEST_FIXTURE(Fixture, RemoveAllParameters)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( int32_t(3), parameterManager->NumParameters() );
		parameterManager->RemoveAllParameters();
		CHECK_EQUAL(int32_t( 0), parameterManager->NumParameters() );
	}
		
	TEST_FIXTURE(Fixture, FindParameters)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( floatParameter, parameterManager->GetParameterByID("1") );
		CHECK_EQUAL( intParameter, parameterManager->GetParameterByID("2") );
		CHECK_EQUAL( controlStringParameter, parameterManager->GetParameterByID("3") );
		CHECK( parameterManager->GetParameterByID("25") == 0 );
	}
	
	TEST_FIXTURE(Fixture, DuplicateParameterIdentifiers)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);
		AAX_CParameter<float>* floatParameter2 = new AAX_CParameter<float>("1", AAX_CString("floatParam2"), 0, floatTaper, floatDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( floatParameter, parameterManager->GetParameterByID("1") );
		CHECK_EQUAL( intParameter, parameterManager->GetParameterByID("2") );
		CHECK_EQUAL( controlStringParameter, parameterManager->GetParameterByID("3") );
		
		//Same ID, so it should fail and leave the first parameter in there.
		parameterManager->AddParameter(floatParameter2);
		CHECK_EQUAL( int32_t(3), parameterManager->NumParameters() );			
		CHECK_EQUAL( floatParameter, parameterManager->GetParameterByID("1") );
	}

	TEST_FIXTURE(Fixture, AddLinearFloatParameter)
	{
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("param1"), 50.0f, AAX_CLinearTaperDelegate<float>(0.0f, 100.0f), AAX_CNumberDisplayDelegate<float>());
		AAX_CParameter<float>* floatParameter2 = new AAX_CParameter<float>("2", AAX_CString("param2"), 0.0f, AAX_CLinearTaperDelegate<float>(-100.0f, 100.0f), AAX_CNumberDisplayDelegate<float>());

		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(floatParameter2);
		AAX_IParameter* parameter1 = parameterManager->GetParameterByID("1");
		AAX_IParameter* parameter2 = parameterManager->GetParameterByID("2");
		AAX_CParameter<float>* floatParam1 = dynamic_cast<AAX_CParameter<float>*>(parameter1);
		CHECK(floatParam1 != 0);
		AAX_CParameter<float>* floatParam2 = dynamic_cast<AAX_CParameter<float>*>(parameter2);
		CHECK(floatParam2 != 0);
		CHECK_EQUAL(50.0f, floatParam1->GetValue());
		CHECK_EQUAL(0.0f, floatParam2->GetValue());
		CHECK_EQUAL(0.5, floatParam1->GetNormalizedValue());
		CHECK_EQUAL(0.5, floatParam2->GetNormalizedValue());
	}
	
	TEST_FIXTURE(Fixture, AddLinearIntegerParameter)
	{
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("1", AAX_CString("param1"), 50, AAX_CLinearTaperDelegate<int32_t>(0, 100), AAX_CNumberDisplayDelegate<int32_t>());
		AAX_CParameter<int32_t>* intParameter2 = new AAX_CParameter<int32_t>("2", AAX_CString("param2"), 0, AAX_CLinearTaperDelegate<int32_t>(-100, 100), AAX_CNumberDisplayDelegate<int32_t>());

		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(intParameter2);
		AAX_IParameter* parameter1 = parameterManager->GetParameterByID("1");
		AAX_IParameter* parameter2 = parameterManager->GetParameterByID("2");
		AAX_CParameter<int32_t>* intParam1 = dynamic_cast<AAX_CParameter<int32_t>*>(parameter1);
		CHECK(intParam1 != 0);
		AAX_CParameter<int32_t>* intParam2 = dynamic_cast<AAX_CParameter<int32_t>*>(parameter2);
		CHECK(intParam2 != 0);
		CHECK_EQUAL(50, intParam1->GetValue());
		CHECK_EQUAL(0, intParam2->GetValue());
		CHECK_EQUAL(0.5, parameter1->GetNormalizedValue());
		CHECK_EQUAL(0.5, parameter2->GetNormalizedValue());
	}

	TEST_FIXTURE(Fixture, AddBinaryBoolParameters)
	{
		AAX_CParameter<bool>* boolParameter = new AAX_CParameter<bool>("1", AAX_CString("param1"), false, AAX_CBinaryTaperDelegate<bool>(), AAX_CBinaryDisplayDelegate<bool>("false", "true"));
		AAX_CParameter<bool>* boolParameter2 = new AAX_CParameter<bool>("2", AAX_CString("param2"), true, AAX_CBinaryTaperDelegate<bool>(), AAX_CBinaryDisplayDelegate<bool>("false", "true"));
		
		parameterManager->AddParameter(boolParameter);
		parameterManager->AddParameter(boolParameter2);
		AAX_IParameter* parameter1 = parameterManager->GetParameterByID("1");
		AAX_IParameter* parameter2 = parameterManager->GetParameterByID("2");
		AAX_CParameter<bool>* boolParam1 = dynamic_cast<AAX_CParameter<bool>*>(parameter1);
		CHECK(boolParam1 != 0);
		AAX_CParameter<bool>* boolParam2 = dynamic_cast<AAX_CParameter<bool>*>(parameter2);
		CHECK(boolParam2 != 0);
		CHECK_EQUAL(false, boolParam1->GetValue());
		CHECK_EQUAL(true, boolParam2->GetValue());
		CHECK_EQUAL(0, parameter1->GetNormalizedValue());
		CHECK_EQUAL(1, parameter2->GetNormalizedValue());
	}


	TEST_FIXTURE(Fixture, GetParameters)
	{
		//Manager takes ownership of these in the tests.  Manager is responsible for deletion.
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CLinearTaperDelegate<int32_t>		intTaper;
		AAX_CNumberDisplayDelegate<int32_t>		intDisplay;
		AAX_CLinearTaperDelegate<uint32_t>		uintTaper;
		AAX_CNumberDisplayDelegate<uint32_t>	uintDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("1", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		AAX_CParameter<int32_t>* intParameter = new AAX_CParameter<int32_t>("2", AAX_CString("intParam"), 0, intTaper, intDisplay);
		AAX_CParameter<uint32_t>* controlStringParameter = new AAX_CParameter<uint32_t>("3", AAX_CString("controlStringParam"), 0, uintTaper, uintDisplay);

		CHECK_EQUAL( int32_t(0), parameterManager->NumParameters() );
		parameterManager->AddParameter(floatParameter);
		parameterManager->AddParameter(intParameter);
		parameterManager->AddParameter(controlStringParameter);
		CHECK_EQUAL( floatParameter, parameterManager->GetParameter(0) );
		CHECK_EQUAL( intParameter, parameterManager->GetParameter(1) );
		CHECK_EQUAL( controlStringParameter, parameterManager->GetParameter(2) );
		CHECK( parameterManager->GetParameter(25) == 0 );
	}
	
	
}

