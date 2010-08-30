// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PopulationFiring/PHY_FireResults_Population.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_Population.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_FireResults_Population.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Population constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Population::PHY_FireResults_Population( const MIL_Population& firer )
    : PHY_FireResults_ABC()
    , firer_( firer )
    , nID_  ( idManager_.GetFreeId() )
{
    client::StartPopulationFire asnMsg;
    asnMsg().mutable_id()->set_id( nID_ );
    asnMsg().mutable_firing_population()->set_id( firer_.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Population destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Population::~PHY_FireResults_Population()
{
    client::StopPopulationFire asnMsg;
    asnMsg().mutable_id()->set_id( nID_ );
    Serialize( *asnMsg().mutable_units_damages() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}
