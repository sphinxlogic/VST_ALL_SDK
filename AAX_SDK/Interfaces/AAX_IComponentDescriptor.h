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
 *	\file  AAX_IComponentDescriptor.h
 *
 *	\brief Description interface for an %AAX plug-in algorithm
 *
 */ 
/*================================================================================================*/


#ifndef _AAX_ICOMPONENTDESCRIPTOR_H_
#define _AAX_ICOMPONENTDESCRIPTOR_H_

#include "AAX.h"
#include "AAX_IDma.h"
#include "AAX_Callbacks.h"

class AAX_IPropertyMap;


/** \brief	Description interface for an %AAX plug-in component
		
	\details	
	\hostimp
	
	This is an abstract interface containing everything needed to describe a single 
	algorithm of an Effect.  For more information about algorithm processing in
	%AAX plug-ins, see \ref CommonInterface_Algorithm.
		
	\ingroup CommonInterface_Describe
*/
class AAX_IComponentDescriptor
{
public:
	virtual ~AAX_IComponentDescriptor() {}
		
	/*!
	 *  \brief Clears the descriptor
	 *  
	 *  Clears the descriptor and readies it for the next algorithm description
	 *
	 */	
	virtual AAX_Result					Clear () = 0;
	
	/*!
	 *  \brief Subscribes an audio input context field
	 *  
	 *  Defines an audio in port for host-provided information in the algorithm's
	 *	context structure.
     *
     *  - Data type: float**
     *  - Data kind: An array of float arrays, one for each input channel
	 *
	 *  \param[in] iFieldIndex
	 *		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	 */	
    virtual AAX_Result                  AddAudioIn ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	*  \brief Subscribes an audio output context field
	*  
	*  Defines an audio out port for host-provided information in the algorithm's
     *	context structure.
     *
     *  - Data type: float**
     *  - Data kind: An array of float arrays, one for each output channel
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*/	
    virtual AAX_Result                  AddAudioOut ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	*  \brief Subscribes a buffer length context field
	*  
	*  Defines a buffer length port for host-provided information in the algorithm's
	*  context structure.
	*
	*  - Data type: int32_t*
	*  - Data kind: The number of samples in the current audio buffer
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*/	
    virtual AAX_Result                  AddAudioBufferLength ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	*  \brief Subscribes a sample rate context field
	*  
	*  Defines a sample rate port for host-provided information in the algorithm's
	*  context structure.
	*
	*  - Data type: \ref AAX_CSampleRate *
	*  - Data kind: The current sample rate
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*/	
    virtual AAX_Result                  AddSampleRate ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	*  \brief Subscribes a clock context field
	*  
	*  Defines a clock port for host-provided information in the algorithm's
	*  context structure.
	*
	*  - Data type: \ref AAX_CTimestamp *
	*  - Data kind: A running counter which increments even when the transport is not
	*               playing. The counter increments exactly once per sample quantum.
	*
	*  \compatibility As of Pro Tools 11.1, this field may be used in both Native
	*  and DSP plug-ins. The DSP clock data is a 16-bit cycling counter. This field
	*  was only available for Native plug-ins in previous Pro Tools versions.
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*/	
    virtual AAX_Result                  AddClock ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	*  \brief Subscribes a side-chain input context field
	*  
	*  Defines a side-chain input port for host-provided information in the algorithm's
	*  context structure.
	*
	*  - Data type: int32_t*
	*  - Data kind: The index of the plug-in's first side-chain input channel
    *               within the array of input audio buffers
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*/	
    virtual AAX_Result                  AddSideChainIn ( AAX_CFieldIndex iFieldIndex ) = 0;

	/*!
	 *  \brief Adds a custom data port to the algorithm context
	 *  
	 *  Defines a read-only data port for plug-in information in the algorithm's
	 *	context structure. The plug-in can send information to this port
	 *	using \ref AAX_IController::PostPacket().
	 *
	 *	The host guarantees that all packets will be delivered to this
	 *	port in the order in which they were posted, up to the point of
	 *	a packet buffer overflow.
	 *
	 *  \param[in] iFieldIndex
	 *		Unique identifier for the port, generated using \ref AAX_FIELD_INDEX
	 *	\param[in] iPacketSize
	 *		Size of the data packets that will be sent to this port
	 *	\param[in] iPortType
	 *		Whether or not the host should buffer this port
	 */	
	virtual AAX_Result					AddDataInPort ( AAX_CFieldIndex iFieldIndex, uint32_t iPacketSize, AAX_EDataInPortType iPortType = AAX_eDataInPortType_Buffered ) = 0;

