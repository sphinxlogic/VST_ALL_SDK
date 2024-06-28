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
 *	\file DataConversionTests.h
 *
 */ 
/*================================================================================================*/

#ifndef AAXLibrary_DataConversionTests_h
#define AAXLibrary_DataConversionTests_h

#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_Enums.h"

SUITE(DataConversionTests)
{
	TEST(IntLimits)
	{
		CHECK_EQUAL(INT32_MIN,  AAX_INT32_MIN);
		CHECK_EQUAL(INT32_MAX,  AAX_INT32_MAX);
		CHECK_EQUAL(0x0U,       AAX_UINT32_MIN);
		CHECK_EQUAL(UINT32_MAX, AAX_UINT32_MAX);

		CHECK_EQUAL(INT16_MIN,  AAX_INT16_MIN);
		CHECK_EQUAL(INT16_MAX,  AAX_INT16_MAX);
		CHECK_EQUAL(0x0U,       AAX_UINT16_MIN);
		CHECK_EQUAL((unsigned)UINT16_MAX, AAX_UINT16_MAX);
	}

	TEST(StemFormatNum)
	{
		// If any stem formats are added in the future, update all Stem Format macro tests.
		CHECK_EQUAL(13U, AAX_eStemFormatNum);
	}
	
	TEST(StemFormatChannelCount)
	{
		CHECK_EQUAL(1, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Mono));
		CHECK_EQUAL(2, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Stereo));
		CHECK_EQUAL(3, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_LCR));
		CHECK_EQUAL(4, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_LCRS));
		CHECK_EQUAL(4, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Quad));
		CHECK_EQUAL(5, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_5_0));
		CHECK_EQUAL(6, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_5_1));
		CHECK_EQUAL(6, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_6_0));
		CHECK_EQUAL(7, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_6_1));
		CHECK_EQUAL(7, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_7_0_SDDS));
		CHECK_EQUAL(8, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_7_1_SDDS));
		CHECK_EQUAL(7, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_7_0_DTS));
		CHECK_EQUAL(8, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_7_1_DTS));
		CHECK_EQUAL(0, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_None));
		CHECK_EQUAL(0, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Any));

#if USE_DISCRETE_STEMFORMATS
		CHECK_EQUAL(1, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_1));
		CHECK_EQUAL(2, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_2));
		CHECK_EQUAL(3, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_3));
		CHECK_EQUAL(4, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_4));
		CHECK_EQUAL(5, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_5));
		CHECK_EQUAL(6, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_6));
		CHECK_EQUAL(7, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_7));
		CHECK_EQUAL(8, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_8));
		CHECK_EQUAL(9, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_9));
		CHECK_EQUAL(10, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_10));
		CHECK_EQUAL(11, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_11));
		CHECK_EQUAL(12, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_12));
		CHECK_EQUAL(13, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_13));
		CHECK_EQUAL(14, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_14));
		CHECK_EQUAL(15, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_15));
		CHECK_EQUAL(16, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_eStemFormat_Discrete_16));
#endif

		CHECK_EQUAL(AAX_UINT16_MAX, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_STEM_FORMAT(0, 0xFFFF)));
		CHECK(-1 != AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_STEM_FORMAT(0, 0xFFFF)));
		
		// This definition should generate a compiler warning about signed/unsigned
		// comparison, but I don't know how to add a unit test for a compiler warning...
//		CHECK_EQUAL(0, AAX_STEM_FORMAT_CHANNEL_COUNT(AAX_STEM_FORMAT(0,-1)));
	}
	
	TEST(StemFormatIndex)
	{
		CHECK_EQUAL(0, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Mono));
		CHECK_EQUAL(1, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Stereo));
		CHECK_EQUAL(2, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_LCR));
		CHECK_EQUAL(3, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_LCRS));
		CHECK_EQUAL(4, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Quad));
		CHECK_EQUAL(5, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_5_0));
		CHECK_EQUAL(6, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_5_1));
		CHECK_EQUAL(7, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_6_0));
		CHECK_EQUAL(8, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_6_1));
		CHECK_EQUAL(9, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_7_0_SDDS));
		CHECK_EQUAL(10, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_7_1_SDDS));
		CHECK_EQUAL(11, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_7_0_DTS));
		CHECK_EQUAL(12, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_7_1_DTS));
		CHECK_EQUAL(-100, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_None));
		CHECK_EQUAL(-1, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Any));

