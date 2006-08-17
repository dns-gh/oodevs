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

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/RC/MIL_RC_PopulationVictimeAffrontements.h"
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
    NET_ASN_MsgStartPionFire asnMsg;
    asnMsg.GetAsnMsg().m.munitionPresent = 0;
    asnMsg.GetAsnMsg().oid_tir           = nID_;
    asnMsg.GetAsnMsg().tireur            = firer.GetID();
    asnMsg.GetAsnMsg().type              = MsgStartPionFire_type::direct;
    asnMsg.GetAsnMsg().cible.t           = T_MsgStartPionFire_cible_pion;
    asnMsg.GetAsnMsg().cible.u.pion      = target.GetID();    
    
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
    NET_ASN_MsgStartPionFire asnMsg;
    asnMsg.GetAsnMsg().m.munitionPresent  = 0;
    asnMsg.GetAsnMsg().oid_tir            = nID_;
    asnMsg.GetAsnMsg().tireur             = firer.GetID();
    asnMsg.GetAsnMsg().type               = MsgStartPionFire_type::direct;
    asnMsg.GetAsnMsg().cible.t            = T_MsgStartPionFire_cible_population;
    asnMsg.GetAsnMsg().cible.u.population = target.GetID();    
    
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
    NET_ASN_MsgStartPionFire asnMsg;
    asnMsg.GetAsnMsg().m.munitionPresent = 1;
    asnMsg.GetAsnMsg().oid_tir           = nID_;
    asnMsg.GetAsnMsg().tireur            = firer.GetID();
    asnMsg.GetAsnMsg().type              = MsgStartPionFire_type::indirect;
    asnMsg.GetAsnMsg().cible.t           = T_MsgStartPionFire_cible_position;
    asnMsg.GetAsnMsg().cible.u.position  = new ASN1T_CoordUTM();
    asnMsg.GetAsnMsg().munition          = dotationCategory.GetMosID();
    NET_ASN_Tools::WritePoint( targetPosition, *asnMsg.GetAsnMsg().cible.u.position );

    asnMsg.Send();

    delete asnMsg.GetAsnMsg().cible.u.position;   
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::~PHY_FireResults_Pion()
{
    NET_ASN_MsgStopPionFire asnMsg;
    asnMsg.GetAsnMsg().oid_tir = nID_;

    Serialize( asnMsg.GetAsnMsg().degats_pions       );
    Serialize( asnMsg.GetAsnMsg().degats_populations );
   
    asnMsg.Send();

    CleanAfterSerialization( asnMsg.GetAsnMsg().degats_pions       );
    CleanAfterSerialization( asnMsg.GetAsnMsg().degats_populations );

    // $$$ Merde pour VABF Popu
    static MT_Random randomGenerator;
    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( CIT_PopulationDamagesMap it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population&               population = *it->first;
        const PHY_FireDamages_Population&   damages    =  it->second;

        uint rNbrWounded = damages.GetNbrKilledHumans() * randomGenerator.rand_ii( 0.6, 0.75 );

        MIL_RC::pRcPopulationVictimeAffrontements_->Send( population, MIL_RC::eRcTypeMessage, damages.GetNbrKilledHumans(), rNbrWounded );
        MIL_RC::pRcPopulationVictimeAffrontements_->Send( firer_    , MIL_RC::eRcTypeMessage, damages.GetNbrKilledHumans(), rNbrWounded );
    }
}

