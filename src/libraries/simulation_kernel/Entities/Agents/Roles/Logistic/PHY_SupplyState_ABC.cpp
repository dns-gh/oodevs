// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_SupplyState_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 12:12 $
// $Revision: 2 $
// $Workfile: PHY_SupplyState_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyState_ABC.h"
#include "MIL_AgentServer.h"

MIL_IDManager PHY_SupplyState_ABC::idManager_;

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC::PHY_SupplyState_ABC
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyState_ABC::PHY_SupplyState_ABC()
    : nID_          ( idManager_.GetFreeId() )
    , nCreationTick_( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_SupplyState_ABC::~PHY_SupplyState_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_SupplyState_ABC::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< unsigned int& >( nCreationTick_ );
    idManager_.Lock( nID_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_SupplyState_ABC::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nID_
         << nCreationTick_;
}
