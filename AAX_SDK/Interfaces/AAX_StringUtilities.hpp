/*================================================================================================*/
/*
 *	Copyright 2014-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/

#ifndef AAXLibrary_AAX_StringUtilities_hpp
#define AAXLibrary_AAX_StringUtilities_hpp

#include "AAX_IString.h"
#include "AAX_Assert.h"

#include <algorithm>
#include <string>
#include <cstring>

//===========================================================================
//
//	FloatToString:	Convert the given floating point number to a pascal string.
//
//===========================================================================
/*
void	FloatToString(float aNumber, StringPtr aString)
{
	Str255			MantissaStr;
	double		aDouble;
	StringPtr		tempStr;
	int32_t			mantissa,tens,hundreds;
	int16_t			count;

	aDouble = (double) aNumber;
	if (aNumber < 0.0)  // take abs value
		aDouble = -aDouble;
	
	aDouble += 0.005;		// perform rounding by adding 1/2 of the hundreths digit
	
	mantissa = aDouble;
	tens = (aDouble * 10.0) - (mantissa * 10.0);
	hundreds = (aDouble * 100.0) - (mantissa * 100.0) - (tens * 10.0);

	NumToString(mantissa, MantissaStr);
	
	// set up string length
	if (aNumber < 0.0)
		*aString++ = (char) (1 + 3 + *MantissaStr);
	else
		*aString++ = (char) (3 + *MantissaStr);
	
	tempStr = MantissaStr;
	
	// copy mantissa first
	count = *tempStr++;
	
	if (aNumber < 0.0)
		*aString++ = '-';

	while (count--)
		*aString++ = *tempStr++;

	*aString++ = '.';
	*aString++ = (char) (tens + '0');
	*aString++ = (char) (hundreds + '0');
}
*/

///==============================================================
//
//  GetCStringOfLength
//
// A routine for selecting a string based on the size passed in
// by the client.  If none of the strings are short enough then
// the shortest string is truncated to fit.
//
// stringIn="A Very Nice String\nA String\nAStrng\nStr\n";
//
// Submitted from Erik Gavriluk of BombFactory (Free of Charge)
// Debugged and separator character changed by Frederick Umminger
//===============================================================

void AAX::GetCStringOfLength(char *s_out, const char* s_in, int32_t aMaxChars)
{
	AAX_ASSERT(0 < aMaxChars);
	
	const char kSeparator = '\n';
	
	if(s_in)
	{
		const char* s_begin = s_in;
		const char* s_end = s_begin;
		while(s_begin)
		{
			// Count characters in current substring
			while((*s_end != kSeparator) && (*s_end != '\0'))
			{
				s_end++;
			};
			
			// If substring is less than or equal to aMaxChars then use it.
			if((s_end-s_begin <= aMaxChars) || (*s_end=='\0'))
			{
				break;
			}
			
			s_begin = ++s_end;
		}
		// We don't use strncpy in order to make sure a '\0' gets put on the end of s_out
		*s_out = '\0';
		const int32_t length = int32_t(s_end-s_begin);
		if (0 < length && 0 < aMaxChars)
		{
			strncat(s_out, s_begin, static_cast<size_t>(std::min(aMaxChars,length)));
		}
	}
	else if (0 < aMaxChars)
	{
		strncpy(s_out, "", static_cast<size_t>(aMaxChars));
	};
}

int32_t AAX::Caseless_strcmp(const char* cs, const char* ct)
{
	if(cs)
	{
		if(ct)
		{
			while(*cs && *ct)
			{
				int32_t cmp = toupper(*ct++) - toupper(*cs++);
				if(cmp) return cmp;
			};
			if(*cs)
			{
				return -1;
			}
			else
			{
				if(*ct)
				{
					return 1;
				}
				else
				{
					return 0;
				};
			};
		}
		else
		{
			return -1;
		};
	}
	else
	{
		if(ct)
			return 1;
		else
			return 0;
	}
	
}


std::string AAX::Binary2String(uint32_t value, int32_t numBits)
{
	std::string s;
	
	uint32_t currentBitMask = (static_cast<uint32_t>(0x1) << (numBits-1));
	
	while (currentBitMask != 0)
	{
		if (currentBitMask & value)
		{
			s += "1";
		}
		else
		{
			s += "0";
		};
		currentBitMask >>= 1;
	}
	return s;
}

