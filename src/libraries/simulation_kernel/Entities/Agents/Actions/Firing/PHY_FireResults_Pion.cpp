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
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target )
    : PHY_FireResults_ABC()
    , firer_( firer )
    , nID_  ( idManager_.GetFreeId() )
{
    client::StartUnitFire asnMsg;
    asnMsg().set_fire_oid( nID_ );
    asnMsg().set_firer_oid( firer.GetID() );
    asnMsg().set_type( Common::direct );
    asnMsg().mutable_target()->set_unit( target.GetID() );

    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_Population& target )
    : PHY_FireResults_ABC()
    , firer_( firer )
    , nID_  ( idManager_.GetFreeId() )
{
    client::StartUnitFire asnMsg;
    asnMsg().set_fire_oid( nID_ );
    asnMsg().set_firer_oid( firer.GetID() );
    asnMsg().set_type( Common::direct );
    asnMsg().mutable_target()->set_population( target.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MT_Vector2D& targetPosition, const PHY_DotationCategory& dotationCategory )
    : PHY_FireResults_ABC()
    , firer_( firer )
    , nID_  ( idManager_.GetFreeId() )
{
    client::StartUnitFire asnMsg;
    asnMsg().set_fire_oid( nID_ );
    asnMsg().set_firer_oid( firer.GetID() );
    asnMsg().set_type( Common::indirect );
    asnMsg().set_ammunition( dotationCategory.GetMosID() );
    NET_ASN_Tools::WritePoint( targetPosition, *asnMsg().mutable_target()->mutable_position() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::~PHY_FireResults_Pion()
{
    {
        client::StopUnitFire asnMsg;
        asnMsg().set_fire_oid( nID_ );
        Serialize( *asnMsg().mutable_units_damages() );
        Serialize( *asnMsg().mutable_populations_damages() );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }
    // $$$ Merde pour VABF Popu
    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( CIT_PopulationDamagesMap it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population& population = *it->first;
        const PHY_FireDamages_Population& damages = it->second;
        unsigned int rNbrWounded = static_cast< unsigned int >( damages.GetNbrKilledHumans() * MIL_Random::rand_ii( 0.6, 0.75, MIL_Random::eFire ) );
        MIL_Report::PostEvent( population, MIL_Report::eReport_ConfrontationWithPopulation, damages.GetNbrKilledHumans(), rNbrWounded );
        MIL_Report::PostEvent( firer_, MIL_Report::eReport_ConfrontationWithPopulation, damages.GetNbrKilledHumans(), rNbrWounded );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion::GetID
// Created: NLD 2005-02-21
// -----------------------------------------------------------------------------
unsigned int PHY_FireResults_Pion::GetID() const
{
    return nID_;
}
