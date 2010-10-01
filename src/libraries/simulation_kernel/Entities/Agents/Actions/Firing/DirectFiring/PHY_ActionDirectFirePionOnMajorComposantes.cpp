// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.cpp $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_ActionDirectFirePionOnMajorComposantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionDirectFirePionOnMajorComposantes.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnMajorComposantes constructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnMajorComposantes::PHY_ActionDirectFirePionOnMajorComposantes( MIL_AgentPion& pion, int ammoDotationClass, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy, double percentage, int firingMode )
    : PHY_ActionDirectFirePion_ABC( pion, pEnemy, percentage, firingMode, ammoDotationClass, PHY_DirectFireData::eFireUsingAllComposantes, PHY_DirectFireData::eFireOnlyOnMajorComposantes )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDirectFirePionOnMajorComposantes destructor
// Created: NLD 2004-08-18
// -----------------------------------------------------------------------------
PHY_ActionDirectFirePionOnMajorComposantes::~PHY_ActionDirectFirePionOnMajorComposantes()
{
}
