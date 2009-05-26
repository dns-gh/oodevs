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
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Population constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Population::PHY_FireResults_Population( const MIL_Population& firer )
    : PHY_FireResults_ABC()
    , firer_             ( firer )
    , nID_               ( MIL_IDManager::GetFreeId() )
{
    NET_ASN_MsgStartPopulationFire asnMsg;
    asnMsg().fire_oid  = nID_;
    asnMsg().firer_oid = firer_.GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Population destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Population::~PHY_FireResults_Population()
{
    NET_ASN_MsgStopPopulationFire asnMsg;
    asnMsg().fire_oid = nID_;

    Serialize( asnMsg().units_damages );

    asnMsg.Send();

    CleanAfterSerialization( asnMsg().units_damages );
}

