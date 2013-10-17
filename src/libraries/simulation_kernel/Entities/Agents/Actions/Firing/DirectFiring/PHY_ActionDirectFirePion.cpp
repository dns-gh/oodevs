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
#include "Decision/DEC_Decision_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion::PHY_ActionDirectFirePion( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode, int ammoDotationClass )
: PHY_ActionDirectFirePion_ABC( pion, pEnemy, percentage, firingMode, ammoDotationClass, firing::PHY_DirectFireData::eFireUsingAllComposantes, firing::PHY_DirectFireData::eFireOnAllComposantes )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePion destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePion::~PHY_ActionDirectFirePion()
{

}
