//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Version.cpp $
// $Author: Ape $
// $Modtime: 21/10/04 17:47 $
// $Revision: 1 $
// $Workfile: ADN_Version.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "MT_Tools/MT_Version.h"
#include "tools/Version.h"

const char* szADN_Version   = ( "ADN - " + std::string( tools::AppVersion() ) + " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ ).c_str();