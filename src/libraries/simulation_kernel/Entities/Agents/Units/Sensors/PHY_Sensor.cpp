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

#include "simulation_kernel_pch.h"
#include "PHY_Sensor.h"

// -----------------------------------------------------------------------------
// Name: PHY_Sensor constructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor::PHY_Sensor( const PHY_SensorType& type, MT_Float rHeight )
    : type_   ( type )
    , rHeight_( rHeight )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Sensor destructor
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
PHY_Sensor::~PHY_Sensor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Sensor::GetHeight
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
MT_Float PHY_Sensor::GetHeight() const
{
    return rHeight_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Sensor::GetType
// Created: NLD 2004-08-20
// -----------------------------------------------------------------------------
const PHY_SensorType& PHY_Sensor::GetType() const
{
    return type_;
}
