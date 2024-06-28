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
 *  \file   SizeTest.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/

#include "Digi_UnitTest/Digi_UT_Include.h"
#include "AAX_CString.h"

SUITE(StringTest)
{
	
	TEST(UninitializedStringValue)
	{
		AAX_CString	inputString;
		CHECK_EQUAL("", inputString.CString());
	}
	
	TEST(InitializedStringValue)
	{
		AAX_CString	inputString("Howdy");
		CHECK_EQUAL("Howdy", inputString.CString());
		AAX_CString copyString(inputString);
		CHECK_EQUAL("Howdy", copyString.CString());
	}
	
	TEST(EqualityOperator)
	{
		AAX_CString	inputString("There");
		CHECK_EQUAL("There", inputString.CString());
		inputString.Erase(4, 1);
		CHECK(strcmp("There",inputString.CString()) != 0);
	}
	
	TEST(InequalityOperator)
	{
		AAX_CString	inputString("There");
		CHECK(AAX_CString("Here") != inputString);
		CHECK(!(AAX_CString("There") != inputString));
	}
	
	TEST(AssignmentOperator)
	{
		AAX_CString	inputString;
		inputString = "cString";
		CHECK_EQUAL("cString", inputString.CString());
		inputString = AAX_CString("string");
		CHECK_EQUAL("string", inputString.CString());
	}
	
	TEST(Length)
	{
		AAX_CString	inputString("Howdy");
		CHECK_EQUAL(uint32_t(5), inputString.Length());	
		inputString = "";
		CHECK_EQUAL(uint32_t(0), inputString.Length());	
	}
	
	TEST(MaxLength)
	{
		AAX_CString inputString("HelloWorld");
		CHECK_EQUAL(static_cast<uint32_t>(-2), inputString.MaxLength());
	}
	
	TEST(Clear)
	{
		AAX_CString inputString("HelloWorld");
		CHECK_EQUAL(uint32_t(10), inputString.Length());	
		inputString.Clear();
		CHECK_EQUAL(uint32_t(0), inputString.Length());
		CHECK_EQUAL("", inputString.CString());
	}
	
	TEST(Empty)
	{
		AAX_CString inputString;
		CHECK(inputString.Empty() == true);
		inputString = "A";
		CHECK(inputString.Empty() == false);
		inputString.Clear();
		CHECK(inputString.Empty() == true);
	}
	
	TEST(Append)
	{
		AAX_CString	inputString;
		inputString.Append(AAX_CString("AB"));
		CHECK_EQUAL("AB", inputString.CString());		
		inputString.Append("CD");
		CHECK_EQUAL("ABCD", inputString.CString());		
		inputString.AppendNumber(10);
		CHECK_EQUAL("ABCD10", inputString.CString());		
		inputString.AppendNumber(5.5, 1);
		CHECK_EQUAL("ABCD105.5", inputString.CString());		
	}
	
	TEST(Insert)
	{
		AAX_CString	inputString("AAAA");
		inputString.Insert(1, AAX_CString("B"));
		CHECK_EQUAL("ABAAA", inputString.CString());		
		inputString.Insert(3, "C");
		CHECK_EQUAL("ABACAA", inputString.CString());		
		inputString.InsertNumber(5, 5);
		CHECK_EQUAL("ABACA5A", inputString.CString());		
		inputString.InsertNumber(7, 5.5, 1);
		CHECK_EQUAL("ABACA5A5.5", inputString.CString());		
	}
	
	TEST(Replace)
	{
		AAX_CString inputString("ABABA");
		inputString.Replace(2, 2, AAX_CString("CC"));
		CHECK_EQUAL("ABCCA", inputString.CString());
		inputString.Replace(3, 2, "DD");
		CHECK_EQUAL("ABCDD", inputString.CString());
		inputString.Replace(4, 1, AAX_CString("E"));
		CHECK_EQUAL("ABCDE", inputString.CString());
	}
	
	
	TEST(LessThanOperator)
	{
		AAX_CString str1("A");
		AAX_CString	str2("B");
		AAX_CString	str3("C");
		CHECK( str1 < str2);
		CHECK( str2 < str3);
		CHECK( AAX_CString() < str1);
	}
	
	TEST(GreaterThanOperator)
	{
		AAX_CString str1("A");
		AAX_CString	str2("B");
		AAX_CString	str3("C");
		CHECK( str3 > str2);
		CHECK( str2 > str1);
		CHECK( str1 > AAX_CString());
	}

	TEST(SubStr)
	{
		AAX_CString	inputString("HowdyThere");
		AAX_CString howdy;
		inputString.SubString(0, 5, &howdy);	
		AAX_CString there;
		inputString.SubString(5, 5, &there);
		CHECK_EQUAL("Howdy", howdy.CString());	
		CHECK_EQUAL("There", there.CString());
	}
	
	TEST(PlusEqualOperator)
	{
		AAX_CString howdy("Howdy");	
		AAX_CString there("There");
		CHECK_EQUAL("Howdy", howdy.CString());	
		CHECK_EQUAL("There", there.CString());	
		AAX_CString howdyThere = howdy;
		CHECK_EQUAL("Howdy", howdyThere.CString());
		howdyThere += there;
		CHECK_EQUAL("HowdyThere", howdyThere.CString());
		howdyThere.Clear();
		howdyThere = howdy;
		howdyThere += "There";	//const char* version
		CHECK_EQUAL("HowdyThere", howdyThere.CString());		
	}
	
	TEST(Erase)
	{
		AAX_CString	inputString("0123456789");
		inputString.Erase(5, 3);
		CHECK_EQUAL(uint32_t(7), inputString.Length());
		CHECK_EQUAL("0123489", inputString.CString());
		AAX_CString	shortString("01234");
		shortString.Erase(0, 10);
		CHECK_EQUAL("", shortString.CString());
		AAX_CString	shortString2("012345");
		shortString2.Erase(3, 10);
		CHECK_EQUAL("012", shortString2.CString());
	}
    
    TEST(stdString)
    {
        std::string  stdAStr("AString");
        std::string  stdBStr("BString");
        
        AAX_CString  aStr(stdAStr);
        AAX_CString  bStr(stdBStr);
        CHECK_EQUAL(aStr, stdAStr);
        CHECK_EQUAL(bStr, stdBStr);
        
        std::string  a = aStr.StdString();
        const std::string b = bStr.StdString();
        CHECK_EQUAL(a, stdAStr);
        CHECK_EQUAL(b, stdBStr);
        
        bStr = aStr;
        CHECK_EQUAL(aStr, bStr);
    }
	
}
