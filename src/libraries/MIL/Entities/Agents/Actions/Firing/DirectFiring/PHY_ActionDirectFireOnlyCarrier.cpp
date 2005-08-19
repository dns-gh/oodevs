// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFireOnlyCarrier.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFireOnlyCarrier.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFireOnlyCarrier.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFireOnlyCarrier constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFireOnlyCarrier::PHY_ActionDirectFireOnlyCarrier( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFire_ABC( pion, diaCall, PHY_DirectFireData::eFireComposantesCarrier )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFireOnlyCarrier destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFireOnlyCarrier::~PHY_ActionDirectFireOnlyCarrier()
{
}
