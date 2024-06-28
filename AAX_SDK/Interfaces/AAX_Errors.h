/*================================================================================================*/
/*
 *
 *	Copyright 2010-2015 by Avid Technology, Inc.
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
 *	\file AAX_Errors.h
 *
 *	\brief Definitions of error codes used by %AAX plug-ins
 *
 */ 
/*================================================================================================*/


/// @cond ignore
#ifndef AAX_ERRORS_H
#define AAX_ERRORS_H
/// @endcond

#include "AAX_Enums.h"

enum AAX_EError
{
	AAX_SUCCESS												= 0,
	
	AAX_ERROR_INVALID_PARAMETER_ID							= -20001,
	AAX_ERROR_INVALID_STRING_CONVERSION						= -20002,
	AAX_ERROR_INVALID_METER_INDEX							= -20003,
	AAX_ERROR_NULL_OBJECT									= -20004,
	AAX_ERROR_OLDER_VERSION									= -20005,
	AAX_ERROR_INVALID_CHUNK_INDEX							= -20006,
	AAX_ERROR_INVALID_CHUNK_ID								= -20007,
	AAX_ERROR_INCORRECT_CHUNK_SIZE							= -20008,
	AAX_ERROR_UNIMPLEMENTED									= -20009,
	AAX_ERROR_INVALID_PARAMETER_INDEX						= -20010,
	AAX_ERROR_NOT_INITIALIZED								= -20011,
	AAX_ERROR_ACF_ERROR										= -20012,
	AAX_ERROR_INVALID_METER_TYPE							= -20013,
	AAX_ERROR_CONTEXT_ALREADY_HAS_METERS					= -20014,
	AAX_ERROR_NULL_COMPONENT								= -20015,
	AAX_ERROR_PORT_ID_OUT_OF_RANGE							= -20016,
	AAX_ERROR_FIELD_TYPE_DOES_NOT_SUPPORT_DIRECT_ACCESS		= -20017,
	AAX_ERROR_DIRECT_ACCESS_OUT_OF_BOUNDS					= -20018,
	AAX_ERROR_FIFO_FULL										= -20019,
	AAX_ERROR_INITIALIZING_PACKET_STREAM_THREAD				= -20020,
	AAX_ERROR_POST_PACKET_FAILED							= -20021,
	AAX_RESULT_PACKET_STREAM_NOT_EMPTY						= -20022,
	AAX_RESULT_ADD_FIELD_UNSUPPORTED_FIELD_TYPE				= -20023,
	AAX_ERROR_MIXER_THREAD_FALLING_BEHIND					= -20024,
	AAX_ERROR_INVALID_FIELD_INDEX							= -20025,
	AAX_ERROR_MALFORMED_CHUNK								= -20026,
	AAX_ERROR_TOD_BEHIND									= -20027,
	AAX_RESULT_NEW_PACKET_POSTED							= -20028,
	AAX_ERROR_PLUGIN_NOT_AUTHORIZED                         = -20029,    //return this from EffectInit() if the plug-in doesn't have proper license.
	AAX_ERROR_PLUGIN_NULL_PARAMETER							= -20030,
	AAX_ERROR_NOTIFICATION_FAILED							= -20031,
	AAX_ERROR_INVALID_VIEW_SIZE 							= -20032,
	AAX_ERROR_SIGNED_INT_OVERFLOW							= -20033,
	AAX_ERROR_NO_COMPONENTS									= -20034,
	AAX_ERROR_DUPLICATE_EFFECT_ID							= -20035,
	AAX_ERROR_DUPLICATE_TYPE_ID								= -20036,
	AAX_ERROR_EMPTY_EFFECT_NAME								= -20037,
	AAX_ERROR_UNKNOWN_PLUGIN								= -20038,
	AAX_ERROR_PROPERTY_UNDEFINED							= -20039,
	AAX_ERROR_INVALID_PATH									= -20040,
	
	AAX_ERROR_PLUGIN_BEGIN									= -20600, ///< Custom plug-in error codes may be placed in the range [ \ref AAX_ERROR_PLUGIN_BEGIN, \ref AAX_ERROR_PLUGIN_END )
	AAX_ERROR_PLUGIN_END									= -21000  ///< Custom plug-in error codes may be placed in the range [ \ref AAX_ERROR_PLUGIN_BEGIN, \ref AAX_ERROR_PLUGIN_END )
}; AAX_ENUM_SIZE_CHECK( AAX_EError );

