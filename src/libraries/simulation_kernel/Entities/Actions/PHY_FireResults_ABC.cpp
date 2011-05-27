// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PHY_FireResults_ABC.cpp $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 3 $
// $Workfile: PHY_FireResults_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_FireResults_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

MIL_IDManager PHY_FireResults_ABC::idManager_;

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC constructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_ABC::PHY_FireResults_ABC()
    : nNbrRefs_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC destructor
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_FireResults_ABC::~PHY_FireResults_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::Serialize
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Serialize( sword::UnitsFireDamages& asn ) const
{
    for( CIT_AgentDamagesMap it = agentsDamages_.begin(); it != agentsDamages_.end(); ++it )
        it->second.Serialize( *it->first, *asn.add_elem() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::Serialize
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Serialize( sword::CrowdsFireDamages& asn ) const
{
    for( CIT_PopulationDamagesMap it = populationsDamages_.begin(); it != populationsDamages_.end(); ++it )
        it->second.Serialize( *it->first, *asn.add_elem() );
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::SendDamagesPion
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::SendDamagesPion( const MIL_Agent_ABC& firer, unsigned int fireId, bool direct ) const
{
    for( CIT_AgentDamagesMap it = agentsDamages_.begin(); it != agentsDamages_.end(); ++it )
    {
        client::UnitDamagedByUnitFire msg;
        msg().mutable_unit()->set_id( it->first->GetID() );
        msg().mutable_party()->set_id( it->first->GetArmy().GetID() );
        msg().mutable_firer()->set_id( firer.GetID() );
        msg().mutable_fire()->set_id( fireId );
        msg().set_direct_fire( direct );
        msg().set_fratricide( firer.GetArmy().IsAFriend( it->first->GetArmy() ) == eTristate_True );
        it->second.SerializeDamages( msg() );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
    for( CIT_PopulationDamagesMap it = populationsDamages_.begin(); it != populationsDamages_.end(); ++it )
    {
        client::CrowdDamagedByUnitFire msg;
        msg().mutable_crowd()->set_id( it->first->GetID() );
        msg().mutable_party()->set_id( it->first->GetArmy().GetID() );
        msg().mutable_firer()->set_id( firer.GetID() );
        msg().mutable_fire()->set_id( fireId );
        msg().set_direct_fire( direct );
        msg().set_fratricide( firer.GetArmy().IsAFriend( it->first->GetArmy() ) == eTristate_True );
        it->second.SerializeDamages( msg() );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::SendDamagesCrowd
// Created: JSR 2011-05-13
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::SendDamagesCrowd( const MIL_Population& firer, unsigned int fireId ) const
{
    for( CIT_AgentDamagesMap it = agentsDamages_.begin(); it != agentsDamages_.end(); ++it )
    {
        client::UnitDamagedByCrowdFire msg;
        msg().mutable_unit()->set_id( it->first->GetID() );
        msg().mutable_party()->set_id( it->first->GetArmy().GetID() );
        msg().mutable_firer()->set_id( firer.GetID() );
        msg().mutable_fire()->set_id( fireId );
        msg().set_fratricide( firer.GetArmy().IsAFriend( it->first->GetArmy() ) == eTristate_True );
        it->second.SerializeDamages( msg() );
        msg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetDamages
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireDamages_Agent& PHY_FireResults_ABC::GetDamages( const MIL_Agent_ABC& target )
{
    return agentsDamages_[ &target ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetDamages
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
PHY_FireDamages_Population& PHY_FireResults_ABC::GetDamages( const MIL_Population& target )
{
    return populationsDamages_[ &target ];
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::IncRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::IncRef()
{
    ++nNbrRefs_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::DecRef
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::DecRef()
{
    assert( nNbrRefs_ > 0 );
    --nNbrRefs_;
    if( nNbrRefs_ == 0 )
        delete this;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetPopulationDamages
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
const PHY_FireResults_ABC::T_PopulationDamagesMap& PHY_FireResults_ABC::GetPopulationDamages() const
{
    return populationsDamages_;
}

// -----------------------------------------------------------------------------
// Name: PHY_FireResults_ABC::GetPopulationDamages
// Created: NLD 2006-04-26
// -----------------------------------------------------------------------------
void PHY_FireResults_ABC::Hit()
{
    // NOTHING
}

