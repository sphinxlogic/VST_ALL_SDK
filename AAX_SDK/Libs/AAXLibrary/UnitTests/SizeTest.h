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
 *  \file   SizeTest.h
 *
 */
/*================================================================================================*/


#include "Digi_UnitTest/Digi_UT_Include.h"

#include <stdint.h>
#include "acfbasetypes.h"
#include "AAX.h"
#include "AAX_GUITypes.h"

SUITE(BasicTypeSizeTest) 
{
	TEST(ACFIntTest)
    {
		CHECK( sizeof(int32_t) == sizeof(acfSInt32) );
		CHECK( sizeof(uint32_t) == sizeof(acfUInt32) );
		CHECK( sizeof(int64_t) == sizeof(acfSInt64) );
		CHECK( sizeof(uint64_t) == sizeof(acfUInt64) );
	}
}	

SUITE(AAXTypeSizeTest)
{
	TEST(Type_Test)
	{
		CHECK( sizeof(AAX_CIndex) == 4 );
		CHECK( sizeof(AAX_CCount) == 4 );
		CHECK( sizeof(AAX_CBoolean) == 1 );
		CHECK( sizeof(AAX_CSelector) == 4 );
		CHECK( sizeof(AAX_CTimestamp) == 8 );
		CHECK( sizeof(AAX_CTimeOfDay) == 8 );
        CHECK( sizeof(AAX_CTransportCounter) == 8 );
		CHECK( sizeof(AAX_CSampleRate) == 4 );
		CHECK( sizeof(AAX_CTypeID) == 4 );
		CHECK( sizeof(AAX_CFieldIndex) == 4 );
		CHECK( sizeof(AAX_CComponentID) == 4 );
		CHECK( sizeof(AAX_CMeterID) == 4 );
		CHECK( sizeof(AAX_CParamID) == sizeof(void*) );
		CHECK( sizeof(AAX_CEffectID) == sizeof(void*) );
	}
	
	TEST(Chunk_Struct_Test)
	{
		// AAX_SPlugInChunk
        #if ( defined(_WIN64) || defined(__LP64__) ) 
            CHECK_EQUAL( size_t(60), sizeof(AAX_SPlugInChunk)); // 57, packed  (8 byte aligned)
        #else
            CHECK_EQUAL( size_t(58), sizeof(AAX_SPlugInChunk)); // 57, packed   (2 byte aligned)
        #endif
        CHECK_EQUAL( size_t(0), offsetof(AAX_SPlugInChunk,fSize) );
        CHECK_EQUAL( size_t(4), offsetof(AAX_SPlugInChunk,fVersion) );
        CHECK_EQUAL( size_t(8), offsetof(AAX_SPlugInChunk,fManufacturerID) );
        CHECK_EQUAL( size_t(12), offsetof(AAX_SPlugInChunk,fProductID) );
        CHECK_EQUAL( size_t(16), offsetof(AAX_SPlugInChunk,fPlugInID) );
        CHECK_EQUAL( size_t(20), offsetof(AAX_SPlugInChunk,fChunkID) );
        CHECK_EQUAL( size_t(24), offsetof(AAX_SPlugInChunk,fName) );
        CHECK_EQUAL( size_t(56), offsetof(AAX_SPlugInChunk,fData) );
		
		// AAX_SPlugInChunkHeader
		CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fSize), offsetof(AAX_SPlugInChunkHeader,fSize) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fVersion), offsetof(AAX_SPlugInChunkHeader,fVersion) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fManufacturerID), offsetof(AAX_SPlugInChunkHeader,fManufacturerID) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fProductID), offsetof(AAX_SPlugInChunkHeader,fProductID) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fPlugInID), offsetof(AAX_SPlugInChunkHeader,fPlugInID) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fChunkID), offsetof(AAX_SPlugInChunkHeader,fChunkID) );
        CHECK_EQUAL( offsetof(AAX_SPlugInChunk,fName), offsetof(AAX_SPlugInChunkHeader,fName) );
    }
    
    TEST(AAX_SPlugInIdentifierTriad_TEST)
    {
        CHECK_EQUAL(size_t(12), sizeof(AAX_SPlugInIdentifierTriad));
        CHECK_EQUAL(size_t(0), offsetof(AAX_SPlugInIdentifierTriad, mManufacturerID));
        CHECK_EQUAL(size_t(4), offsetof(AAX_SPlugInIdentifierTriad, mProductID));
        CHECK_EQUAL(size_t(8), offsetof(AAX_SPlugInIdentifierTriad, mPlugInID));
    }
    
    TEST(Midi_Struct_Test)
    {
        #if ( defined(_WIN64) || defined(__LP64__) )
            //64 bit windows and 64 bit OSX  (THESE ARE CHANGED TO BE 8 BYTE ALIGNED)
            CHECK_EQUAL( size_t(16), sizeof(AAX_CMidiPacket));  //13, packed.  Alg requires 8 byte aligned.
            CHECK_EQUAL( size_t(0), offsetof(AAX_CMidiPacket,mTimestamp) );
            CHECK_EQUAL( size_t(4), offsetof(AAX_CMidiPacket,mLength) );
            CHECK_EQUAL( size_t(8), offsetof(AAX_CMidiPacket,mData) );
            CHECK_EQUAL( size_t(12), offsetof(AAX_CMidiPacket,mIsImmediate) );
            
            CHECK_EQUAL( size_t(16), sizeof(AAX_CMidiStream));  //12, packed.  Alg requires 8 byte aligned.
            CHECK_EQUAL( size_t(0), offsetof(AAX_CMidiStream,mBufferSize) );
            CHECK_EQUAL( size_t(8), offsetof(AAX_CMidiStream,mBuffer) );
        #else 
           //<DMT> In 32 bit, instead of 8 byte alignement values, I believe that this is what is locked down for PT10 compatibility.
            #ifdef _WIN32
                    CHECK_EQUAL( size_t(14), sizeof(AAX_CMidiPacket));  //13, packed.  
            #else	//GNU
                    CHECK_EQUAL( size_t(16), sizeof(AAX_CMidiPacket));  //13, packed. (for some reason, OSX is different here)
            #endif
            CHECK_EQUAL( size_t(0), offsetof(AAX_CMidiPacket,mTimestamp) );
            CHECK_EQUAL( size_t(4), offsetof(AAX_CMidiPacket,mLength) );
            CHECK_EQUAL( size_t(8), offsetof(AAX_CMidiPacket,mData) );
            CHECK_EQUAL( size_t(12), offsetof(AAX_CMidiPacket,mIsImmediate) );

            CHECK_EQUAL( size_t(8), sizeof(AAX_CMidiStream));  //8, packed.  Original unit tests claim this should be 5 bytes packed, but that looks wrong to me.
            CHECK_EQUAL( size_t(0), offsetof(AAX_CMidiStream,mBufferSize) );
            CHECK_EQUAL( size_t(4), offsetof(AAX_CMidiStream,mBuffer) );
        #endif
	}

	TEST(GUITypes_Test)
	{
		CHECK( sizeof(AAX_Point) == 8 );
		CHECK( sizeof(AAX_Rect) == 16 );
	}
}
