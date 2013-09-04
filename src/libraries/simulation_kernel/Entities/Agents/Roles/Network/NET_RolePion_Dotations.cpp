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

using namespace network;

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations constructor
// Created: LDC 2013-01-09
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::NET_RolePion_Dotations()
    : pion_                   ( 0 )
    , bLastStateDead_         ( false )
    , bLastStateNeutralized_  ( false )
    , bExternalMustUpdateData_( false )
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations constructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
NET_RolePion_Dotations::NET_RolePion_Dotations( MIL_AgentPion& pion )
    : pion_                   ( &pion )
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
    file & boost::serialization::base_object< NET_RoleInterface_Dotations >( *this );
    file & pion_;
    file & bLastStateDead_;
    file & bLastStateNeutralized_;
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendMsgUnitAttributes
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendMsgUnitAttributes( client::UnitAttributes& asnMsg, unsigned int context ) const
{
    asnMsg.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendChangedState( unsigned int context ) const
{
    if( pion_->IsMarkedForDestruction() )
        return;

    // UnitAttributes message
    if( bExternalMustUpdateData_
        || pion_->IsDead()        != bLastStateDead_
        || pion_->IsNeutralized() != bLastStateNeutralized_ )
    {
        client::UnitAttributes msg;
        msg().mutable_unit()->set_id( pion_->GetID() );
        bool bIsDead = pion_->IsDead();
        if( bLastStateDead_ != bIsDead )
        {
            msg().set_dead( bIsDead );
            bLastStateDead_ = bIsDead;
        }
        bool bIsNeutralized = pion_->IsNeutralized();
        if( bLastStateNeutralized_ != bIsNeutralized )
        {
            msg().set_neutralized( bIsNeutralized );
            bLastStateNeutralized_ = bIsNeutralized;
        }
        pion_->Apply( &network::NetworkUnitAttributesMessageSender_ABC::SendChangedState, msg );
        SendMsgUnitAttributes( msg, context );
    }

    // 'standalones' messages
    pion_->Apply( &network::NetworkMessageSender_ABC::SendChangedState );
}

// -----------------------------------------------------------------------------
// Name: NET_RolePion_Dotations::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::SendFullState( unsigned int context ) const
{
    if( pion_->IsMarkedForDestruction() )
        return;

    // UnitAttributes
    client::UnitAttributes msg;
    msg().mutable_unit()->set_id( pion_->GetID() );
    msg().set_dead( bLastStateDead_ = pion_->IsDead() );
    msg().set_neutralized( bLastStateNeutralized_ = pion_->IsNeutralized() );
    pion_->Apply( &network::NetworkUnitAttributesMessageSender_ABC::SendFullState, msg );
    SendMsgUnitAttributes( msg, context );

    // 'standalones' messages
    pion_->Apply( &network::NetworkMessageSender_ABC::SendFullState, context );
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
// Name: NET_RolePion_Dotations::NotifyDataHasChanged
// Created: MGD 2009-09-29
// -----------------------------------------------------------------------------
void NET_RolePion_Dotations::NotifyDataHasChanged()
{
    bExternalMustUpdateData_ = true;
}
