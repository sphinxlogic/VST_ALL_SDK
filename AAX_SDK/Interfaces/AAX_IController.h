/*================================================================================================*/
/*
 *
 *	Copyright 2013-2016 by Avid Technology, Inc.
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
 *	\file  AAX_IController.h
 *
 *	\brief Interface for the %AAX host's view of a single instance of an effect
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_ICONTROLLER_H_
#define _AAX_ICONTROLLER_H_

#include "AAX_Properties.h"
#include "AAX_IString.h"
#include "AAX.h"

/**
 *	\brief 	Interface for the %AAX host's view of a single instance of an effect.
 *			Used by both clients of the %AAX host and by effect components.
 *
 *	\details
 *	\hostimp
 */
class AAX_IController
{
public:	
	
		virtual ~AAX_IController(void) {}
	
		/** @name Host information getters
		 *
		 *	Call these methods to retrieve environment and run-time information from the %AAX host.
		 */
		//@{
		virtual AAX_Result	GetEffectID (
			AAX_IString *	oEffectID) const = 0;
		/*!
		 *	\brief CALL: Returns the current literal sample rate
		 *
		 *	\param[out] oSampleRate
		 *		The current sample rate
		 */
		virtual // AAX_VController
		AAX_Result
		GetSampleRate (
			AAX_CSampleRate *oSampleRate ) const = 0;
		/*!
		 *	\brief CALL: Returns the plug-in's input stem format
		 *
		 *	\param[out] oStemFormat
		 *		The current input stem format
		 */
		virtual // AAX_VController
		AAX_Result
		GetInputStemFormat (
			AAX_EStemFormat *oStemFormat ) const = 0;
		/*!
		 *	\brief CALL: Returns the plug-in's output stem format
		 *
		 *	\param[out] oStemFormat
		 *		The current output stem format
		 */
		virtual // AAX_VController
		AAX_Result
		GetOutputStemFormat (
			AAX_EStemFormat *oStemFormat) const = 0;
		/*!
		 *	\brief CALL: Returns the most recent signal (algorithmic) latency that has been
		 *	published by the plug-in
		 *	
		 *	This method provides the most recently published signal latency. The host may not
		 *	have updated its delay compensation to match this signal latency yet, so plug-ins
		 *	that dynamically change their latency using
		 *	\ref AAX_IController::SetSignalLatency() "SetSignalLatency()" should always wait for
		 *	an \ref AAX_eNotificationEvent_SignalLatencyChanged notification before updating its
		 *	algorithm to incur this latency.
		 *
		 *	\sa \ref AAX_IController::SetSignalLatency() "SetSignalLatency()"
		 *
		 *	\param[out] oSamples
		 *		The number of samples of signal delay published by the plug-in
		 */
		virtual 
		AAX_Result	
		GetSignalLatency(
			int32_t* oSamples) const = 0;
		/*!
		 *	\brief CALL: returns the plug-in's current real-time DSP cycle count
		 *
		 *	This method provides the number of cycles that the %AAX host expects the DSP plug-in
		 *	to consume.  The host uses this value when allocating DSP resources for the plug-in.
		 *
		 *	\note A plug-in should never apply a DSP algorithm with more demanding resource
		 *	requirements than what is currently accounted for by the host.  To set a higher cycle
		 *	count value, a plug-in must call \ref AAX_IController::SetCycleCount(), then poll
		 *	\ref AAX_IController::GetCycleCount() until the new value has been applied.  Once the
		 *	host has recognized the new cycle count value, the plug-in may apply the more
		 *	demanding algorithm.
		 *
		 *	\param[in] iWhichCycleCount
		 *		Selector for the requested cycle count metric.  One of:
		 *			\li \ref AAX_eProperty_TI_SharedCycleCount
		 *			\li \ref AAX_eProperty_TI_InstanceCycleCount
		 *			\li \ref AAX_eProperty_TI_MaxInstancesPerChip
		 *	\param[in] oValue
		 *		The current value of the selected cycle count metric
		 *
		 *	\todo PLACEHOLDER - NOT CURRENTLY IMPLEMENTED IN HOST
		 */		
		virtual
		AAX_Result
		GetCycleCount(
			AAX_EProperty iWhichCycleCount,
			AAX_CPropertyValue* oValue) const = 0;
		/*!
		 *	\brief CALL: Returns the current Time Of Day (TOD) of the system
		 *
		 *	This method provides a plug-in the TOD (in samples) of the current system. TOD is the
		 *	number of samples that the playhead has traversed since the beginning of playback.
		 *
		 *	\note The TOD value is the immediate value of the audio engine playhead. This value is
		 *	incremented within the audio engine's real-time rendering context; it is not
		 *	synchronized with non-real-time calls to plug-in interface methods.
		 *
		 *	\param[out] oTODLocation
		 *		The current Time Of Day as set by the host
		 */
		virtual
		AAX_Result
		GetTODLocation (
			AAX_CTimeOfDay* oTODLocation ) const = 0;
		//@} Host information accessors
		
