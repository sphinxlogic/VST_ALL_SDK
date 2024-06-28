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
 *	\file  AAX_VComponentDescriptor.h
 *
 *	\brief Version-managed concrete ComponentDescriptor class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VCOMPONENTDESCRIPTOR_H
#define AAX_VCOMPONENTDESCRIPTOR_H

#include "AAX_IComponentDescriptor.h"
#include "AAX_IDma.h"
#include "AAX_IACFComponentDescriptor.h"
#include "acfunknown.h"
#include "ACFPtr.h"
#include <set>


class AAX_IPropertyMap;
class AAX_IACFComponentDescriptor;
class AAX_IACFComponentDescriptorV2;
class IACFUnknown;

/**
 *	\brief Version-managed concrete \ref AAX_IComponentDescriptor class
 *
 */
class AAX_VComponentDescriptor : public AAX_IComponentDescriptor
{
public:
	AAX_VComponentDescriptor ( IACFUnknown * pUnkHost );
	virtual ~AAX_VComponentDescriptor ();
	
	virtual AAX_Result			Clear ();
	virtual AAX_Result			AddReservedField ( AAX_CFieldIndex inFieldIndex, uint32_t inFieldType );
	virtual AAX_Result			AddAudioIn ( AAX_CFieldIndex inFieldIndex );
	virtual AAX_Result			AddAudioOut ( AAX_CFieldIndex inFieldIndex );
	virtual AAX_Result			AddAudioBufferLength ( AAX_CFieldIndex inFieldIndex );
	virtual AAX_Result			AddSampleRate ( AAX_CFieldIndex inFieldIndex );
	virtual AAX_Result			AddClock ( AAX_CFieldIndex inFieldIndex );
	virtual AAX_Result			AddSideChainIn ( AAX_CFieldIndex inFieldIndex );

	virtual AAX_Result			AddDataInPort ( AAX_CFieldIndex inFieldIndex, uint32_t inPacketSize, AAX_EDataInPortType inPortType );
	virtual AAX_Result			AddAuxOutputStem ( AAX_CFieldIndex inFieldIndex, int32_t inStemFormat, const char inNameUTF8[]);
	virtual AAX_Result			AddPrivateData ( AAX_CFieldIndex inFieldIndex, int32_t inDataSize, uint32_t /* AAX_EPrivateDataOptions */ inOptions = AAX_ePrivateDataOptions_DefaultOptions );
    virtual AAX_Result          AddTemporaryData( AAX_CFieldIndex iFieldIndex, uint32_t iDataElementSize);
	virtual AAX_Result			AddDmaInstance ( AAX_CFieldIndex inFieldIndex, AAX_IDma::EMode inDmaMode );
	virtual AAX_Result			AddMeters ( AAX_CFieldIndex inFieldIndex, const AAX_CTypeID* inMeterIDs, const uint32_t inMeterCount);
	virtual AAX_Result			AddMIDINode ( AAX_CFieldIndex inFieldIndex, AAX_EMIDINodeType inNodeType, const char inNodeName[], uint32_t channelMask );


	/** \copydoc AAX_IComponentDescriptor::NewPropertyMap()
	 *	
	 *	This implementation retains each generated \ref AAX_IPropertyMap and destroys the property map upon AAX_VComponentDescriptor destruction
	 */
	virtual AAX_IPropertyMap *	NewPropertyMap () const;
	/** \copydoc AAX_IComponentDescriptor::DuplicatePropertyMap()
	 *	
	 *	This implementation retains each generated \ref AAX_IPropertyMap and destroys the property map upon AAX_VComponentDescriptor destruction
	 */
	virtual AAX_IPropertyMap *	DuplicatePropertyMap (AAX_IPropertyMap* iPropertyMap) const;
	virtual AAX_Result			AddProcessProc_Native ( 
		AAX_CProcessProc inProcessProc, 
		AAX_IPropertyMap * inProperties = NULL, 
		AAX_CInstanceInitProc inInstanceInitProc = NULL, 
		AAX_CBackgroundProc inBackgroundProc = NULL, 
		AAX_CSelector * outProcID = NULL );
	virtual AAX_Result			AddProcessProc_TI ( 
		const char inDLLFileNameUTF8[], 
		const char inProcessProcSymbol[], 
		AAX_IPropertyMap * inProperties = NULL,  
		const char	inInstanceInitProcSymbol [] = NULL, 
		const char	inBackgroundProcSymbol [] = NULL, 
		AAX_CSelector * outProcID = NULL );


	IACFUnknown*				GetIUnknown(void) const;

private:
	ACFPtr<IACFUnknown>					mUnkHost;
	ACFPtr<AAX_IACFComponentDescriptor>	mIACFComponentDescriptor;
    ACFPtr<AAX_IACFComponentDescriptor_V2> mIACFComponentDescriptorV2;
	std::set<AAX_IPropertyMap *>		mPropertyMaps;	
};


#endif // #ifndef _AAX_ICOMPONENTDESCRIPTOR_H_
