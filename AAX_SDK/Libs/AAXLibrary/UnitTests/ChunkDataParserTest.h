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
 *  \file   ChunkDataParserTest.h
 *
 *  \author Ian Ratzer
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
#include "AAX_CString.h"

SUITE(ChunkDataParserTest)
{
	TEST(Constructor)
	{
		AAX_CChunkDataParser* parser = new AAX_CChunkDataParser();

		CHECK(parser->GetChunkDataSize() == AAX_ChunkDataParserDefs::HEADER_SIZE);
	}
	
	TEST(AddElements)
	{
		// Push one of each kind of element, check IsEmpty() and Clear()

		AAX_CChunkDataParser* parser = new AAX_CChunkDataParser();

		parser->AddFloat("Float for testing", 1.2f);
		CHECK(!parser->IsEmpty());
		parser->Clear();
		CHECK(parser->IsEmpty());
		
		parser->AddDouble("Double for testing", 3.4f);
		CHECK(!parser->IsEmpty());
		parser->Clear();
		CHECK(parser->IsEmpty());
		
		parser->AddInt32("Int32 for testing", 65000000);
		CHECK(!parser->IsEmpty());
		parser->Clear();
		CHECK(parser->IsEmpty());
		
		parser->AddInt16("Int16 for testing", -32000);
		CHECK(!parser->IsEmpty());
		parser->Clear();
		CHECK(parser->IsEmpty());
		
		parser->AddString("String for testing", "This is a somewhat longer string for testing purposes.");
		CHECK(!parser->IsEmpty());
		parser->Clear();
		CHECK(parser->IsEmpty());
	}
	
	TEST(FindElements)
	{
		// Push a handful of different data elements and find them

		int16_t foundInt16;
		int32_t foundInt32;
		double foundDouble;
		float foundFloat;
		AAX_CString foundString;
		
		AAX_CChunkDataParser* parser = new AAX_CChunkDataParser();

		parser->AddFloat("Float #1 for testing", 1.2f);
		parser->AddInt16("Int16 #1 for testing", -32000);
		parser->AddString("String #1 for testing", "This is a string for testing purposes.");
		parser->AddDouble("Double #1 for testing", 3.4f);
		parser->AddInt32("Int32 #1 for testing", 65000000);

		parser->AddFloat("Float #2 for testing", -3.4f);
		parser->AddInt16("Int16 #2 for testing", 67);
		parser->AddDouble("Double #2 for testing", 5.6e21);
		parser->AddString("String #2 for testing", "This is a much, much longer string than the other one blah blah blah blah blah blah blah blah for testing purposes.");
		parser->AddInt32("Int32 #2 for testing", -65000000);

		CHECK(!parser->IsEmpty());

		parser->FindInt16("Int16 #2 for testing", &foundInt16);
		CHECK_EQUAL(67, foundInt16);
		
		parser->FindInt32("Int32 #2 for testing", &foundInt32);
		CHECK_EQUAL(-65000000, foundInt32);
		
		parser->FindString("String #1 for testing", &foundString);
		CHECK_EQUAL("This is a string for testing purposes.", foundString.Get());

		parser->FindString("String #2 for testing", &foundString);
		CHECK_EQUAL("This is a much, much longer string than the other one blah blah blah blah blah blah blah blah for testing purposes.", foundString.Get());

		parser->FindInt16("Int16 #1 for testing", &foundInt16);
		CHECK_EQUAL(-32000, foundInt16);
		
		parser->FindInt32("Int32 #1 for testing", &foundInt32);
		CHECK_EQUAL(65000000, foundInt32);
		
		parser->FindDouble("Double #1 for testing", &foundDouble);
		CHECK_EQUAL(3.4f, foundDouble);
		
		parser->FindDouble("Double #2 for testing", &foundDouble);
		CHECK_EQUAL(5.6e21, foundDouble);
		
		parser->FindFloat("Float #1 for testing", &foundFloat);
		CHECK_EQUAL(float(1.2), foundFloat);
		
		parser->FindFloat("Float #2 for testing", &foundFloat);
		CHECK_EQUAL(float(-3.4), foundFloat);
		
		parser->ReplaceDouble("Double #2 for testing", 5.6f);
		parser->FindDouble("Double #2 for testing", &foundDouble);
		CHECK_EQUAL(5.6f, foundDouble);
	}
	
	TEST(LoadAndSet)
	{
		// Load and Set Chunk data including Strings
		
		AAX_Result result;
		
		int16_t foundInt16;
		int32_t foundInt32;
		double foundDouble;
		float foundFloat;
		AAX_CString foundString;

		AAX_CChunkDataParser* parser = new AAX_CChunkDataParser();

		parser->AddInt32("Int32 #1 for testing", 650000);
		parser->AddDouble("Double #1 for testing", 2.2f);
		parser->AddString("String #1 for testing", "This is a string longer longer longer longer longer longer longer longer long");
		parser->AddInt16("Int16 #1 for testing", -32000);
		parser->AddFloat("Float #1 for testing", 1.2f);
		
		result = parser->GetChunkDataSize();
		CHECK_EQUAL(224, result);

		char* rawChunk = new char[300];
		AAX_SPlugInChunk* chunk = reinterpret_cast<AAX_SPlugInChunk*>(rawChunk);
		
		result = parser->GetChunkData(chunk);
		CHECK_EQUAL(AAX_SUCCESS, result);
		
		parser->Clear();

		parser->LoadChunk(chunk);
		
		parser->FindInt32("Int32 #1 for testing", &foundInt32);
		CHECK_EQUAL(650000, foundInt32);
		
		parser->FindString("String #1 for testing", &foundString);
		CHECK_EQUAL("This is a string longer longer longer longer longer longer longer longer long", foundString.Get());
		
		parser->FindDouble("Double #1 for testing", &foundDouble);
		CHECK_EQUAL(double(2.2f), foundDouble);
		
		parser->FindFloat("Float #1 for testing", &foundFloat);
		CHECK_EQUAL(1.2f, foundFloat);

		parser->FindInt16("Int16 #1 for testing", &foundInt16);
		CHECK_EQUAL(-32000, foundInt16);
	}
}