		/** @name Host information setters
		 *
		 *	Call these methods to set dynamic plug-in run-time information on the %AAX host.
		 */
		//@{
		/*!
		 *	\brief CALL: Submits a request to change the delay compensation value that the host uses
		 *	to account for the plug-in's signal (algorithmic) latency
		 *
		 *	This method is used to request a change in the number of samples that the %AAX host
		 *	expects the plug-in to delay a signal.
		 *
		 *	The host is not guaranteed to immediately apply the new latency value. A plug-in should
		 *	avoid incurring an actual algorithmic latency that is different than the latency accounted
		 *	for by the host.
		 *
		 *	To set a new latency value, a plug-in must call \ref AAX_IController::SetSignalLatency(),
		 *	then wait for an \ref AAX_eNotificationEvent_SignalLatencyChanged notification. Once this
		 *	notification has been received, \ref AAX_IController::GetSignalLatency() will reflect the
		 *	updated latency value and the plug-in should immediately apply any relevant algorithmic
		 *	changes that alter its latency to this new value.
		 *
		 *	\param[in] iNumSamples
		 *		The number of samples of signal delay that the plug-in requests to incur
		 */
		virtual 
		AAX_Result	
		SetSignalLatency(
			int32_t iNumSamples) = 0;
		/*!
		 *	\brief CALL: Indicates a change in the plug-in's real-time DSP cycle count
		 *
		 *	This method is used to request a change in the number of cycles that the %AAX host
		 *	expects the DSP plug-in to consume.
		 *
		 *	\note A plug-in should never apply a DSP algorithm with more demanding resource
		 *	requirements than what is currently accounted for by the host.  To set a higher cycle
		 *	count value, a plug-in must call \ref AAX_IController::SetCycleCount(), then poll
		 *	\ref AAX_IController::GetCycleCount() until the new value has been applied.  Once the
		 *	host has recognized the new cycle count value, the plug-in may apply the more
		 *	demanding algorithm.
		 *
		 *	\param[in] iWhichCycleCounts
		 *		Array of selectors indicating the specific cycle count metrics that should be set.
		 *		Each selector must be one of:
		 *			\li \ref AAX_eProperty_TI_SharedCycleCount
		 *			\li \ref AAX_eProperty_TI_InstanceCycleCount
		 *			\li \ref AAX_eProperty_TI_MaxInstancesPerChip
		 *	\param[in] iValues
		 *		An array of values requested, one for each of the selected cycle count metrics.
		 *	\param[out] oNumValues
		 *		The number of values provided
		 *
		 *	\todo PLACEHOLDER - NOT CURRENTLY IMPLEMENTED IN HOST
		 */		
		virtual
		AAX_Result
		SetCycleCount(
			AAX_EProperty* iWhichCycleCounts,
			AAX_CPropertyValue* iValues,
			int32_t oNumValues) = 0;
		//@} Host information setters

