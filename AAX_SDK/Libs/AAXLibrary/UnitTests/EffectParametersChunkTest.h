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
 *  \file   EffectParametersChunkTest.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/
#include "Digi_UnitTest/Digi_UT_Include.h"

#include "UnitTestAutomationDelegate.h"
#include "AAX_VAutomationDelegate.h"
#include "AAX_CBinaryDisplayDelegate.h"
#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_Assert.h"

#include "AAX_CEffectParameters.h"
#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_CNumberDisplayDelegate.h"
#include "AAX_CStringDisplayDelegate.h"
#include "AAX_CParameter.h"


class EffectParametersChunkTestObject : public AAX_CEffectParameters
{
public:
	EffectParametersChunkTestObject ( ) :
		AAX_CEffectParameters()
	{
	}
	
protected:
	AAX_Result	EffectInit()
	{
		AAX_CLinearTaperDelegate<float>		floatTaper;
		AAX_CNumberDisplayDelegate<float>	floatDisplay;
		AAX_CParameter<float>* floatParameter = new AAX_CParameter<float>("FloatParameter", AAX_CString("floatParam"), 0, floatTaper, floatDisplay);
		mParameterManager.AddParameter(floatParameter);
		return AAX_SUCCESS;
	}
};


SUITE(EffectParametersChunkTest)
{
	
	
	struct Fixture
	{
		Fixture() 
		{
			automationDelegate = new UnitTestAutomationDelegate();
			effectParameters = new EffectParametersChunkTestObject();
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
		
	TEST_FIXTURE(Fixture, GetNumberOfChunks)
	{
		int32_t	numChunks;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetNumberOfChunks(&numChunks));
		CHECK_EQUAL(1, numChunks);
	}
	
	TEST_FIXTURE(Fixture, GetChunkIDFromIndex)
	{
		AAX_CTypeID	chunkID;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunkIDFromIndex(0, &chunkID));
		CHECK_EQUAL(AAX_CTypeID('elck'), chunkID);
		
		//Check an out of bounds index.
		CHECK_EQUAL(AAX_Result(AAX_ERROR_INVALID_CHUNK_INDEX), effectParameters->GetChunkIDFromIndex(1, &chunkID));
		CHECK_EQUAL(AAX_CTypeID(0), chunkID);
	}
		
	TEST_FIXTURE(Fixture, GetChunkSize)
	{
		uint32_t	chunkSize;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunkSize('elck', &chunkSize));
		CHECK_EQUAL((uint32_t)32, chunkSize);	
		
		//Check an out of bounds index.
		CHECK_EQUAL(AAX_Result(AAX_ERROR_INVALID_CHUNK_ID), effectParameters->GetChunkSize('abcd', &chunkSize));
		CHECK_EQUAL((uint32_t)0, chunkSize);
	}
	
	TEST_FIXTURE(Fixture, GetChunk)
	{
		//Get the chunk size and create the chunk.
		uint32_t	chunkSize;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunkSize('elck', &chunkSize));
		CHECK_EQUAL((uint32_t)32, chunkSize);	
		uint32_t rawSize = chunkSize + sizeof(AAX_SPlugInChunkHeader);
		char* rawChunk = new char[rawSize];
		AAX_SPlugInChunk* chunk = reinterpret_cast<AAX_SPlugInChunk*> (rawChunk);
		
//		int32_t				fSize;				// The size of the entire chunk (including this field). 
//		int32_t				fVersion;			// The chunk's version. 
//		AAX_CTypeID			fManufacturerID;	// The Plug-In's manufacturer ID (assigned by Digi) 
//		AAX_CTypeID			fProductID;			// The Plug-In file's product ID 
//		AAX_CTypeID			fPlugInID;			// The ID of a particular Plug-In within the file 
//		AAX_CTypeID			fChunkID;			// The ID of a particular Plug-In chunk. 
//		Str31				fName;				// A user defined name for this chunk. 
//		char				fData[1];			// The chunk's data. 
		
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunk('elck', chunk));
		CHECK_EQUAL(32, chunk->fSize);
		
		//Check an out of bounds index.
		CHECK_EQUAL(AAX_Result(AAX_ERROR_INVALID_CHUNK_ID), effectParameters->GetChunk('abcd', chunk));
		
		//delete the chunk data.
		delete [] rawChunk;

	}
	
	TEST_FIXTURE(Fixture, SetChunk)
	{
		//Get the initial values.
		double initialFloatValue;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNormalizedValue("FloatParameter", &initialFloatValue));

		//Get the chunk size and create the chunk.
		uint32_t	chunkSize;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunkSize('elck', &chunkSize));
		CHECK_EQUAL((uint32_t)32, chunkSize);	
		uint32_t rawSize = chunkSize + sizeof(AAX_SPlugInChunk);
		char* rawChunk = new char[rawSize];
		AAX_SPlugInChunk* chunk = reinterpret_cast<AAX_SPlugInChunk*> (rawChunk);
		
		//Get the chunk.
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetChunk('elck', chunk));
		
		//Change the control values.
		effectParameters->SetParameterNormalizedValue( "FloatParameter", .5 );	
		double newFloatValue;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNormalizedValue("FloatParameter", &newFloatValue));
		CHECK(newFloatValue != initialFloatValue);
		
		//Set the chunk and verify settings were restored.
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->SetChunk ( 'elck', chunk ));
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetParameterNormalizedValue("FloatParameter", &newFloatValue));
		CHECK_EQUAL(initialFloatValue, newFloatValue);
		
		//delete the chunk data.
		delete [] rawChunk;
	}
	
	TEST_FIXTURE(Fixture, CompareActiveChunk)
	{
		//<DT>  I'm not sure we need to keep this function.
	}
	
	TEST_FIXTURE(Fixture, GetNumberOfChanges)
	{
		int32_t numChanges;
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetNumberOfChanges(&numChanges));
		CHECK_EQUAL((int32_t)0, numChanges);
		
		//Change the control values.
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->SetParameterNormalizedValue( "FloatParameter", 0.5 ));	
		CHECK_EQUAL(AAX_Result(AAX_SUCCESS), effectParameters->GetNumberOfChanges(&numChanges));
		CHECK_EQUAL((int32_t)1, numChanges);
	}
	
}