/*

////<DT>  Old ones below 

//
// NOTE: (Undefined) comments for an error code mean that it's
// either no longer supported or returned from another source
// other than DAE.
//
												
//----------------------------------------------------------------------------
// Error codes for all of Fic
//----------------------------------------------------------------------------

enum {
	AAX_ERROR_NoCodeResourceErr			= -9000,	// Unable to load DSP code from DAE's resource fork.
	AAX_ERROR_CodeLoadedErr				= -9001,	// (Undefined)
	AAX_ERROR_NoCodeLoadedErr			= -9002,	// Trying to communicate with DSP's without code.
	AAX_ERROR_HostTimeoutErr			= -9003,	// Host Timeout Error.  DSP is not responding.
	AAX_ERROR_HostBusyErr				= -9004,	// (Undefined)
	AAX_ERROR_TransferTimeOutErr		= -9005,	// DSP data transfer timeout error.
	AAX_ERROR_LowMemoryErr				= -9006,	// DAE was unable to allocate memory.  Memory is low.
	AAX_ERROR_UnimplementedErr			= -9007,	// An unimplemented method was called.
	AAX_ERROR_AllocatedErr				= -9008,	// (Undefined)
	AAX_ERROR_TooManySlaveDSPsErr		= -9009,	// Too many DSP's trying to be slaved.
	AAX_ERROR_NoDSPGroupErr				= -9010,	// Tried to run a method when no DSP group exists.
	AAX_ERROR_EnabledErr				= -9011,	// Tried to execute code when interrupts were enabled.
	AAX_ERROR_AlreadyGroupedErr			= -9012,	// Tried to add a dsp object to a group when it's already part of one.
	AAX_ERROR_NILObjectErr				= -9013,	// Standard error return when an object is NULL.
	AAX_ERROR_NoDriverDSPErr			= -9014,	// Missing DSPPtr from the SADriver.
	AAX_ERROR_BadIndexErr				= -9015,	// Index to an array or list is invalid.
	AAX_ERROR_NoMasterDSPErr			= -9016,	// fMasterDSP field is NULL.
	AAX_ERROR_AlreadyDeferredErr		= -9017,	// Tried to install a deferred task when the task was already deferred.
	AAX_ERROR_NotAllocatedErr			= -9018,	// Unable to allocate Session 8 card or allocate extents in a file.
	AAX_ERROR_FileSystemBusyErr			= -9019,	// PB chain for an audio file returned an error for a disk task.
	AAX_ERROR_RunningErr				= -9020,	// Tried to execute code when the deck was started.
	AAX_ERROR_DSPConfigErr				= -9021,	// Error with the DSP codes configuration table.
	AAX_ERROR_TooManyItemsErr			= -9022,	// Number of needed items goes beyond a lists max size.
	AAX_ERROR_ItemNotFoundErr			= -9023,	// Unable to find an object in a list of objects.
	AAX_ERROR_WrongTypeErr				= -9024,	// Type value not found or not supported.
	AAX_ERROR_NoDeckErr					= -9025,	// Standard error returned from other objects that require a deck object.
	AAX_ERROR_NoSuchTickErr				= -9026,	// Invalid tick number when trying to create a tick transfer.
	AAX_ERROR_TooManyTransfersErr		= -9027,	// Trying to create too many tick transfers.
	AAX_ERROR_NoDSPErr					= -9028,	// Required DSP object is NULL.
	AAX_ERROR_NoFeederErr				= -9029,	// (Undefined)
	AAX_ERROR_NoOwnerErr				= -9030,	// Play or record track not owned by a channel.
	AAX_ERROR_PrimedErr					= -9031,	// Tried to execute code when the deck was primed.
	AAX_ERROR_AlreadyAttached			= -9032,	// DAE object already attached to another DAE object.
	AAX_ERROR_TooManyDSPTracksErr		= -9033,	// The user has run out of virtual tracks for a given card or dsp.
	AAX_ERROR_BadDSPTrackIDErr			= -9034,	// (Undefined)
	AAX_ERROR_ParseErr					= -9035,	// While trying to parse a data structure ran into an error.
	AAX_ERROR_CantGetMessageErr			= -9036,	// NULL messages while dealing with the Grey Matter sys axe.
	AAX_ERROR_NoReplyErr				= -9037,	// No reply from the sys axe.  Card is dead.
	AAX_ERROR_BadStatusErr				= -9038,	// Error status returned from a message sent to the sys axe.
	AAX_ERROR_ARoseOpenQueueErr			= -9039,	// OpenQueue for A/ROSE return NULL for the queue.
	AAX_ERROR_ARoseFindTaskErr			= -9040,	// A/ROSE Lookup_Task never returned a valid object.
	AAX_ERROR_NotAcquiredErr			= -9041,	// Tried to execute code when an object needs to be acquired first.
	AAX_ERROR_DiskAxeTimeOutErr			= -9042,	// Timeout occured while waiting for a status register on the sys axe.
	AAX_ERROR_NoMoreSlotsErr			= -9043,	// (Undefined)
	AAX_ERROR_NoDiskAxeVolumesErr		= -9044,	// No drives connected or mounted on the sys axe.
	AAX_ERROR_NoSSIClockErr				= -9045,	// DSP does not recieve peripheral clock interrupts.
	AAX_ERROR_SwitchTimeOutErr 			= -9046,	// Timeout occured while waiting for DAE to become the active app.
	AAX_ERROR_OnlyOneManagerErr			= -9047,	// (Undefined)
	AAX_ERROR_NotFound					= -9048,	// Missing DAE resource or timeout occured while waiting for DAE to launch.
	AAX_ERROR_NotOpenErr				= -9049,	// Audio file not opened when trying to access samples.
	AAX_ERROR_CantRecordErr				= -9050,	// Error returned when CanRecord() returns false.  Exp: Recording on scrub channel.
	AAX_ERROR_BadEffectNumErr			= -9051,	// Bad effects number index.
	AAX_ERROR_IncompatibleSRErr			= -9052,	// Hardware does not support sample rate.  Peripheral may need to be declared.
	AAX_ERROR_NoMorePBsErr				= -9053,	// No parameter block for an audio file object.
	AAX_ERROR_WrongObjectErr			= -9054,	// Object size or pointers do not match.
	AAX_ERROR_LowVersionErr				= -9055,	// Errors with version number too low.
	AAX_ERROR_HighVersionErr			= -9056,	// Errors with version number too high.
	AAX_ERROR_NotStartedErr				= -9057,	// Tried to execute code when the deck was not started yet.
	AAX_ERROR_ScriptedOnlyErr			= -9058,	// Error when something can only be done in a script mode.
	AAX_ERROR_Only1PunchInErr			= -9059,	// Error when deck can only support a single punch in.
	AAX_ERROR_AssertErr					= -9060,	// Generic error when a format does not match.
	AAX_ERROR_ScrubOnlyErr				= -9061,	// Tried to scrub in a non-scrub mode or on a sys axe channel.
	AAX_ERROR_NoSADriverErr				= -9062,	// InitSADriver failed.  Possible missing DigiSystem INIT.
	AAX_ERROR_BadFirstSyncLoc			= -9063,	// Error if the deck possition is greater than the assigned sync location.
	AAX_ERROR_CantFindDAEFolder			= -9064,	// Unable to find "DAE Folder" in the system folder.
	AAX_ERROR_CantFindDAEApp			= -9065,	// Unable to find DAE app in the DAE Folder.
	AAX_ERROR_Needs32BitModeErr			= -9066,	// DAE runs only in 32 bit mode.
	AAX_ERROR_NeedsSystem7Err			= -9067,	// DAE requires system 7 or higher software.
	AAX_ERROR_HatesVirtualMemErr		= -9068,	// DAE will not run if virtual memory is turned on.
	AAX_ERROR_Needs68020Err				= -9069,	// DAE requires at least a 68020 processor to run.
	AAX_ERROR_SCIConnectErr				= -9070,	// Unable to get SCI ports between two dsp's to communicate. 
	AAX_ERROR_SADriverVersionErr		= -9071,	// Unable to get DigiSystem INIT version or it's version is too low.
	AAX_ERROR_UserCancelledErr			= -9072,	// User chose to cancel or quit operation from DAE dialog.
	AAX_ERROR_DiskTooSlowErr			= -9073,	// Disk action did not complete in time for next command.
	AAX_ERROR_AudioTrackTooDense1		= -9074,	// Audio playlist is too dense.
	AAX_ERROR_AudioTrackTooDense2		= -9075,	// Audio playlist is too dense for silience play list.
	AAX_ERROR_CantDescribeZone			= -9076,	// Zone description is NULL.
	AAX_ERROR_CantApplyPlayLimits		= -9077,	// Ran out of time regions for a zone.
	AAX_ERROR_CantApplySkipMode			= -9078,	// Ran out of time regions for a zone in skip mode.
	AAX_ERROR_CantApplyLoop				= -9079,	// Ran out of time regions for a zone in loop mode.
	AAX_ERROR_NoDiskAxePlayElem			= -9080,	// Unable to find the first element in a play list to be sent to the sys axe.
	AAX_ERROR_BadDiskAxePlayElem		= -9081,	// Element for disk axe play list is out of range.
	AAX_ERROR_NoDiskAxeRecElem			= -9082,	// Unable to find the first element in a record list to be sent to the sys axe.
	AAX_ERROR_BadDiskAxeRecElem			= -9083,	// Element for disk axe record list is out of range.
	AAX_ERROR_AutoSortErr				= -9084,	// DSP event elements are not sorted in relation to time.
	AAX_ERROR_NoAutoEvent				= -9085,	// No event list for an auto parser.
	AAX_ERROR_AutoTrackTooDense1		= -9086,	// Automation event scripts are too dense.
	AAX_ERROR_AutoTrackTooDense2		= -9087,	// Ran out of free events for the automation parser.
	AAX_ERROR_NothingAllowedErr			= -9088,	// Missing allowed decks for the hw setup dialog.
	AAX_ERROR_HardwareNotFreeErr		= -9089,	// Unable to select a deck because the hardware is allocated or not available.
	AAX_ERROR_DSPsNotSyncedErr			= -9090,	// Timeout while waiting for a desired tick.
	AAX_ERROR_DSPsStoppedErr			= -9091,	// Timeout implementing a disk task while waiting for an elapsed time.
	AAX_ERROR_UnderrunErr1				= -9092,	// Under run error from the DSP.
	AAX_ERROR_UnderrunErr2				= -9093,	// Under run error from the DSP.
	AAX_ERROR_UnderrunErr3				= -9094,	// Under run error from the DSP.
	AAX_ERROR_BadVRefNumErr				= -9095,	// Audio file is not on the propper disk SCSI chain.
	AAX_ERROR_NoPeripheralSelected		= -9096,	// Deck can not be aquired without a peripheral being selected.
	AAX_ERROR_LaunchMemoryErr			= -9097,	// Unable to launch DAE because of a memory error.  DAE does NOT launch.
	AAX_ERROR_SetupLowMemoryErr			= -9098,	// Not enough memory reserved for DAE with the given hardware configurations.  DAE DOES launch.
	AAX_ERROR_GestaltBadSelector		= -9099,	// Gestalt selector not supported.
	AAX_ERROR_FileCopyInProgress		= -9100,	// Attempt made to start a new file copy while already in progress
	AAX_ERROR_FileCopyTooManyBytes		= -9101,	// Files aren't as big as the number of bytes requested to be copied
	AAX_ERROR_ExtensionTooOld			= -9102,	// DAE extension is too old for this version of DAE
	AAX_ERROR_ExtensionTooNew			= -9103,	// DAE extension is too new for this version of DAE
	AAX_ERROR_ExtensionCorrupt			= -9104,	// DAE extension does not conform to the expected format
	AAX_ERROR_SDSEntryNameTooLong		= -9105,	// Name registry entry name is too long
	AAX_ERROR_SDSPathNameTooLong		= -9106,	// Name registry path name is too long
	AAX_ERROR_SDSInvalidNameErr			= -9107,	// Name registry path+entry invalid
	AAX_ERROR_IteratorCantErr			= -9108,	// Requested operation not supported on provided iterator
	AAX_ERROR_IteratorInvalid			= -9109,	// Can't perform operation on iterator because iterator is not valid
	AAX_ERROR_InvalidNameId				= -9110,	// A name cannot be found in the name registry
	AAX_ERROR_SDSInvalidTask			= -9111,	// An invalid SDS task has been specified.
	AAX_ERROR_SDSBadStatus				= -9112,	// A token's fDeliveryStatus field has been corrupted.
	AAX_ERROR_OutOfTokensErr			= -9113,	// The token dispatcher has run out of tokens.
	AAX_ERROR_TclScriptErr				= -9114,	// An error occurred during execution of a Tcl script
	AAX_ERROR_DateBombExpired			= -9115,	// Compiled-in date bomb has expired
	AAX_ERROR_NeedsPCIErr				= -9116,	// This release only supports PCI machines.
	AAX_ERROR_AdatLinkCantGetTimeCode	= -9117,	// AdatLink cannot find time code edge.
	AAX_ERROR_DuplicateWriteFiles		= -9118,	// Writing to the same file multiple times during processing.
	AAX_ERROR_CantConvertBufferErr		= -9119,	// Unable to convert audio buffer between source and destination formats.
	AAX_ERROR_ReentrancyErr				= -9120,	// (Undefined)
	AAX_ERROR_CantGetTempBuffer			= -9121,	// Disk scheduler ran out of temporary buffers.  Playlist is too complex.
	AAX_ERROR_PendingRequestsFull		= -9122,	// (Undefined)
	AAX_ERROR_RequestHandlesFull		= -9123,	// (Undefined)
	AAX_ERROR_AnonymousDrive			= -9124,	// (Win32) Disk scheduler can't use a drive that doesn't have a drive signature.
	AAX_ERROR_InvalidDiskPath			= -9125,	// File mapper cannot find requested directory info.
	AAX_ERROR_UnrecognizedDiskVolume	= -9126,	// File mapper cannot recognize the requested volume.
	AAX_ERROR_ComputerNeedsRestart		= -9127,	// DAE state has changed such that the computer needs to restart
	AAX_ERROR_CPUOverload				= -9128,	// Host processing has exceeded its CPU allocation.
	AAX_ERROR_HostInterruptTooLong		= -9129,	// Host processing held off other system interrupts for too long.
	AAX_ERROR_UnityErr					= -9130,	// Avid Unity subsystem error.
	AAX_ERROR_BadDiskFormat				= -9131,	// File mapper found bad structures on disk
	AAX_ERROR_BounceHandlerTooSlow		= -9132,
	AAX_ERROR_BounceHandlerTooSlowToConvertWhileBouncing = -9133,
	AAX_ERROR_MBoxLostConnection		= -9134,		// MBox was disconnected during playback
	AAX_ERROR_MBoxNotConnected			= -9135,		// MBox is not connected
	AAX_ERROR_USBIsochronousUnderrun	= -9136,		// USB audio streaming underrun
	AAX_ERROR_AlreadyAcquired 			= -9137,		// tried to change coarse sample rate on already acquired deck
	AAX_ERROR_TDM2BusTopologyErr 		= -9138,		// eDsiTDM2BusTopologyErr was returned from DSI.
	AAX_ERROR_NoFileInDiskScheduler		= -9139,		// NULL file detected in disk request
	AAX_ERROR_BadDurationDiskScheduler	= -9140,		// bad duration for a disk request
	AAX_ERROR_DiskSchedulerNoneIssued	= -9141,		// no request issued to disk scheduler
	AAX_ERROR_DirectIODHSAlreadyOpen	= -9142,			// can't run if a DirectIO client is running DHS right now
	AAX_ERROR_AcquiredButChangedBuffers	= -9143,			// DAE was able to acquire the device but had to change the disk buffers size to do it.
	AAX_ERROR_StreamManagerUnderrun		= -9144,			// received error from StreamManager
	AAX_ERROR_DirectMidiError			= -9145,			// an error occurred in the DirectMidi subsytem
	AAX_ERROR_ResourceForkNotFound		= -9146,			// Could not find the DAE resource fork (i.e. fnfErr)
	AAX_ERROR_InputDelayNotSupported	= -9147,
	AAX_ERROR_InsufficientBounceStreams	= -9148,
	AAX_ERROR_MaxFileSizeReached		= -9149,			// We tried to grow a file beyond the 31 bit file size limitation (can happen w/destructive record)
	AAX_ERROR_NoTDMHardwareExists		= -9150,	// Returned when there's no TDM hardware in the system.
	AAX_ERROR_RequiredDriversNotPresent	= -9151,	// There are no TDM drivers present.
	AAX_ERROR_RequiredPlugInsNotPresent	= -9152,	// (Undefined)
	AAX_ERROR_AutoTrackTooDense3		= -9153,	// Too many ramp regions for automation track.
	AAX_ERROR_AutoTotalTooDenseForMixer	= -9154,	// (Undefined)
	AAX_ERROR_AutoTotalTooDenseForDSP	= -9155,	// (Undefined)
	AAX_ERROR_BadPlugInSpec				= -9156,	// Default error returned when there's no component object attatched to a spec.
	AAX_ERROR_FarmRequiredErr			= -9157,	// Error returned by objects that require a DSP farm in the system.
	AAX_ERROR_PlugInUsedKeyEvent		= -9158,	// Indicates a plugin DID respond to a key event.
	AAX_ERROR_IPRUnknown				= -9159,	// Indicates the IPR value equals -1 when it shouldn't.
	AAX_ERROR_IPRNotUnknown				= -9160,	// Indicates the IPR value does not equal -1 when it should.
	AAX_ERROR_DCMSendMessageErr			= -9161,	// Failed trying to send a message to a plug-in DSP; MasterDCM = Engine DSPs
	AAX_ERROR_DCMGetMessageErr			= -9162,	// Failed trying to get a message from a plug-in DSP; MasterDCM = Engine DSPs
	AAX_ERROR_PlugInDidSetCursor		= -9163,		// When returned by FicPlugInEvent, the plug-in DID change the cursor.
	AAX_ERROR_EngineThreadStarved		= -9164,		// A high-priority audio engine thread was starved, possibly because of plug-in priority inversion bugs
	AAX_ERROR_FreeformTextError			= -9165,			// Used internally to throw arbitrary errors (usually for testing)
	AAX_ERROR_DCMSendMessageErrSlave	= -9166,	// Failed trying to send a message to a plug-in DSP; SlaveDCM = Mixer DSPs
	AAX_ERROR_DCMGetMessageErrSlave		= -9167,	// Failed trying to get a message from a plug-in DSP; SlaveDCM = Mixer DSPs

	
};

// Weird errors preserved here for backwards compatibility (i.e., older DAE's returned these errors, so we should also):

enum {
	AAX_ERROR_BeyondPlayableRange		= -9735		// Session playback passed the signed 32 bit sample number limit ( = kFicParseErr - 700).
};


//----------------------------------------------------------------------------
// Error codes returned from the SADriver/DigiSystem INIT via DAE
//----------------------------------------------------------------------------

enum {
	AAX_ERROR_SADriverErrOffset			= -9200,	// Offset only, should never be returned as a result.
	AAX_ERROR_Unsupported				= -9201,	// Unsupported feature being set from a piece of hardware.
	AAX_ERROR_NoStandardShell			= -9202,	// Unable to load standard shell code resource.
	AAX_ERROR_TooManyPeripherals		= -9203,	// Went beyond the max number of peripherals allowed in the code.
	AAX_ERROR_SADriverHostTimeoutErr	= -9204,	// Timeout occured while trying to communicate with the DSP's host port.
	AAX_ERROR_InvalidValue				= -9205,	// Invalid value being set to a hardware feature.
	AAX_ERROR_InvalidObject				= -9206,	// NULL object found when a valid object is required.

	AAX_ERROR_NILClient					= -9210,	// Trying to opperate on a NULL client.
	AAX_ERROR_ClientRegistered			= -9211,	// Client already registered.
	AAX_ERROR_ClientUnregistered		= -9212,	// Trying to remove a client when it's not registered.
	AAX_ERROR_NoListener				= -9213,	// No client to respond to a message from another client.

	AAX_ERROR_CardOwned					= -9220,	// A card is owned by a client.
	AAX_ERROR_DSPOwned					= -9230,	// A DSP is owned by a client.

	AAX_ERROR_NILShell					= -9240,	// Trying to opperate on a NULL shell.
	AAX_ERROR_ShellRegistered			= -9241,	// Shell already registered.
	AAX_ERROR_ShellUnregistered			= -9242,	// Trying to remove a shell when it's not registered.
	AAX_ERROR_ShellTooSmall				= -9243,	// (Undefined)
	AAX_ERROR_ShellTooLarge				= -9244,	// DSP code runs into standard shell or runs out of P memory.
	AAX_ERROR_StandardShell				= -9245,	// Trying to unregister the standard shell.

	AAX_ERROR_NoDriverFile				= -9250,	// Unable to open or create the DigiSetup file.
	AAX_ERROR_DriverFileUnused			= -9251,	// Trying to free the DigiSetup file when it hasn't been openned.
	AAX_ERROR_NILResource				= -9252,	// Resource not found in the DigiSetup file.
	AAX_ERROR_BadSize					= -9253,	// Resource size does not match pointer size requested.
	AAX_ERROR_BadSlot					= -9254,	// NuBus slot value is out of range for the system.
	AAX_ERROR_BadIndex					= -9255		// DSP index is out of range for the system.
};


//----------------------------------------------------------------------------
// Error codes for Elastic audio 
//----------------------------------------------------------------------------
enum {
	AAX_ERROR_ElasticGeneralErr			= -9400,	// don't know what else to do
	AAX_ERROR_ElasticUnsupported		= -9401,	// requested op unsupported
	AAX_ERROR_ElasticUnderrun			= -9402,	// underrun; plugin didn't output what it was supposed to
	AAX_ERROR_ElasticCPUOverload		= -9403,	// Like kFicCPUOverload but for Fela
	AAX_ERROR_ElasticOutOfMemory		= -9404,	// you're not going to last long...
	AAX_ERROR_ElasticTrackTooDense		= -9405,	// like kFicAudioTrackTooDense1; feeder list too big
	AAX_ERROR_ElasticInadequateBuffering	= -9406,	// reserved buffers for Fela data too small
	AAX_ERROR_ElasticChannelErr			= -9407,	// Fela requires exactly 1 play track channel
	AAX_ERROR_ElasticConnectionErr		= -9408,	// Problem with a plugin connection 
	AAX_ERROR_ElasticBufferNotFound		= -9409,	// Can't find a necessary buffer for Fela processing
	AAX_ERROR_ElasticNeverStoppingErr	= -9410,	// infinite loop in an Elastic plugin?
	AAX_ERROR_ElasticDriftBackwardsErr	= -9411,	// disconnect between DAE (app?) and plugin data consumption rates
	AAX_ERROR_ElasticDriftForwardsErr	= -9412,	// disconnect between DAE (app?) and plugin data consumption rates
	AAX_ERROR_ElasticPlugInLimitsErr	= -9413,	// problem with plugin drift/lookAhead; too much requested?
	AAX_ERROR_ElasticChannelCountMismatch	= -9414,	// the number of output channels for the elastic plugin do not match the number of input channels.
	AAX_ERROR_ElasticInvalidParameter	= -9415,    // Elastic function was passed a bad parameter
	AAX_ERROR_ElasticInvalidState		= -9416,	// Elastic track's internal state is in error.
	AAX_ERROR_ElasticPlugInConnected	= -9417,	// Can't change stem format once an elastic plugin is already connected to a track
	AAX_ERROR_ElasticScrubCacheErr		= -9418,	// problem reading from / writing to the scrub cache
	AAX_ERROR_ElasticEphemeralAllocErr	= -9419,	// ephemeral buffer alloc failure
	AAX_ERROR_ElasticDiskTooSlowErr		= -9473,	// Like -9073, but caught in a new way (Elastic needs disk data sooner)

	AAX_ERROR_ElasticMaxErr
};


//----------------------------------------------------------------------------
// Error codes for Disk Cache
//----------------------------------------------------------------------------
enum {
	AAX_ERROR_DiskCachePageOverflow		= -9500,	// not enough pages in the cache to fulfill page request.
	AAX_ERROR_DiskCachePageInFileErr	= -9501,	// problem allocating new pages for a file.
	AAX_ERROR_DiskCacheWriteErr			= -9502,	// problem writing to the disk cache.
	AAX_ERROR_DiskCacheDiskWriteErr		= -9503,    // problem writing to disk from the cache.
	AAX_ERROR_DiskCacheInvalidNull		= -9504,    // invalid NULL variable.  NULL and 0 have special meaning in the cache.
	AAX_ERROR_DiskCacheEOFErr			= -9505,    // reading or writing beyond the end of a file.
	AAX_ERROR_DiskCacheMissingDataErr	= -9506,	// data that's supposed to be in the cache is not.
	AAX_ERROR_DiskCacheGeneralErr		= -9507,	// general error.
	AAX_ERROR_DiskCacheDoubleLRUPageErr	= -9508,	// duplicate page in the LRU.
	AAX_ERROR_DiskCacheDoubleOwnerPageErr = -9509,	// two pages with the same owner.
	AAX_ERROR_DiskCachePageLeakErr		= -9510,	// page leak in the allocator. 
	AAX_ERROR_DiskCacheMappingErr		= -9511,	// corruption in mapping of disk cache objects to the page allocator
	AAX_ERROR_DiskCacheMissingPagesErr	= -9512,	// pages supposed to be in the cache aren't there
	AAX_ERROR_DiskCacheUnityFileErr		= -9513,	// Unity and ISIS are incompatible with the disk cache's temporary buffers
	AAX_ERROR_DiskCacheOutOfMemory		= -9514,	// Couldn't allocate the disk cache!  32bits will suffocate us all.
};


//----------------------------------------------------------------------------
// Error codes for Copland Card
//----------------------------------------------------------------------------
enum {
	AAX_ERROR_CoplandPIOOverflow		= -9600,	// PIO ring buffer overflowed
	AAX_ERROR_CoplandPIOUnderflow		= -9601,	// PIO ring buffer underflow
	AAX_ERROR_CoplandPIOSyncErr			= -9602,	// PIO sync error
	AAX_ERROR_CoplandPIOClockChange		= -9603,	// PIO clock change error
	AAX_ERROR_CoplandPIOUnknownErr		= -9604,	// PIO unknown error
	AAX_ERROR_CoplandTDMRcvOverflow		= -9605,	// TDM receive overflow
	AAX_ERROR_CoplandTDMXmtUnderflow	= -9606,	// TDM transmit underflow
	AAX_ERROR_CoplandTDMSyncErr			= -9607,	// TDM sync error
	AAX_ERROR_CoplandTDMUnknownErr		= -9608,	// TDM unknown error
};


//----------------------------------------------------------------------------
// Error codes returned from the Session 8 or Disk I/O SCSI driver
//----------------------------------------------------------------------------

enum {
	AAX_ERROR_ArbitraryErr				= -8000,	// Misc. errors from the driver.
	AAX_ERROR_NoFreeCards				= -8001,	// Unable to find free elvis board.
	AAX_ERROR_NoSuchCard				= -8002,	// Trying to execute code for a card that does NOT exist.
	AAX_ERROR_LowMemory					= -8003,	// Unable to allocate memory from the system heap.
	AAX_ERROR_NULLTaskErr				= -8004,	// Ran accross a disk task which was NULL.
	AAX_ERROR_FailedReadSectors 		= -8005,	// Unable to read data from a disk.
	AAX_ERROR_FailedWriteSectors 		= -8006,	// Unable to write data to a disk.
	AAX_ERROR_BadFormatErr				= -8007,	// Disk is not properly formated.
	AAX_ERROR_NoHFSErr					= -8008,	// No HFS system on the disk.
	AAX_ERROR_NoDrivesMounted			= -8009,	// No drives are mounted within the driver.

	AAX_ERROR_SCSITimeout				= -8010,	// Timeout occured while waiting for SCSI operation.
	AAX_ERROR_InvalidBlockSize			= -8011,	// Unsupported block transfer size.
	AAX_ERROR_SCSINotAvailable			= -8012,	// Unable to access SCSI because it is in use.
	AAX_ERROR_InvalidDrive				= -8013,	// Drive not mounted within the driver.
	AAX_ERROR_DRAMNotAvailable			= -8014		// Unable to access DRAM because it is on use.
};


//----------------------------------------------------------------------------
// Various Widget Error Codes
//----------------------------------------------------------------------------

enum {

	// External Callback Proc Errors -7000..-7024
	AAX_ERROR_SelectorNotSupported 		= -7000,	// This selector ID is unknown currently.
	AAX_ERROR_WidgetNotFound 			= -7001,	// Refnum did not specify a known widget.
	AAX_ERROR_SoundFileDocNotFound		= -7002,	// Requested sound file was not found.
	AAX_ERROR_NoMemToOpen 				= -7003,	// There was not enough mem to open a component.
	AAX_ERROR_CantFindCFMShimComponent	= -7004,	// Mach-O DAE couldn't find a component needed to help load CFM plug-ins

	// Plug-In Manager Errors -7025..-7049
	AAX_ERROR_SingleInstanceMultiOpen 	= -7025,	// One component instance may be opened only once.
	AAX_ERROR_PlugInNotInstantiated 	= -7026,	// A non-instantiated plug-in was asked to do something.
	AAX_ERROR_NilComponentObject 		= -7027,	// A component-referencing object was NIL.
	AAX_ERROR_WidgetNotOpen 			= -7028,	// A non-instantiated widget was asked to do something.
//TIMILEONE ADD
	AAX_ERROR_TIShellError				= -7029,	// Error returned from TIShell Loading
	AAX_ERROR_DspMgrError				= -7030,	// An error originating in DspMgr returned
	AAX_ERROR_EffectLoadError			= -7031,	// A problem loading the effect through the AAX Effect Factory or Collection.

	// Plug-In Manager Errors -7050..-7075
	AAX_ERROR_NotEnoughHardware 		= -7050,	// Not enough hardware available to instantiate a plug-in.
	AAX_ERROR_NoHardwarePresent 		= -7051,	// No hardware at all available to instantiate a plug-in.
	AAX_ERROR_NotEnoughTDMSlots 		= -7052,	// Not enough TDM slots available to instantiate a plug-in.
	AAX_ERROR_NotEnoughMemory 			= -7053,	// Not enough host memory available to instantiate a plug-in.
	AAX_ERROR_CantInstantiatePlugIn 	= -7054,	// Unable to instantiate a plug-in (generic error).
	AAX_ERROR_CantFindPlugIn 			= -7055,	// Unable to find the specified plug-in.
	AAX_ERROR_NoPlugInsExist 			= -7056,	// No plug-ins at all exist.
	AAX_ERROR_NoHardwareForIOMode 		= -7057,	// No more hardware available in the specified I/O mode.
	AAX_ERROR_PlugInUnauthorized		= -7058,	// To catch uncopyprotected plugins
	AAX_ERROR_UnprotectedDSPCodeDetected	= -7059,	// Remove DSP code built on Windows from your HCP protected plugin. Just use Altura on the Mac HCP-protected plugin resources.
	AAX_ERROR_PlugInServicesTooOld		= -7060,	// The plug-in wants a version of the PIS_API that is newer than the one Fic provided.
	AAX_ERROR_DSPResourceNotFound		= -7061,	// DSP code resource could not be found.  Make sure a resource is defined for each supported core type (check your .r (Mac) or .rcx (Win) SACD def file)
	AAX_ERROR_InvalidHostSignalNet		= -7062,	// The signalNet ptr does not correspond to a CHostSignalNet instance

	// Widget errors (returned by calls to widget functions): -7075..-7099.
	AAX_ERROR_WidgetNotEnoughMemory 		= -7075,	// Widget did not have enough memory for request (general err).
	AAX_ERROR_WidgetCannotControlInstance 	= -7076,	// Widget has hit internal limit of instances it can control.
	AAX_ERROR_WidgetNotEnoughHardware 		= -7077,	// Widget could not find more hardware for a new instance.
	AAX_ERROR_WidgetNoHardwarePresent 		= -7078,	// Widget could not find any hardware for any instances.
	AAX_ERROR_WidgetCannotMove				= -7079,	// Widget can not move to another DSP.
	AAX_ERROR_WidgetDSPManagerNotPresent	= -7080,	// DSPManager is not present and needs to be for widget to operate
	AAX_ERROR_WidgetUnsupportedSampleRate	= -7081,	// Widget cannot instantiate at the current sample rate
	AAX_ERROR_WidgetSetChunkFailed			= -7082,	// PIug-in settings update failed.  CProcess::SetChunk(...)
	AAX_ERROR_WidgetGetChunkFailed			= -7083,	// Failure extracting settings state from plugin.  CProcess::GetChunk(...)
	AAX_ERROR_WidgetCompareActiveChunkFailed	= -7084,	// Failure do compare of plug-in's settings.  CProcess::CompareActiveChunk(...)
	AAX_ERROR_WidgetIncompatible			= -7085,	// Widget is not compatible with this version
	AAX_ERROR_WidgetHTDMUnsupported			= -7086,	// Widget attempted to use StreamManager, which has been EOL'ed.

	// Connection errors: -7100..-7124
	AAX_ERROR_InputPortInUse 				= -7100,	// Tried to connect to an input that is already connected.
	AAX_ERROR_OutputPortCannotConnect 		= -7101,	// Specified output port has reached its limit of output connections.
	AAX_ERROR_WidgetCantConnectInternally 	= -7102,	// Obsolete.
	AAX_ERROR_InvalidConnection 			= -7103,	// Invalid or freed connection reference passed.
	AAX_ERROR_BadConnectionInfo 			= -7104,	// TDM talker & listener data not consistent on disconnect.
	AAX_ERROR_FreeConnectionErr 			= -7105,	// Could not delete connection info.
	AAX_ERROR_InvalidPortNum 				= -7106,	// Out-of-range or nonexistent port number specified.
	AAX_ERROR_PortIsDisconnected 			= -7107,	// Tried to disconnect a disconnected port.
	AAX_ERROR_NotPreparedForConnections 	= -7108,	// PrepareForConnections has not been called with the true value.
	AAX_ERROR_PreparedForConnections 		= -7109,	// PrepareForConnections was last called with the true value.

	AAX_ERROR_BadStemFormat					= -7110,
	AAX_ERROR_BadInputStemFormat			= -7111,
	AAX_ERROR_BadOutputStemFormat			= -7112,
	AAX_ERROR_BadSideChainStemFormat		= -7113,
	AAX_ERROR_BadGenericStemFormat			= -7114,
	AAX_ERROR_StemFormatsNotActuallySupported = -7115,

	AAX_ERROR_TooManyUnprepareForConns       = -7116,	// prepare and unprepare for connections calls are unbalanced
	AAX_ERROR_NoFirstRTASDuringPlayback      = -7117,	// can't instantiate the first RTAS plug-in on the fly (TDM decks)
	AAX_ERROR_NoBridgeConnectionDuringPlayback = -7118, // can't create or free a bridge connection during playback

	// Subwidget errs: -7125..-7149
	AAX_ERROR_SubWidgetIndexRangeErr 		= -7125,	// Specified subwidget index doesn't correspond with a subwidget.
	AAX_ERROR_InstanceIndexRangeErr 		= -7126,	// Specified instance index doesn't correspond with an instance.
	AAX_ERROR_SubWidgetCantBypass 			= -7127,	// Subwidget can't do a bypass on any instances.
	AAX_ERROR_NULLSubWidgetList 			= -7128,	// Needed to look through list, but it's NULL!
	AAX_ERROR_EmptySubWidgetList 			= -7129,	// List isn't NULL, but has no elements.

	// Instance errs: -7150..-7174
	AAX_ERROR_NumInstancesWentNegative 		= -7150,	// Somehow a count of instances (in widget or DSP) went < 0.
	AAX_ERROR_InstanceCantBypass 			= -7151,	// One particular instance can't do a bypass.
	AAX_ERROR_CantChangeNumInputs 			= -7152,	// Plugin does not have variable number of inputs.
	AAX_ERROR_CantChangeNumOutputs 			= -7153,	// Plugin does not have variable number of outputs.
	AAX_ERROR_SetNumInputsOutOfRange 		= -7154,	// Number of inputs being set is out of range.
	AAX_ERROR_SetNumOutputsOutOfRange 		= -7155,	// Number of outputs being set is out of range.
	AAX_ERROR_InstanceCantMute				= -7156,	// Plugin can't mute itself.
	AAX_ERROR_ChunkRangeErr					= -7157,	// Handle of plugin settings will not work on a plugin. 
	AAX_ERROR_MeterIndexOutOfRange 			= -7158,	// Index for a meter value is out of ranges.
	
	// callback errs: -7175..-7199
	AAX_ERROR_NullXCMDProc 					= -7175,	// Plugin does not have an XCMD proc pointer to DAE.
	AAX_ERROR_NotEnoughMemXCMD			 	= -7176,	// Ran out of memory trying to do an XCMD
	
	// driver call errs: -7200..-7249
	AAX_ERROR_BadDriverRefNum 				= -7200,	// Plugin does not have a valid driver object.
	AAX_ERROR_BadHardwareRefNum 			= -7201,	// Plugin does not have a valid pointer to a hardware object.  DSPPtr = NULL.
	AAX_ERROR_BadWidgetRef 					= -7202,	// Widget object is NULL.
	AAX_ERROR_EmptyDriverTypeList 			= -7203,	// No drivers registered into DAE.
	AAX_ERROR_NoWidgetHardwareFound 		= -7204,	// Unable to find hardware for the widget.
	AAX_ERROR_BadCardChannel 				= -7205,	// Unable to allocate audio channel to access tdm.
	AAX_ERROR_SATReadErr 					= -7206,	// Driver call to read SAT returned an err.
	AAX_ERROR_SATReadFailed 				= -7207,	// Driver call succeeded, but val returned is bad.
	AAX_ERROR_TimeSlotTalking 				= -7208,	// Tried to change a talker to another card channel w/o disconnect.
	AAX_ERROR_TimeSlotListening 			= -7209,	// Tried to change a listener to another card channel w/o disconnect.
	AAX_ERROR_SATWriteErr 					= -7210,	// Driver call to write SAT returned an err.
	AAX_ERROR_NoMoreCardChansAvail 			= -7211,	// Hardware ran out of card channels - BIG PROBLEM!!!
	AAX_ERROR_BadTimeSlotNum 				= -7212,	// Given time slot to disconnect ain't owned by caller.
	AAX_ERROR_ListenersWentNegative 		= -7213,	// Somehow num of listeners to a card chan went < 0 BIG PROBLEM!
	AAX_ERROR_GetDSPListErr 				= -7214,	// Err in driver call.
	AAX_ERROR_NoMemForTypeList 				= -7215,	// Out-of-mem trying to generate driver type list.
	AAX_ERROR_riverOpenFailed				= -7216,	// (optional) Attempt to open SADriver returned an err.
	AAX_ERROR_WaitStateErr 					= -7217,	// Err setting/getting DSP waits.
	AAX_ERROR_DSPErr 						= -7218,	// General DSP err.
	AAX_ERROR_NoDriverAvail 				= -7219,	// We don't have any of the drivers a widget can use.
	AAX_ERROR_riverSignInFailed 			= -7220,	// SADriver init worked, but our sign-in failed.
	AAX_ERROR_DSPDMAChannelTimeout 			= -7221,	// Onyx DMA channels not stopped in time for changing TDM ASIC
	AAX_ERROR_SATReadVerifyFailed 			= -7222,	// Read verify after writing SAT is incorrect
	AAX_ERROR_CantRouteMultipleLocalIO		= -7223,	// Special case of kBadTimeSlotNum for better app error reporting.  See PT 5.1.1 #11872 & #14178.
	AAX_ERROR_LoggedExceptionInConnMgr      = -7224,    // Logged  exception caught in Connection Manager
	AAX_ERROR_UnknownExceptionInConnMgr     = -7225,    // Unknown exception caught in Connection Manager
	
	// Config problems: -7250..-7299
	AAX_ERROR_BadConfigHandle 				= -7250,	// Handle given to restore config has bad size or internal inconsistency.
																			
	// Widget control errors: -7300..-7324
	AAX_ERROR_ControlIndexRangeErr 			= -7300,	// Passed control index was out of range (couldn't find control).
	AAX_ERROR_NotOurControl 				= -7301,	// Passed in control that didn't belong to widget.
	AAX_ERROR_NullControl 					= -7302,	// Passed in control ref was NULL.
	AAX_ERROR_DSPLocked						= -7303,	// Can't perform control update, DSP is locked from use.
	AAX_ERROR_ControlValueInfoNotSupported	= -7304,	// For use with FicGetControlValueInfo()

	// Automation errors: -7325..-7349
	AAX_ERROR_NoTimeOfDaySlotsYet 			= -7325,	// Tried to connect to time-of-day time slots, but there are none.

	// Builtin plugin errors: -7350..-7374
	AAX_ERROR_UnsupportedBuiltinPlugin 		= -7350,	// Invalid built-in plugin spec.

	// I/O mode-related errors: -7375..-7399
	AAX_ERROR_BadIOModeNum 					= -7375,	// Sent an illegal number as the I/O mode for a widget.
	AAX_ERROR_WidgetCannotUseMode 			= -7376,	// Legal mode specified, but widget can't use the mode.

	// Sate-based automation errors: -7425..-7449
	AAX_ERROR_NotStateAutomatable			= -7425,	// plug-in does not support state-based automation
	AAX_ERROR_AutoIndexRangeErr 			= -7426,	// internal state or trigger index out-of-range
	AAX_ERROR_InvalidAutoItemErr 			= -7427,	// tried to remove a state or trigger at unused index
	AAX_ERROR_NULLItemPtrErr 				= -7428,	// send NULL state or trigger pointer on a Get call
	AAX_ERROR_NoStateListErr 				= -7429,	// no mem available for list of states
	AAX_ERROR_NoAutoManagerList	 			= -7430,	// no mem to create automation managers
	AAX_ERROR_StateIndexRangeErr 			= -7431,	// sent out-of-range state index
	AAX_ERROR_TriggerIndexRangeErr 			= -7432,	// sent out-of-range trigger index
	AAX_ERROR_TooManyTriggersErr			= -7433,	// ran out of room for triggers on hardware.

	// ASP Processing errors: - 7450..-7499
	AAX_ERROR_ProcessStuckInLoop			= -7450,	// Plugin is stuck in a loop for an process pass.
	AAX_ERROR_AnalyzeStuckInLoop			= -7451,	// Plugin is stuck in a loop for an analysis pass.
	AAX_ERROR_OutputBoundsNotInited			= -7452,	// Plugin needs to set output connections to valid range within InitOutputBounds.
	AAX_ERROR_ConnectionBufferOverwrite		= -7453,	// Plugin overwrote the end of the connection buffer.
	AAX_ERROR_NoASPBounds					= -7454,	// Start and end bounds for an ASP process or analysis were equal.
	AAX_ERROR_InvalidASPBounds				= -7455,	// End bounds are less than the start bounds for an ASP process.
	AAX_ERROR_ASPDoneProcessing				= -7456,	// The ASP terminated processing with no errors.
	AAX_ERROR_ASPErrorWritingToDisk			= -7457,	// ASP encountered error while writing audio data to disk.
	AAX_ERROR_ASPOutputFileTooLarge			= -7458,	// ASP tried to write a file larger than the 2^31 bytes in size.
	AAX_ERROR_ASPOverwriteOnUnity			= -7459,	// ASP tried to write destructively to Unity

	// Errors called from Failure Handler routines.
	AAX_ERROR_WidgetAssertErr				= -7400,
	AAX_ERROR_WidgetNILObjectErr			= AAX_ERROR_NotEnoughMemory,
	AAX_ERROR_WidgetLowMemoryErr			= AAX_ERROR_NotEnoughMemory,
	AAX_ERROR_riverErrOffset				= AAX_ERROR_SADriverErrOffset,
	AAX_ERROR_UnknownErr					= -7401					// Plugin caught an unknown exception
};

//----------------------------------------------------------------------------
//  Digi Serial Port Errors
//----------------------------------------------------------------------------

enum {
	AAX_ERROR_SerBadParameterPointer		= -7500,
	AAX_ERROR_SerBadRoutineSelector			= -7501,
	AAX_ERROR_SerPortDoesNotExist			= -7502,
	AAX_ERROR_SerPortAlreadyInUse			= -7503,
	AAX_ERROR_SerPortNotOpen				= -7504,
	AAX_ERROR_SerBadPortRefereceNumber		= -7505
};

//----------------------------------------------------------------------------
//  DCM Errors (formerly in DCM.h)
//----------------------------------------------------------------------------

enum
{
	AAX_ERROR_DCMAcquirePortErr		=	-11002,
	AAX_ERROR_DCMNilObjectErr		= 	-11003,
	AAX_ERROR_DCMAssertErr			=	-11004,			// A DSP might be dead.
	AAX_ERROR_DCMLowMemErr			=	-11005
};

// Play nice with emacs
// Local variables:
// mode:c++
// End:

*/

/// @cond ignore
#endif // AAX_ERRORS_H
/// @endcond
