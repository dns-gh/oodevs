// *****************************************************************************
//
// $Created: JVT 2005-05-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.cpp $
// $Author: Jvt $
// $Modtime: 12/05/05 15:25 $
// $Revision: 1 $
// $Workfile: PHY_ActionLendCollectionComposantes.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionLendCollectionComposantes.h"

#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendCollectionComposantes constructor
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
PHY_ActionLendCollectionComposantes::PHY_ActionLendCollectionComposantes( MIL_AgentPion& pion, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, unsigned int nbrToLend )
    : PHY_ActionLendComposantes( pion, pStart, pAgent, nbrToLend, &PHY_ComposantePion::CanCollectCasualties )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendCollectionComposantes destructor
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
PHY_ActionLendCollectionComposantes::~PHY_ActionLendCollectionComposantes()
{

}
