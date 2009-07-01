// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_SupplyState_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 12:12 $
// $Revision: 2 $
// $Workfile: PHY_SupplyState_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_SupplyState_ABC.h"
#include "Tools/MIL_IDManager.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC::PHY_SupplyState_ABC
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_SupplyState_ABC::PHY_SupplyState_ABC()
    : nID_          ( MIL_IDManager::GetFreeId() )
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
void PHY_SupplyState_ABC::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< uint& >( nID_ )
         >> const_cast< uint& >( nCreationTick_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_SupplyState_ABC::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_SupplyState_ABC::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nID_
         << nCreationTick_;
}