	/** @brief	Adds an auxiliary output stem for a plug-in.
	 *
     *  Use this method to add  additional output channels to the algorithm context.
	 *
	 *	The aux output stem audio buffers will be added to the end of the audio outputs array
	 *	in the order in which they are described. When writing audio data to a specific aux
	 *	output, find the proper starting channel by accumulating all of the channels of the
	 *	main output stem format and any previously-described aux output stems.
	 *
	 *	The plug-in is responsible for providing a meaningful name for each aux outputs. At
	 *	the very least, individual outputs should be labeled "Output xx", where "xx" is the
	 *	aux output number as it is defined in the plug-in. The output name should also include
	 *	the words "mono" and "stereo" to support when users are looking for an output with a
	 *	specific stem format.
	 *
	 *	\compatibility There is a hard limit to the number of outputs that Pro Tools supports for a
	 *	single plug-in instance.  This limit is currently set at 256 channels, which includes all of
	 *	the plug-in's output channels in addition to the sum total of all of its aux output stem
	 *	channels.
	 *
	 *	\compatibility Pro Tools supports only mono and stereo auxiliary output stem formats
	 *
	 *	\warning This method will return an error code on hosts which do not support auxiliary
	 *	output stems. This indicates that the host will not provide audio buffers for auxiliary
	 *	output stems during processing. A plug-in must not attempt to write data into auxiliary
	 *	output stem buffers which have not been provided by the host!
     *
	 *  \param[in] iFieldIndex
	 *		DEPRECATED: This parameter is no longer needed by the host, but is included in the interface for binary compatibility
	 *	\param[in] iStemFormat
     *      The stem format of the new aux output
	 *	\param[in] iNameUTF8
     *      The name of the aux output. This name is static and cannot be changed after the descriptor is submitted to the host
	 *		
	 */
	virtual AAX_Result					AddAuxOutputStem ( AAX_CFieldIndex iFieldIndex, int32_t iStemFormat, const char iNameUTF8[]) = 0;
	/*!
	 *  \brief Adds a private data port to the algorithm context
	 *  
	 *  Defines a read/write data port for private state data.
	 *	Data written to this port will be maintained by the host
	 *	between calls to the algorithm context.
     *
	 *	\sa alg_pd_registration
	 *
	 *  \param[in] iFieldIndex
	 *		Unique identifier for the port, generated using \ref AAX_FIELD_INDEX
	 *	\param[in] iDataSize
	 *		Size of the data packets that will be sent to this port
	 *	\param[in]	iOptions
	 *		Options that define the private data port's behavior
	 */
	virtual AAX_Result					AddPrivateData ( AAX_CFieldIndex iFieldIndex, int32_t iDataSize, /* AAX_EPrivateDataOptions */ uint32_t iOptions = AAX_ePrivateDataOptions_DefaultOptions ) = 0;
    
    /*! 
    *   \brief Adds a block of data to a context that is not saved between callbacks and is scaled by the system buffer size.
    *
    *   This can be very useful if you use block processing and need to store intermediate results.  Just specify your base element
    *   size and the system will scale the overall block size by the buffer size.  For example, to create a buffer of floats that is
    *   the length of the block, specify 4 bytes as the elementsize.
    *
    *   This data block does not retain state across callback and can also be reused across instances on memory contrained systems.
    *
    *   \param[in] iFieldIndex
    *       Unique identifier for the port, generated using \ref AAX_FIELD_INDEX
    *   \param[in] iDataElementSize
    *       The size of a single piece of data in the block.  This number will be multipied by the processing block size to determine total block size.
    */
    virtual AAX_Result  AddTemporaryData( AAX_CFieldIndex iFieldIndex, uint32_t iDataElementSize) = 0;

	/**
		\brief Adds a DMA field to the plug-in's context
	 
		DMA (direct memory access) provides efficient reads from and writes to external memory on the
		DSP.  DMA behavior is emulated in host-based plug-ins for cross-platform portability.
		
		\note The order in which DMA instances are added defines their priority and therefore order of
		execution of DMA operations. In most plug-ins, Scatter fields should be placed first in order
		to achieve the lowest possible access latency.
		
		For more information, see \ref additionalFeatures_DMA .
		
		\todo Update the DMA system management such that operation priority can be set arbitrarily
	 
		\param[in]	iFieldIndex
			Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
		\param[in]	iDmaMode
			AAX_IDma::EMode that will apply to this field
				
	*/
	virtual AAX_Result					AddDmaInstance ( AAX_CFieldIndex iFieldIndex, AAX_IDma::EMode iDmaMode ) = 0;
	
