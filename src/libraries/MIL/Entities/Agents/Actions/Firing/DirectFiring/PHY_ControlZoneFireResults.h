// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ControlZoneFireResults.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_ControlZoneFireResults.h $
//
// *****************************************************************************

#ifndef __PHY_ControlZoneFireResults_h_
#define __PHY_ControlZoneFireResults_h_

#include "MIL.h"

#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"

// =============================================================================
// @class  PHY_ControlZoneFireResults
// Created: JVT 2004-08-03
// =============================================================================
class PHY_ControlZoneFireResults : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_ControlZoneFireResults )

public:
    PHY_ControlZoneFireResults();
    virtual ~PHY_ControlZoneFireResults();
};

#include "PHY_ControlZoneFireResults.inl"

#endif // __PHY_ControlZoneFireResults_h_
