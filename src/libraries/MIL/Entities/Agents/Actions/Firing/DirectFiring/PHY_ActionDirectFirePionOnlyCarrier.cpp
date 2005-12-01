// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnlyCarrier.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnlyCarrier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFirePionOnlyCarrier.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnlyCarrier constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnlyCarrier::PHY_ActionDirectFirePionOnlyCarrier( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFirePion_ABC( pion, diaCall, PHY_DirectFireData::eFireComposantesCarrier )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnlyCarrier destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnlyCarrier::~PHY_ActionDirectFirePionOnlyCarrier()
{
}