	/**	@brief Adds a meter field to the plug-in's context
	
		Meter fields include an array of meter tap values, with one tap per meter per context.  Only
		one meter field should be added per Component.  Individual meter behaviors can be described
		at the Effect level.
	 
		For more information, see \ref AdditionalFeatures_Meters .
		
		\param[in]	iFieldIndex
			Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
		\param[in]	iMeterIDs
			Array of 32-bit IDs, one for each meter.  Meter IDs must be unique within the Effect.
		\param[in]	iMeterCount
			The number of meters included in this field
	 */
	virtual AAX_Result					AddMeters ( AAX_CFieldIndex iFieldIndex, const AAX_CTypeID* iMeterIDs, const uint32_t iMeterCount ) = 0;

	/** @brief	Adds a MIDI node field to the plug-in's context
	 
	 - Data type: \ref AAX_IMIDINode *
	 
	 The resulting MIDI node data will be available both in the algorithm context and in the plug-in's
	 \ref AAX_IEffectParameters "data model" via
	 \ref AAX_IACFEffectParameters_V2::UpdateMIDINodes() "UpdateMIDINodes()".
	 
	 To add a MIDI node that is only accessible to the plug-in's data model, use
	 \ref AAX_IEffectDescriptor::AddControlMIDINode()
	 
	 \compatibility Due to current restrictions MIDI data won't be delivered to DSP algorithms, only to %AAX Native.

		\param[in]	iFieldIndex
			The ID of the port.  MIDI node ports should formatted as a pointer to an \ref AAX_IMIDINode.
		\param[in]	iNodeType
			The type of MIDI node, as \ref AAX_EMIDINodeType
		\param[in]	iNodeName
			The name of the MIDI node as it should appear in the host's UI
		\param[in]	channelMask
			The channel mask for the MIDI node. This parameter specifies used MIDI channels. For Global MIDI nodes, use a mask of \ref AAX_EMidiGlobalNodeSelectors
	*/
	virtual AAX_Result					AddMIDINode ( AAX_CFieldIndex iFieldIndex, AAX_EMIDINodeType iNodeType, const char iNodeName[], uint32_t channelMask ) = 0;

	/*!
	*  \brief Subscribes a context field to host-provided services or information
	*  
	*  \note Currently for internal use only. 
	*
	*  \param[in] iFieldIndex
	*		Unique identifier for the field, generated using \ref AAX_FIELD_INDEX
	*  \param[in] iFieldType
	*		Type of field that is being added
	*/	
	virtual AAX_Result					AddReservedField ( AAX_CFieldIndex iFieldIndex, uint32_t iFieldType ) = 0;

