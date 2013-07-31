// *****************************************************************************
//
// $Created: LDC 2012-08-03
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionLendSpecificComposantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"

namespace
{
    bool CheckComposante( const PHY_ComposantePion& composante, PHY_ComposanteTypePion* type )
    {
        return &composante.GetType() == type;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendSpecificComposantes constructor
// Created: LDC 2012-08-03
// -----------------------------------------------------------------------------
PHY_ActionLendSpecificComposantes::PHY_ActionLendSpecificComposantes( MIL_AgentPion& pion, DEC_Decision_ABC* pStart, DEC_Decision_ABC* pAgent, PHY_ComposanteTypePion* type, unsigned int nbrToLend )
    : PHY_ActionLendComposantes( pion, pStart, pAgent, nbrToLend, boost::bind( &CheckComposante, _1, type ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLendSpecificComposantes destructor
// Created: LDC 2012-08-03
// -----------------------------------------------------------------------------
PHY_ActionLendSpecificComposantes::~PHY_ActionLendSpecificComposantes()
{
    // NOTHING
}
