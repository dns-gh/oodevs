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
#include <qstring.h>

// =============================================================================
/** @class  Tools
    @brief  Crappy translation tools  
*/
// Created: AGE 2006-03-15
// =============================================================================
namespace tools
{
    QString translate( const char* context, const char* what );

    QString ToString( kernel::E_TroopHealthState nState );
    QString ToString( kernel::E_PerceptionResult nResult );

    QString Unknown();

    const char* GetXmlSection( E_UnitPosture nPosture );
    const char* GetXmlSection( kernel::E_WeatherType nWeatherType );
    const char* GetXmlSection( kernel::E_LightingType nLightingType );
};

#endif // __Kernel_Tools_h_
