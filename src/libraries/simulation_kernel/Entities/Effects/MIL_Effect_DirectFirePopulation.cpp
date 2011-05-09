// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_DirectFirePopulation.cpp $
// $Author: Nld $
// $Modtime: 30/03/05 11:33 $
// $Revision: 4 $
// $Workfile: MIL_Effect_DirectFirePopulation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_DirectFirePopulation.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Entities/Actions/PHY_FireResults_ABC.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePopulation constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFirePopulation::MIL_Effect_DirectFirePopulation( MIL_PopulationElement_ABC& target, unsigned int nNbrAmmoFired, PHY_FireResults_ABC& fireResult )
    : target_       ( target )
    , nNbrAmmoFired_( nNbrAmmoFired )
    , fireResult_   ( fireResult )
{
    fireResult_.IncRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePopulation destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
MIL_Effect_DirectFirePopulation::~MIL_Effect_DirectFirePopulation()
{
    fireResult_.DecRef();
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DirectFirePopulation::Execute
// Created: NLD 2004-09-23
// -----------------------------------------------------------------------------
bool MIL_Effect_DirectFirePopulation::Execute()
{
    target_.ApplyFire( nNbrAmmoFired_, fireResult_, true );
    delete this;
    return false; // Effect must be stopped
}
