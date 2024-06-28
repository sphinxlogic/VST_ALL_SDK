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
 *	\file  AAX_VController.h
 *
 *	\brief Version-managed concrete Controller class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VCONTROLLER_H
#define AAX_VCONTROLLER_H

#include "AAX_IController.h"
#include "AAX_IACFController.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-assign"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "ACFPtr.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

class IACFUnknown;

/*!
 \brief Version-managed concrete \ref AAX_IController "Controller" class
 
 \details
 For usage information, see \ref using_acf_host_provided_interfaces
 
 */
class AAX_VController : public AAX_IController
{
public:
	AAX_VController( IACFUnknown* pUnknown );
	virtual ~AAX_VController();
	
	//Host Information Getters
	virtual AAX_Result	GetEffectID ( AAX_IString *	oEffectID) const;
	virtual AAX_Result	GetSampleRate ( AAX_CSampleRate * oSampleRate ) const;
	virtual AAX_Result	GetInputStemFormat ( AAX_EStemFormat * oStemFormat ) const;
	virtual AAX_Result	GetOutputStemFormat ( AAX_EStemFormat * oStemFormat ) const;
	virtual AAX_Result	GetSignalLatency( int32_t* oSamples) const;
	virtual AAX_Result	GetHybridSignalLatency(int32_t* oSamples) const;
	virtual AAX_Result	GetPlugInTargetPlatform(AAX_CTargetPlatform* oTargetPlatform) const;
	virtual AAX_Result	GetIsAudioSuite(AAX_CBoolean* oIsAudioSuite) const;

	virtual AAX_Result	GetCycleCount( AAX_EProperty iWhichCycleCount, AAX_CPropertyValue* oNumCycles) const;
	virtual AAX_Result	GetTODLocation ( AAX_CTimeOfDay* oTODLocation ) const;
	virtual AAX_Result	GetCurrentAutomationTimestamp(AAX_CTransportCounter* oTimestamp) const;
	virtual AAX_Result	GetHostName(AAX_IString* oHostNameString) const;

	//Host Information Setters (Dynamic info)
	virtual AAX_Result	SetSignalLatency(int32_t numSamples);
	virtual AAX_Result	SetCycleCount( AAX_EProperty* iWhichCycleCounts, AAX_CPropertyValue* iValues, int32_t numValues);
	
	//Posting functions.
	virtual AAX_Result	PostPacket ( AAX_CFieldIndex iFieldIndex, const void * iPayloadP, uint32_t iPayloadSize );
	
	// Notification functions
	virtual	AAX_Result	SendNotification ( AAX_CTypeID iNotificationType, const void* iNotificationData, uint32_t iNotificationDataSize );
	
	//Metering functions
	virtual AAX_Result	GetCurrentMeterValue ( AAX_CTypeID iMeterID, float * oMeterValue ) const;
	virtual AAX_Result	GetMeterPeakValue( AAX_CTypeID iMeterID, float * oMeterPeakValue ) const;
	virtual AAX_Result	ClearMeterPeakValue ( AAX_CTypeID iMeterID ) const;
	virtual AAX_Result	GetMeterClipped ( AAX_CTypeID iMeterID, AAX_CBoolean * oClipped ) const;
	virtual AAX_Result	ClearMeterClipped ( AAX_CTypeID iMeterID ) const;
	virtual AAX_Result	GetMeterCount ( uint32_t * oMeterCount ) const;

	//MIDI functions
	virtual AAX_Result	GetNextMIDIPacket( AAX_CFieldIndex* oPort, AAX_CMidiPacket* oPacket );

private:
	ACFPtr<AAX_IACFController>	mIController;
	ACFPtr<AAX_IACFController_V2>	mIControllerV2;
	ACFPtr<AAX_IACFController_V3>	mIControllerV3;
};


#endif // AAX_VCONTROLLER_H

