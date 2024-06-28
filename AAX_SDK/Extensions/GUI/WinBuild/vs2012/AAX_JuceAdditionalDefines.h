/*===================================================================================================
AAX_JuceAdditionalDefines.h

Copyright (c) 2015 Avid, Inc. 
All rights reserved.
===================================================================================================*/

#if defined _MSC_VER
#if _MSC_VER <= 1900
#define fseeko fseek
#define ftello ftell
#endif
#endif