	/** @brief	Creates a new, empty property map.
		
		The component descriptor owns the reference to the resulting property map, and
		the underlying property map is destroyed when the component descriptor is
		released.
				
	*/
	virtual AAX_IPropertyMap *			NewPropertyMap () const = 0; // CONST?
	/** @brief	Creates a new property map using an existing property map
	 
	 The component descriptor owns the reference to the resulting property map, and
	 the underlying property map is destroyed when the component descriptor is
	 released.
	 
		\param[in] iPropertyMap
			The property values in this map will be copied into the new map
	 
	 */
	virtual AAX_IPropertyMap *			DuplicatePropertyMap (AAX_IPropertyMap* iPropertyMap) const = 0; // CONST?
	/** \brief	Registers an algorithm processing entrypoint (process procedure) for the
	 *	native architecture
	 *
	 *	\param[in]	iProcessProc
	 *		Symbol for this processing callback
	 *	\param[in]	iProperties
	 *		A property map for this processing callback. The property map's values are copied
	 *		by the host and associated with the new ProcessProc. The property map contents are
	 *		unchanged and the map may be re-used when registering additional ProcessProcs.
	 *	\param[in]	iInstanceInitProc
	 *		Initialization routine that will be called when a new instance of the Effect
	 *		is created.  See \ref alg_initialization.
	 *	\param[in]	iBackgroundProc
	 *		Background routine that will be called in an idle context within the same
	 *		address space as the associated process procedure.  See
	 *		\ref additionalFeatures_BackgroundProc
	 *	\param[in]	oProcID
	 *		\todo document this parameter
	 */
	virtual AAX_Result					AddProcessProc_Native (	
		AAX_CProcessProc iProcessProc,	
		AAX_IPropertyMap * iProperties = NULL, 
		AAX_CInstanceInitProc iInstanceInitProc = NULL, 
		AAX_CBackgroundProc iBackgroundProc = NULL, 
		AAX_CSelector * oProcID = NULL) = 0;
	/** \brief	Registers an algorithm processing entrypoint (process procedure) for the
	 *	native architecture
	 *
	 *	\param[in]	inDLLFileNameUTF8
	 *		UTF-8 encoded filename for the ELF DLL containing the algorithm code fragment
	 *	\param[in]	iProcessProcSymbol
	 *		Symbol for this processing callback
	 *	\param[in]	iProperties
	 *		A property map for this processing callback. The property map's values are copied
	 *		by the host and associated with the new ProcessProc. The property map contents are
	 *		unchanged and the map may be re-used when registering additional ProcessProcs.
	 *	\param[in]	iInstanceInitProcSymbol
	 *		Initialization routine that will be called when a new instance of the Effect
	 *		is created.  Must be included in the same DLL as the main algorithm
	 *		entrypoint.  See \ref alg_initialization.
	 *	\param[in]	iBackgroundProcSymbol
	 *		Background routine that will be called in an idle context within the same
	 *		address space as the associated process procedure.  Must be included in the
	 *		same DLL as the main algorithm entrypoint.  See
	 *		\ref additionalFeatures_BackgroundProc
	 *	\param[in]	oProcID
	 *		\todo document this parameter
	 */
	virtual AAX_Result					AddProcessProc_TI ( 
		const char inDLLFileNameUTF8 [], 
		const char iProcessProcSymbol [], 
		AAX_IPropertyMap * iProperties = NULL, 
		const char iInstanceInitProcSymbol [] = NULL,
		const char iBackgroundProcSymbol [] = NULL,
		AAX_CSelector * oProcID = NULL) = 0;


	/** \brief	Registers an algorithm processing entrypoint (process procedure) for the
	 *	native architecture
	 *
	 *	This template provides an \ref AAX_CALLBACK based interface to the
	 *	\ref AddProcessProc_Native method.
	 *
	 *	\par	iProcessProc
	 *		AAX_CALLBACK for this processing callback
	 *	\par	iProperties
	 *		A property map for this processing callback. The property map's values are copied
	 *		by the host and associated with the new ProcessProc. The property map contents are
	 *		unchanged and the map may be re-used when registering additional ProcessProcs.
	 *	\par	iInstanceInitProc
	 *		AAX_CALLBACK initialization routine that will be called when a new instance
	 *		of the Effect is created.  See \ref alg_initialization.
	 *	\par	iBackgroundProc
	 *		AAX_CALLBACK background routine that will be called in an idle context within
	 *		the same address space as the associated process procedure.  See
	 *		\ref additionalFeatures_BackgroundProc
	 */
	template <typename aContextType>
	AAX_Result AddProcessProc_Native ( 
		void (AAX_CALLBACK *iProcessProc) ( aContextType * const iInstancesBegin [], const void * iInstancesEnd),
		AAX_IPropertyMap * iProperties = NULL,
		int32_t (AAX_CALLBACK *iInstanceInitProc) ( const aContextType * iInstanceContextPtr, AAX_EComponentInstanceInitAction iAction ) = NULL, 
		int32_t (AAX_CALLBACK *iBackgroundProc) ( void ) = NULL );
};

template <typename aContextType>
inline AAX_Result
AAX_IComponentDescriptor::AddProcessProc_Native ( 
	void (AAX_CALLBACK *iProcessProc) ( aContextType * const	iInstancesBegin [], const void * iInstancesEnd),
	AAX_IPropertyMap * iProperties,
	int32_t (AAX_CALLBACK *iInstanceInitProc) ( const aContextType * iInstanceContextPtr, AAX_EComponentInstanceInitAction iAction ),
	int32_t (AAX_CALLBACK *iBackgroundProc) ( void ) )
{
	return this->AddProcessProc_Native( 
		reinterpret_cast <AAX_CProcessProc>( iProcessProc ), 
		iProperties, 
		reinterpret_cast<AAX_CInstanceInitProc>( iInstanceInitProc ),
		reinterpret_cast<AAX_CBackgroundProc>( iBackgroundProc ) );
}

#endif // #ifndef _AAX_ICOMPONENTDESCRIPTOR_H_
