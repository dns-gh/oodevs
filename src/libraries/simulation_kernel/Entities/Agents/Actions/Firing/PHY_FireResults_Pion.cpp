// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_FireResults_Pion.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_Pion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_FireResults_Pion.h"

#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Orders/MIL_Report.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_AgentPion& firer, const MIL_Agent_ABC& target )
    : PHY_FireResults_ABC()
    , firer_             ( firer )
    , nID_               ( MIL_IDManager::fireResultsPion_.GetFreeSimID() )
{
    NET_ASN_MsgStartUnitFire asnMsg;
    asnMsg().m.ammunitionPresent = 0;
    asnMsg().fire_oid            = nID_;
    asnMsg().firer_oid           = firer.GetID();
    asnMsg().type                = MsgStartUnitFire_type::direct;
    asnMsg().target.t            = T_MsgStartUnitFire_target_unit;
    asnMsg().target.u.unit       = target.GetID();    
    
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_AgentPion& firer, const MIL_Population& target )
    : PHY_FireResults_ABC()
    , firer_             ( firer )
    , nID_               ( MIL_IDManager::fireResultsPion_.GetFreeSimID() )
{
    NET_ASN_MsgStartUnitFire asnMsg;
    asnMsg().m.ammunitionPresent = 0;
    asnMsg().fire_oid            = nID_;
    asnMsg().firer_oid           = firer.GetID();
    asnMsg().type                = MsgStartUnitFire_type::direct;
    asnMsg().target.t            = T_MsgStartUnitFire_target_population;
    asnMsg().target.u.population = target.GetID();    
    
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_AgentPion& firer, const MT_Vector2D& targetPosition, const PHY_DotationCategory& dotationCategory )
    : PHY_FireResults_ABC()
    , firer_             ( firer )
    , nID_               ( MIL_IDManager::fireResultsPion_.GetFreeSimID() )
{
    NET_ASN_MsgStartUnitFire asnMsg;
    asnMsg().m.ammunitionPresent = 1;
    asnMsg().fire_oid            = nID_;
    asnMsg().firer_oid           = firer.GetID();
    asnMsg().type                = MsgStartUnitFire_type::indirect;
    asnMsg().target.t            = T_MsgStartUnitFire_target_position;
    asnMsg().target.u.position   = new ASN1T_CoordLatLong();
    asnMsg().ammunition          = dotationCategory.GetMosID();
    NET_ASN_Tools::WritePoint( targetPosition, *asnMsg().target.u.position );

    asnMsg.Send();

    delete asnMsg().target.u.position;   
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::~PHY_FireResults_Pion()
{
    NET_ASN_MsgStopUnitFire asnMsg;
    asnMsg().fire_oid = nID_;

    Serialize( asnMsg().units_damages       );
    Serialize( asnMsg().populations_damages );
   
    asnMsg.Send();

    CleanAfterSerialization( asnMsg().units_damages       );
    CleanAfterSerialization( asnMsg().populations_damages );

    // $$$ Merde pour VABF Popu
    static MT_Random randomGenerator;
    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( CIT_PopulationDamagesMap it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population&               population = *it->first;
        const PHY_FireDamages_Population&   damages    =  it->second;

        uint rNbrWounded = damages.GetNbrKilledHumans() * randomGenerator.rand_ii( 0.6, 0.75 );

        MIL_Report::PostEvent( population, MIL_Report::eReport_ConfrontationWithPopulation, damages.GetNbrKilledHumans(), rNbrWounded );
        MIL_Report::PostEvent( firer_    , MIL_Report::eReport_ConfrontationWithPopulation, damages.GetNbrKilledHumans(), rNbrWounded );
    }
}