#if USE_DISCRETE_STEMFORMATS
		CHECK_EQUAL(-201, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_1));
		CHECK_EQUAL(-202, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_2));
		CHECK_EQUAL(-203, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_3));
		CHECK_EQUAL(-204, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_4));
		CHECK_EQUAL(-205, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_5));
		CHECK_EQUAL(-206, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_6));
		CHECK_EQUAL(-207, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_7));
		CHECK_EQUAL(-208, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_8));
		CHECK_EQUAL(-209, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_9));
		CHECK_EQUAL(-210, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_10));
		CHECK_EQUAL(-211, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_11));
		CHECK_EQUAL(-212, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_12));
		CHECK_EQUAL(-213, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_13));
		CHECK_EQUAL(-214, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_14));
		CHECK_EQUAL(-215, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_15));
		CHECK_EQUAL(-216, AAX_STEM_FORMAT_INDEX(AAX_eStemFormat_Discrete_16));
#endif
		
		CHECK(AAX_INT16_MAX != AAX_STEM_FORMAT_INDEX(AAX_STEM_FORMAT(0xFFFF, 0)));
		CHECK_EQUAL(-1, AAX_STEM_FORMAT_INDEX(AAX_STEM_FORMAT(0xFFFF, 0)));
		CHECK_EQUAL(AAX_INT16_MAX, AAX_STEM_FORMAT_INDEX(AAX_STEM_FORMAT(0x7FFF, 0)));
	}
	
	TEST(StemFormatValues)
	{
		CHECK_EQUAL(0x00000001U, AAX_eStemFormat_Mono);
		CHECK_EQUAL(0x00010002U, AAX_eStemFormat_Stereo);
		CHECK_EQUAL(0x00020003U, AAX_eStemFormat_LCR);
		CHECK_EQUAL(0x00030004U, AAX_eStemFormat_LCRS);
		CHECK_EQUAL(0x00040004U, AAX_eStemFormat_Quad);
		CHECK_EQUAL(0x00050005U, AAX_eStemFormat_5_0);
		CHECK_EQUAL(0x00060006U, AAX_eStemFormat_5_1);
		CHECK_EQUAL(0x00070006U, AAX_eStemFormat_6_0);
		CHECK_EQUAL(0x00080007U, AAX_eStemFormat_6_1);
		CHECK_EQUAL(0x00090007U, AAX_eStemFormat_7_0_SDDS);
		CHECK_EQUAL(0x000A0008U, AAX_eStemFormat_7_1_SDDS);
		CHECK_EQUAL(0x000B0007U, AAX_eStemFormat_7_0_DTS);
		CHECK_EQUAL(0x000C0008U, AAX_eStemFormat_7_1_DTS);
		CHECK_EQUAL(0xFF9C0000U, AAX_eStemFormat_None);
		CHECK_EQUAL(0xFFFF0000U, AAX_eStemFormat_Any);

#if USE_DISCRETE_STEMFORMATS
		CHECK_EQUAL(0xFF370001U, AAX_eStemFormat_Discrete_1);
		CHECK_EQUAL(0xFF360002U, AAX_eStemFormat_Discrete_2);
		CHECK_EQUAL(0xFF350003U, AAX_eStemFormat_Discrete_3);
		CHECK_EQUAL(0xFF340004U, AAX_eStemFormat_Discrete_4);
		CHECK_EQUAL(0xFF330005U, AAX_eStemFormat_Discrete_5);
		CHECK_EQUAL(0xFF320006U, AAX_eStemFormat_Discrete_6);
		CHECK_EQUAL(0xFF310007U, AAX_eStemFormat_Discrete_7);
		CHECK_EQUAL(0xFF300008U, AAX_eStemFormat_Discrete_8);
		CHECK_EQUAL(0xFF2F0009U, AAX_eStemFormat_Discrete_9);
		CHECK_EQUAL(0xFF2E000AU, AAX_eStemFormat_Discrete_10);
		CHECK_EQUAL(0xFF2D000BU, AAX_eStemFormat_Discrete_11);
		CHECK_EQUAL(0xFF2C000CU, AAX_eStemFormat_Discrete_12);
		CHECK_EQUAL(0xFF2B000DU, AAX_eStemFormat_Discrete_13);
		CHECK_EQUAL(0xFF2A000EU, AAX_eStemFormat_Discrete_14);
		CHECK_EQUAL(0xFF29000FU, AAX_eStemFormat_Discrete_15);
		CHECK_EQUAL(0xFF280010U, AAX_eStemFormat_Discrete_16);
#endif
	}
}

#endif
