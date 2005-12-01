// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnlyLoadable.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnlyLoadable.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFirePionOnlyLoadable.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnlyLoadable constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnlyLoadable::PHY_ActionDirectFirePionOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFirePion_ABC( pion, diaCall, PHY_DirectFireData::eFireComposantesLoadable )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnlyLoadable destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnlyLoadable::~PHY_ActionDirectFirePionOnlyLoadable()
{
}