uint32_t AAX::String2Binary(const AAX_IString& s)
{
	uint32_t value = 0;
	
	const char* const cS = s.Get();
	int32_t length = int32_t(s.Length());
	for(int32_t i = 0; i < length ; i++)
	{
		switch(cS[i])
		{
			case '0':
				break;
			case '1':
				value |= (0x1 << (length-1-i));
				break;
			default:
				AAX_ASSERT('0' == cS[i] || '1' == cS[i]);
		};
	};
	
	return value;
}

bool AAX::IsASCII(char inChar)
{
	return (0x20 <= inChar) && (0x7E >= inChar);
}

bool AAX::IsFourCharASCII(uint32_t inFourChar)
{
	const uint32_t oneCharMask = 0x000000FF;
	const size_t oneCharNumBits = 8;
	
	bool result = true;
	for (int16_t i = 3; (i >= 0) && (true == result); --i)
	{
		const char curChar = (inFourChar >> (i*oneCharNumBits)) & oneCharMask;
		result = result && IsASCII(curChar);
	}
	return result;
}

std::string AAX::AsStringFourChar(uint32_t inFourChar)
{
	const uint32_t oneCharMask = 0x000000FF;
	const int16_t oneCharNumBits = 8;
	const std::string unknownChar = "(?)";
	
	std::string resultStr("");
	for (int16_t i = 3; i >= 0; --i)
	{
		const char curChar = static_cast<char>((inFourChar >> (i*oneCharNumBits)) & oneCharMask);
		resultStr += IsASCII(curChar) ? std::string(1, curChar) : unknownChar;
	}
	return resultStr;
}

std::string AAX::AsStringPropertyValue(AAX_EProperty inProperty, AAX_CPropertyValue inPropertyValue)
{
	// 63 characters should be plenty for any of the formats below, but
	// increase this value if the formats are expanded in the future.
	const size_t kBufferSize = 64;
	char buffer[kBufferSize];
	
	// Attempt to infer a sensible way to print the property
	if (AAX_eProperty_SampleRate == inProperty ||
		AAX_eProperty_Constraint_Location == inProperty)
	{
		// Print specific properties' values as bitfield
		
		// We want the exact bits, so we memcpy to avoid any potential issues
		// with casting from signed to unsigned
		uint32_t bitfield;
		memcpy(&bitfield, &inPropertyValue, sizeof(uint32_t));
		
		const int32_t maxNumBitsToShow = 8; // Currently there are no bitfield properties with more than 8 possible flags
		sprintf(buffer, "%s", AAX::Binary2String(bitfield, maxNumBitsToShow).c_str());
	}
	else if (true == AAX::IsFourCharASCII(inPropertyValue))
	{
		// Print values in ASCII range as four-char
		sprintf(buffer, "'%s'", AAX::AsStringFourChar(inPropertyValue).c_str());
	}
	else if (0x00FFFFFF < std::abs(inPropertyValue))
	{
		// Print values with most bits used as hex
		sprintf(buffer, "%#x", inPropertyValue);
	}
	else
	{
		// Otherwise, print as simple decimal
		sprintf(buffer, "%ld", (long int)inPropertyValue);
	}
	
	buffer[kBufferSize-1] = '\0'; // Just in case
	return std::string(buffer);
}

std::string	AAX::AsStringInt32(int32_t inInt32)
{
	const size_t kBufferSize = 16;
	char buffer[kBufferSize];
	sprintf(buffer, "%ld", (long int)inInt32);
	return std::string(buffer);
}

std::string	AAX::AsStringUInt32(uint32_t inUInt32)
{
	const size_t kBufferSize = 16;
	char buffer[kBufferSize];
	sprintf(buffer, "%lu", (unsigned long)inUInt32);
	return std::string(buffer);
}

std::string AAX::AsStringIDTriad(const AAX_SPlugInIdentifierTriad& inIDTriad)
{
	std::string result = "(";
	
	result += "man: '" + AAX::AsStringFourChar(inIDTriad.mManufacturerID) + "', ";
	result += "prod: '" + AAX::AsStringFourChar(inIDTriad.mProductID) + "', ";
	result += "type: '" + AAX::AsStringFourChar(inIDTriad.mPlugInID) + "'";
	
	result += ")";
	return result;
}

#endif
