// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFire.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFire.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_ActionDirectFire.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFire::PHY_ActionDirectFire( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
    : PHY_ActionDirectFire_ABC( pion, diaCall, PHY_DirectFireData::eFireAllComposantes )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFire destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFire::~PHY_ActionDirectFire()
{

}
