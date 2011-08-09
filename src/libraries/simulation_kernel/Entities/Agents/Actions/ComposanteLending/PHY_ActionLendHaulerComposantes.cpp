// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:25 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendHaulerComposantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionLendHaulerComposantes.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendHaulerComposantes constructor
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
PHY_ActionLendHaulerComposantes::PHY_ActionLendHaulerComposantes( MIL_AgentPion& pion, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, unsigned int nbrToLend )
    : PHY_ActionLendComposantes( pion, pStart, pAgent, nbrToLend, &PHY_ComposantePion::CanHaul )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendHaulerComposantes destructor
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
PHY_ActionLendHaulerComposantes::~PHY_ActionLendHaulerComposantes()
{

}
