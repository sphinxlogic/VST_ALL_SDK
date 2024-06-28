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
 */
/*================================================================================================*/ 

#include "DemoMIDI_Parameters.h"
#include "DemoMIDI_Defs.h"
#include "DemoMIDI_Alg.h"

#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_CBinaryDisplayDelegate.h"
#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CNumberDisplayDelegate.h"

#include "AAX_ASSERT.h"

// *******************************************************************************
// ROUTINE:	Create
// *******************************************************************************
AAX_CEffectParameters *AAX_CALLBACK DemoMIDI_Parameters::Create()
{
	return new DemoMIDI_Parameters();
}

// *******************************************************************************
// METHOD:	DemoMIDI_Parameters
// *******************************************************************************
DemoMIDI_Parameters::DemoMIDI_Parameters () :
AAX_CEffectParameters()
{
}

// *******************************************************************************
// METHOD:	EffectInit
// *******************************************************************************
AAX_Result DemoMIDI_Parameters::EffectInit()
{	
	//Add my custom parameters.	
	AAX_CString bypassID = cDefaultMasterBypassID;
	AAX_IParameter * masterBypass = new AAX_CParameter<bool>(
		 bypassID.CString(), AAX_CString("Master Bypass"), false, 
		 AAX_CBinaryTaperDelegate<bool>(),
		 AAX_CBinaryDisplayDelegate<bool>("bypass", "on"), true);
	masterBypass->SetNumberOfSteps( 2 );
	masterBypass->SetType( AAX_eParameterType_Discrete );
	mParameterManager.AddParameter(masterBypass);
	
	AAX_CParameter<float>* gainParameter = new AAX_CParameter<float>(DemoMIDI_GainID, AAX_CString("Gain"), 0.5f, AAX_CLinearTaperDelegate<float>(0.0f, 1.0f), AAX_CNumberDisplayDelegate<float>(), true);
	gainParameter->SetNumberOfSteps(55);
	mParameterManager.AddParameter(gainParameter);
	
	// register packets
	mPacketDispatcher.RegisterPacket(bypassID.CString(), eAlgPortID_BypassIn);
	mPacketDispatcher.RegisterPacket(DemoMIDI_GainID, eAlgPortID_CoefsGainIn);
	
	return AAX_SUCCESS;
}


// ***************************************************************************
// METHOD:	AAX_UpdateMIDINodes
// This will be called by the host if there are MIDI packets that need
// to be handled in the Data Model.
// ***************************************************************************
AAX_Result DemoMIDI_Parameters::UpdateMIDINodes ( AAX_CFieldIndex inFieldIndex,	AAX_CMidiPacket& inPacket )
{
	AAX_Result result;
	
	//
	// Do some MIDI work if necessary.
	
	if ( (inPacket.mData[0] & 0xF0) == 0x90 )
	{
		if ( inPacket.mData[2] == 0x00 )
		{
			//	Note Off
		}
		else
		{
			// Note On
		}
	}
	
 	result = AAX_SUCCESS;
	
	return result;
}




