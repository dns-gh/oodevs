// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionUsingOnlyCarrier.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDirectFirePionUsingOnlyCarrier.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionUsingOnlyCarrier constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionUsingOnlyCarrier::PHY_ActionDirectFirePionUsingOnlyCarrier( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, MT_Float percentage, int firingMode )
    : PHY_ActionDirectFirePion_ABC( pion, pEnemy, percentage, firingMode, -1, PHY_DirectFireData::eFireUsingOnlyComposantesCarrier, PHY_DirectFireData::eFireOnAllComposantes )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionUsingOnlyCarrier destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionUsingOnlyCarrier::~PHY_ActionDirectFirePionUsingOnlyCarrier()
{
    // NOTHING
}
