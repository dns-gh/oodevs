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
    : firer_( firer )
    , nID_  ( idManager_.GetFreeId() )
{
    client::StartCrowdFire msg;
    msg().mutable_fire()->set_id( nID_ );
    msg().mutable_firing_crowd()->set_id( firer_.GetID() );
    msg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Population destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Population::~PHY_FireResults_Population()
{
    client::StopCrowdFire msg;
    msg().mutable_fire()->set_id( nID_ );
    Serialize( *msg().mutable_units_damages() );
    msg.Send( NET_Publisher_ABC::Publisher() );
    SendDamagesCrowd( firer_, nID_ );
}
