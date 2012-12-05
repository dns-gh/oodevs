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
#include "MIL_AgentServer.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target )
    : firer_ ( firer )
    , nID_   ( idManager_.GetFreeId() )
    , direct_( true )
{
    client::StartUnitFire asnMsg;
    asnMsg().mutable_fire()->set_id( nID_ );
    asnMsg().mutable_firing_unit()->set_id( firer.GetID() );
    asnMsg().set_type( sword::StartUnitFire::direct );
    asnMsg().mutable_target()->mutable_unit()->set_id( target.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_PopulationElement_ABC& target )
    : firer_ ( firer )
    , nID_   ( idManager_.GetFreeId() )
    , direct_( true )
{
    client::StartUnitFire asnMsg;
    asnMsg().mutable_fire()->set_id( nID_ );
    asnMsg().mutable_firing_unit()->set_id( firer.GetID() );
    asnMsg().set_type( sword::StartUnitFire::direct );
    asnMsg().mutable_target()->mutable_crowd()->set_id( target.GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion constructor
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MT_Vector2D& targetPosition, const PHY_DotationCategory& dotationCategory )
    : firer_ ( firer )
    , nID_   ( idManager_.GetFreeId() )
    , direct_( false )
{
    client::StartUnitFire asnMsg;
    asnMsg().mutable_fire()->set_id( nID_ );
    asnMsg().mutable_firing_unit()->set_id( firer.GetID() );
    asnMsg().set_type( sword::StartUnitFire::indirect );
    asnMsg().mutable_ammunition()->set_id( dotationCategory.GetMosID() );
    NET_ASN_Tools::WritePoint( targetPosition, *asnMsg().mutable_target()->mutable_position() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion::DoSendReport
// Created: JSR 2011-09-01
// -----------------------------------------------------------------------------
template< typename T >
void PHY_FireResults_Pion::DoSendReport( const T& entity, bool& rcSent ) const
{
    if( entity.GetArmy().IsNeutral( firer_.GetArmy() ) == eTristate_True )
    {
        MIL_Report::PostEvent( entity, MIL_Report::eRC_DamagesCausedByNeutralSide );
        if( !rcSent )
        {
            MIL_Report::PostEvent( firer_, MIL_Report::eRC_DamagesCausedToNeutralSide );
            rcSent = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion::SendReport
// Created: JSR 2011-09-01
// -----------------------------------------------------------------------------
template< typename MSG >
void PHY_FireResults_Pion::SendReport( const MSG& msg, bool& rcSent ) const
{
    for( int i = 0; i < msg.elem_size(); ++i )
    {
        unsigned int targetId = msg.elem( i ).target().id();
        if( MIL_AgentPion* targetPion = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAgentPion( targetId ) )
            DoSendReport( *targetPion, rcSent );
        else if( MIL_Population* targetPop = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( targetId ) )
            DoSendReport( *targetPop, rcSent );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_Pion destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_Pion::~PHY_FireResults_Pion()
{
    {
        client::StopUnitFire asnMsg;
        asnMsg().mutable_fire()->set_id( nID_ );
        Serialize( *asnMsg().mutable_units_damages() );
        Serialize( *asnMsg().mutable_crowds_damages() );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
        bool rcSent = false;
        SendReport( asnMsg().units_damages(), rcSent );
        SendReport( asnMsg().crowds_damages(), rcSent );
    }
    SendDamagesPion( firer_, nID_, direct_ );
    // $$$$ Merde pour VABF Popu
    const T_PopulationDamagesMap& populationDamages = GetPopulationDamages();
    for( auto it = populationDamages.begin(); it != populationDamages.end(); ++it )
    {
        const MIL_Population& population = *it->first;
        const PHY_FireDamages_Population& damages =  it->second;
        unsigned int dead = damages.GetNbrKilledHumans();
        unsigned int wounded = damages.GetNbrWoundedHumans();
        unsigned int scattered = damages.GetNbrScatteredHumans();
        MIL_Report::PostEvent( population, MIL_Report::eRC_PopulationVictimeAffrontements, dead, wounded, scattered );
        MIL_Report::PostEvent( firer_    , MIL_Report::eRC_PopulationVictimeAffrontements, dead, wounded, scattered );
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
