/*================================================================================================*/
/*
 *
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
 *	\file  AAX_IACFPrivateDataAccess.h
 *
 *	\brief Interface for the %AAX host's data access functionality.
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_IACFPrivateDATAACCESS_H_
#define _AAX_IACFPrivateDATAACCESS_H_

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/** \brief Interface for the %AAX host's data access functionality.
 	
 	\details
	\ingroup AuxInterface_DirectData
 */
class AAX_IACFPrivateDataAccess : public IACFUnknown
{
public:	
			
	virtual AAX_Result	ReadPortDirect( AAX_CFieldIndex iFieldIndex, const uint32_t iOffset, const uint32_t iSize, void* oBuffer ) = 0;	///< \copydoc AAX_IPrivateDataAccess::ReadPortDirect()
	virtual AAX_Result	WritePortDirect( AAX_CFieldIndex iFieldIndex, const uint32_t iOffset, const uint32_t iSize, const void* iBuffer ) = 0;	///< \copydoc AAX_IPrivateDataAccess::WritePortDirect()
	
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // #ifndef _AAX_IACFPrivateDATAACCESS_H_
