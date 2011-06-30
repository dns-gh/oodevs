// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Network/NET_RolePion_Dotations.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 17 $
// $Workfile: NET_RolePion_Dotations.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_RolePion_Dotations.h"
#include "NetworkUnitAttributesMessageSender_ABC.h"
#include "NetworkMessageSender_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( network::NET_RolePion_Dotations )

namespace network
{

template< typename Archive >
void save_construct_data( Archive& archive, const NET_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    const MIL_AgentPion* pPion = &role->pion_;
    archive << pPion;
}

template< typename Archive >
void load_construct_data( Archive& archive, NET_RolePion_Dotations* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )NET_RolePion_Dotations( *pion );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::NET_RolePion_Dotations( MIL_AgentPion& pion )
    : pion_                   ( pion )
    , bLastStateDead_         ( false )
    , bLastStateNeutralized_  ( false )
    , bExternalMustUpdateData_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::~NET_RolePion_Dotations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void NET_RolePion_Dotations::serialize( Archive& file, const unsigned int )
{
    file & boost::serialization::base_object< NET_RoleInterface_Dotations >( *this )
         & bLastStateDead_
         & bLastStateNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendMsgUnitAttributes
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendMsgUnitAttributes( client::UnitAttributes& asnMsg, unsigned int context ) const
{
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );

    if( asnMsg().has_reinforcements()  && asnMsg().reinforcements().elem_size() > 0 )
        asnMsg().mutable_reinforcements()->Clear();

    if( asnMsg().has_contamination_agents()  && asnMsg().contamination_agents().elem_size() > 0 )
        asnMsg().mutable_contamination_agents()->Clear();

    if( asnMsg().has_transported_units()  && asnMsg().transported_units().elem_size() > 0 )
        asnMsg().mutable_transported_units()->Clear();

    if( asnMsg().has_resource_dotations()  && asnMsg().resource_dotations().elem_size() > 0 )
        asnMsg().mutable_resource_dotations()->Clear();

    if( asnMsg().has_equipment_dotations()  && asnMsg().equipment_dotations().elem_size() > 0 )
        asnMsg().mutable_equipment_dotations()->Clear();

    if( asnMsg().has_human_dotations()  && asnMsg().human_dotations().elem_size() > 0 )
        asnMsg().mutable_human_dotations()->Clear();

    if( asnMsg().has_lent_equipments()  && asnMsg().lent_equipments().elem_size() > 0 )
        asnMsg().mutable_lent_equipments()->Clear();

    if( asnMsg().has_borrowed_equipments()  && asnMsg().borrowed_equipments().elem_size() > 0 )
        asnMsg().mutable_borrowed_equipments()->Clear();
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendChangedState( unsigned int context ) const
{
    // UnitAttributes message
    if( bExternalMustUpdateData_
        || pion_.IsDead()        != bLastStateDead_
        || pion_.IsNeutralized() != bLastStateNeutralized_ )
    {
        client::UnitAttributes msg;
        msg().mutable_unit()->set_id( pion_.GetID() );
        bool bIsDead = pion_.IsDead();
        if( bLastStateDead_ != bIsDead )
        {
            msg().set_dead( bIsDead );
            bLastStateDead_ = bIsDead;
        }
        bool bIsNeutralized = pion_.IsNeutralized();
        if( bLastStateNeutralized_ != bIsNeutralized )
        {
            msg().set_neutralized( bIsNeutralized );
            bLastStateNeutralized_              = bIsNeutralized;
        }
        pion_.Apply( &network::NetworkUnitAttributesMessageSender_ABC::SendChangedState, msg );
        SendMsgUnitAttributes( msg, context );
    }

    // 'standalones' messages
    pion_.Apply( &network::NetworkMessageSender_ABC::SendChangedState );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendFullState( unsigned int context ) const
{
    // UnitAttributes
    client::UnitAttributes msg;
    msg().mutable_unit()->set_id( pion_.GetID() );
    msg().set_dead( bLastStateDead_ = pion_.IsDead() );
    msg().set_neutralized( bLastStateNeutralized_ = pion_.IsNeutralized() );
    pion_.Apply( &network::NetworkUnitAttributesMessageSender_ABC::SendFullState, msg );
    SendMsgUnitAttributes( msg, context );

    // 'standalones' messages
    pion_.Apply( &network::NetworkMessageSender_ABC::SendFullState, context );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::Clean
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::Clean()
{
    bExternalMustUpdateData_ = false;// Reinitialize for next step
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::NotifyComponentHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::NotifyDataHasChanged()
{
    bExternalMustUpdateData_ = true;
}

} // namespace network
