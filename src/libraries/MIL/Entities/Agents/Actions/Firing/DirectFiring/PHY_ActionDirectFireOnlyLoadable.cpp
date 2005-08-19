// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFireOnlyLoadable.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFireOnlyLoadable.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFireOnlyLoadable.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFireOnlyLoadable constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFireOnlyLoadable::PHY_ActionDirectFireOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFire_ABC( pion, diaCall, PHY_DirectFireData::eFireComposantesLoadable )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFireOnlyLoadable destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFireOnlyLoadable::~PHY_ActionDirectFireOnlyLoadable()
{
}
