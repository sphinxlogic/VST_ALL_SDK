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
 *	\file  AAX_IACFController.h
 *
 *	\brief Interface for the %AAX host's view of a single instance of an 
 *	effect.  Used by both clients of the AAXHost and by effect components.
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_IACFCONTROLLER_H_
#define _AAX_IACFCONTROLLER_H_

#include "AAX.h"
#include "AAX_IString.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/** \brief Interface for the %AAX host's view of a single instance of an 
 *	effect.  Used by both clients of the AAXHost and by effect components.
 */
class AAX_IACFController : public IACFUnknown
{
public:	
			
		// Host information getters
		/** \copydoc AAX_IController::GetEffectID() */
		virtual 
		AAX_Result
		GetEffectID (
			AAX_IString *	oEffectID) const = 0;
		
		/** \copydoc AAX_IController::GetSampleRate() */
		virtual
		AAX_Result
		GetSampleRate (
			AAX_CSampleRate *oSampleRate ) const = 0;

		/** \copydoc AAX_IController::GetInputStemFormat() */
		virtual
		AAX_Result
		GetInputStemFormat (
			AAX_EStemFormat *oStemFormat ) const = 0;

		/** \copydoc AAX_IController::GetOutputStemFormat() */
		virtual
		AAX_Result
		GetOutputStemFormat (
			AAX_EStemFormat *oStemFormat) const = 0;
	 
		/** \copydoc AAX_IController::GetSignalLatency() */
		virtual 
		AAX_Result	
		GetSignalLatency( 
			int32_t* oSamples) const = 0;
	
		/** \copydoc AAX_IController::GetCycleCount() */
		virtual
		AAX_Result
		GetCycleCount(
			AAX_EProperty iWhichCycleCount,
			AAX_CPropertyValue* oValue) const = 0;

		/** \copydoc AAX_IController::GetTODLocation() */
		virtual
		AAX_Result 
		GetTODLocation ( 
			AAX_CTimeOfDay* oTODLocation ) const = 0;

		//Host Information Setters (Dynamic info)
		/** \copydoc AAX_IController::SetSignalLatency() */
		virtual 
		AAX_Result	
		SetSignalLatency(
			int32_t iNumSamples) = 0;	
	
		/** \copydoc AAX_IController::SetCycleCount() */
		virtual
		AAX_Result
		SetCycleCount(
			AAX_EProperty* iWhichCycleCounts,
			AAX_CPropertyValue* iValues,
			int32_t oNumValues) = 0;
		
		// Posting functions
		/** \copydoc AAX_IController::PostPacket() */
		virtual 
		AAX_Result
		PostPacket (
			AAX_CFieldIndex		iFieldIndex,
			const void *	iPayloadP, 
			uint32_t iPayloadSize) = 0;

		//Metering functions
		/** \copydoc AAX_IController::GetCurrentMeterValue() */
		virtual 
		AAX_Result	
		GetCurrentMeterValue ( 
			AAX_CTypeID iMeterID,
			float * oMeterValue ) const = 0;
	
		/** \copydoc AAX_IController::GetMeterPeakValue() */
		virtual
		AAX_Result
		GetMeterPeakValue ( 
			AAX_CTypeID iMeterID,
			float * oPeakValue ) const = 0;
	
		/** \copydoc AAX_IController::ClearMeterPeakValue() */
		virtual
		AAX_Result
		ClearMeterPeakValue ( 
			AAX_CTypeID iMeterID ) const = 0;

		/** \copydoc AAX_IController::GetMeterClipped() */
		virtual
		AAX_Result
		GetMeterClipped ( 
			AAX_CTypeID iMeterID,
			AAX_CBoolean * oClipped ) const = 0;

		/** \copydoc AAX_IController::ClearMeterClipped() */
		virtual
		AAX_Result
		ClearMeterClipped ( 
			AAX_CTypeID iMeterID ) const = 0;
	
		/** \copydoc AAX_IController::GetMeterCount() */
		virtual 
		AAX_Result	
		GetMeterCount ( 
			uint32_t * oMeterCount ) const = 0;

		// MIDI methods
		/** \copydoc AAX_IController::GetNextMIDIPacket() */
		virtual
		AAX_Result
		GetNextMIDIPacket (
			AAX_CFieldIndex* oPort,
			AAX_CMidiPacket* oPacket ) = 0;

	};

/** @copydoc AAX_IController
 */
class AAX_IACFController_V2 : public AAX_IACFController
{
public:
	// Notification method
	/** \copydoc AAX_IController::SendNotification() */
	virtual 
	AAX_Result 
	SendNotification (
		AAX_CTypeID iNotificationType,
		const void* iNotificationData,
		uint32_t iNotificationDataSize) = 0;

    /** \copydoc AAX_IController::GetHybridSignalLatency() */
	virtual
    AAX_Result
    GetHybridSignalLatency(
    	int32_t* oSamples) const = 0;
    
    /** \copydoc AAX_IController::GetCurrentAutomationTimestamp() */
	virtual
    AAX_Result
    GetCurrentAutomationTimestamp(
    	AAX_CTransportCounter* oTimestamp) const = 0;
    
    /** \copydoc AAX_IController::GetHostName() */
	virtual
	AAX_Result
	GetHostName(
		AAX_IString* oHostNameString) const = 0;
};

class AAX_IACFController_V3 : public AAX_IACFController_V2
{
public:
	/** \copydoc AAX_IController::GetPlugInTargetPlatform() */
	virtual
	AAX_Result
	GetPlugInTargetPlatform(
		AAX_CTargetPlatform* oTargetPlatform) const = 0;

	/** \copydoc AAX_IController::GetIsAudioSuite() */
	virtual
	AAX_Result
	GetIsAudioSuite(AAX_CBoolean* oIsAudioSuite) const = 0;
};
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // #ifndef _AAX_IACFCONTROLLER_H_
