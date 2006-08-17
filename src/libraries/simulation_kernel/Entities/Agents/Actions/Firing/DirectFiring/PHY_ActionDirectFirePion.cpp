// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionDirectFirePion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion::PHY_ActionDirectFirePion( MIL_AgentPion& pion, DIA_Call_ABC& diaCall )
: PHY_ActionDirectFirePion_ABC( pion, diaCall, PHY_DirectFireData::eFireUsingAllComposantes, PHY_DirectFireData::eFireOnAllComposantes )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion::~PHY_ActionDirectFirePion()
{

}
