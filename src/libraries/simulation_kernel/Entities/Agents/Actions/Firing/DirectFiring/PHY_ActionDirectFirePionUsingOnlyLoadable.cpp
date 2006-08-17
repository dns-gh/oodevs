// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionUsingOnlyLoadable.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_ActionDirectFirePionUsingOnlyLoadable.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionUsingOnlyLoadable constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionUsingOnlyLoadable::PHY_ActionDirectFirePionUsingOnlyLoadable( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFirePion_ABC( pion, diaCall, PHY_DirectFireData::eFireUsingOnlyComposantesLoadable, PHY_DirectFireData::eFireOnAllComposantes )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionUsingOnlyLoadable destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionUsingOnlyLoadable::~PHY_ActionDirectFirePionUsingOnlyLoadable()
{
}
