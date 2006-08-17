// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Sensors/PHY_Sensor.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_Sensor.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_Sensor.h"

// -----------------------------------------------------------------------------
// Name: PHY_Sensor constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor::PHY_Sensor( const PHY_SensorType& type, MT_Float rHeight )
    : type_   ( type )
    , rHeight_( rHeight )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_Sensor destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor::~PHY_Sensor()
{

}