		/** @name Posting methods
		 *
		 *	Call these methods to post new plug-in information to the host's data management system.
		 */
		//@{
		/*!
		 *	\brief CALL: Posts a data packet to the host for routing between plug-in components
		 *
		 *	The posted packet is identified with a \ref AAX_CFieldIndex packet index value, which is
		 *	equivalent to the target data port's identifier.  The packet's
		 *	payload must have the expected size for the given packet index / data port, as defined
		 *	when the port is created in \ref CommonInterface_Describe "Describe".
		 *	See AAX_IComponentDescriptor::AddDataInPort().
		 *
		 *	\warning Any data structures that will be passed between platforms (for example, sent to
		 *	a TI DSP in an %AAX DSP plug-in) must be properly data-aligned for compatibility across
		 *	both platforms. See \ref AAX_ALIGN_FILE_ALG for more information about guaranteeing
		 *	cross-platform compatibility of data structures used for algorithm processing.
		 *
		 *	\note All calls to this method should be made within the scope of
		 *	\ref AAX_IEffectParameters::GenerateCoefficients(). Calls from outside this method may
		 *	result in packets not being delivered. See \ref PT-206161
		 *
		 *	\param[in] iFieldIndex
		 *		The packet's destination port
		 *	\param[in] iPayloadP
		 *		A pointer to the packet's payload data
		 *	\param[in] iPayloadSize
		 *		The size, in bytes, of the payload data
		 */
		virtual // AAX_VController
		AAX_Result
		PostPacket (
			AAX_CFieldIndex	iFieldIndex,
			const void *	iPayloadP, 
			uint32_t		iPayloadSize) = 0;
		//@} Posting methods

		/** @name Notification methods
		 *
		 *	Call these methods to send events among plug-in components
		 */
		//@{
		/*!
		 *	\brief CALL: Sends an event to the GUI.  
         *
         *   This is supported by %AAX V2 Hosts only.  Check the return code
         *   on the return of this function.  If the error is AAX_ERROR_UNIMPLEMENTED, your plug-in is being loaded
         *   into a host that doesn't support this feature.
		 *
		 *	\param[in] iNotificationType
		 *		Type of notification to send
		 *	\param[in] iNotificationData
		 *		Block of notification data
		 *	\param[in] iNotificationDataSize
		 *		Size of \p iNotificationData, in bytes
		 */
		virtual	AAX_Result	SendNotification (/* AAX_ENotificationEvent */ AAX_CTypeID iNotificationType, const void* iNotificationData, uint32_t iNotificationDataSize) = 0;
		//@} Notification methods

		/** @name Metering methods
		 *
		 *	Methods to access the plug-in's host-managed metering information.
		 *
		 *	\sa \ref AdditionalFeatures_Meters
		 */
		//@{
		/*!
		 *	\brief CALL: Retrieves the current value of a host-managed plug-in meter
		 *
		 *	\param[in] iMeterID
		 *		ID of the meter that is being queried
		 *	\param[out] oMeterValue
		 *		The queried meter's current value
		 */
		virtual AAX_Result	GetCurrentMeterValue ( AAX_CTypeID iMeterID, float * oMeterValue ) const = 0;
		/*!
		 *	\brief CALL: Retrieves the currently held peak value of a host-managed plug-in meter
		 *
		 *	\param[in] iMeterID
		 *		ID of the meter that is being queried
		 *	\param[out] oPeakValue
		 *		The queried meter's currently held peak value
		 */
		virtual AAX_Result	GetMeterPeakValue ( AAX_CTypeID iMeterID, float * oPeakValue ) const = 0;
		/*!
		 *	\brief CALL: Clears the peak value from a host-managed plug-in meter
		 *
		 *	\param[in] iMeterID
		 *		ID of the meter that is being cleared
		 */
		virtual AAX_Result	ClearMeterPeakValue ( AAX_CTypeID iMeterID ) const = 0;
		/*!
		 *	\brief CALL: Retrieves the number of host-managed meters registered by a plug-in.
		 *
		 *	See AAX_IComponentDescriptor::AddMeters().
		 *
		 *	\param[out] oMeterCount
		 *		The number of registered plug-in meters.
		 */
		virtual AAX_Result	GetMeterCount ( uint32_t * oMeterCount ) const = 0;
		/*!
		*	\brief CALL: Retrieves the clipped flag from a host-managed plug-in meter.
		*
		*	See AAX_IComponentDescriptor::AddMeters().
		*
		*	\param[in] iMeterID
		*		ID of the meter that is being queried.
		*	\param[out] oClipped
		*		The queried meter's clipped flag.
		*/
		virtual AAX_Result	GetMeterClipped ( AAX_CTypeID iMeterID, AAX_CBoolean * oClipped ) const = 0;
		/*!
		*	\brief CALL: Clears the clipped flag from a host-managed plug-in meter.
		*
		*	See AAX_IComponentDescriptor::AddMeters().
		*
		*	\param[in] iMeterID
		*		ID of the meter that is being cleared.
		*/
		virtual AAX_Result	ClearMeterClipped ( AAX_CTypeID iMeterID ) const = 0;
		//@} Metering methods


