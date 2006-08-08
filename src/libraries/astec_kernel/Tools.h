// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Kernel_Tools_h_
#define __Kernel_Tools_h_

#include "Types.h"

// =============================================================================
/** @class  Tools
    @brief  Crappy translation tools  
*/
// Created: AGE 2006-03-15
// =============================================================================
namespace tools
{
    const char* ToString( E_UnitPosture nPosture );
    const char* ToString( E_TroopHealthState nState );
    const char* ToString( E_PerceptionResult nResult );
    const char* ToString( E_WeatherType nWeatherType );
    const char* ToString( E_LightingType nLightingType );
};

#endif // __Kernel_Tools_h_