		/** @name MIDI methods
		 *
		 *	Methods to access the plug-in's host-managed MIDI information.
		 *
		 */
		//@{
		/*!
		 *	\brief CALL: Retrieves MIDI packets for described MIDI nodes.
		 *
		 *	\param[out] oPort
		 *		port ID of the MIDI node that has unhandled packet
		 *	\param[out] oPacket
		 *		The MIDI packet
		 */
		virtual AAX_Result	GetNextMIDIPacket ( AAX_CFieldIndex* oPort, AAX_CMidiPacket* oPacket ) = 0;
		//@} MIDI methods
		
        /*!
         *	\brief CALL: Returns the latency between the algorithm normal input samples and the inputs returning from the hyrbid component
         *
         *	This method provides the number of samples that the %AAX host expects the plug-in to delay
         *	a signal.  The host will use this value when accounting for latency across the system.
         *
         *	\note   This value will generally scale up with sample rate, although it's not a simple multiple due to some fixed overhead.
         *          This value will be fixed for any given sample rate regardless of other buffer size settings in the host app.
         *
         *	\param[out] oSamples
         *		The number of samples of hybrid signal delay
		 *
		 *	\ingroup additionalFeatures_Hybrid
         */
        virtual
        AAX_Result GetHybridSignalLatency(int32_t* oSamples) const = 0;
        /*!
         *	\brief CALL: Returns the current automation timestamp if called during the
		 *	\ref AAX_IACFEffectParameters::GenerateCoefficients() "GenerateCoefficients()" call AND the generation
		 *	of coefficients is being triggered by an automation point instead of immediate changes.
         *
         *	\note   This function will return 0 if called from outside of
		 *			\ref AAX_IACFEffectParameters::GenerateCoefficients() "GenerateCoefficients()" or if the
		 *			\ref AAX_IACFEffectParameters::GenerateCoefficients() "GenerateCoefficients()"
		 *			call was initiated due to a non-automated change.  In those cases, you can get your sample offset from the transport
		 *			start using \ref GetTODLocation().
         *
         *	\param[out] oTimestamp
         *		The current coefficient timestamp. Sample count from transport start.
         */
        virtual
        AAX_Result GetCurrentAutomationTimestamp(AAX_CTransportCounter* oTimestamp) const = 0;
        /*!
         *	\brief CALL: Returns name of the host application this plug-in instance is being loaded by.  This string also typically includes version information.
         *
		 *	\compatibility Pro Tools versions from Pro Tools 11.0 to Pro Tools 12.3.1 will return a generic
		 *	version string to this call. This issue is resolved beginning in Pro Tools 12.4.
		 *
         *	\param[out] oHostNameString
         *		The name of the current host application.
         */
        virtual
        AAX_Result GetHostName(AAX_IString* oHostNameString) const = 0;
        /*!
         *	\brief CALL: Returns execution platform type, native or TI
         *
         *	\param[out] oTargetPlatform
         *		The type of the current execution platform as one of \ref AAX_ETargetPlatform.
         */
		virtual
		AAX_Result GetPlugInTargetPlatform(AAX_CTargetPlatform* oTargetPlatform) const = 0;
           /*!
         *	\brief CALL: Returns true for AudioSuite instances
         *
         *	\param[out] oIsAudioSuite
         *		The boolean flag which indicate true for AudioSuite instances.
         */
		virtual 
		AAX_Result GetIsAudioSuite(AAX_CBoolean* oIsAudioSuite) const = 0;
	
};

#endif // #ifndef _AAX_IPLUGIN_H_
