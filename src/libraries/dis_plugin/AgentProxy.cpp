// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "AgentProxy.h"
#include "DetonationPDU.h"
#include "EntityStatePDU.h"
#include "IdentifierFactory_ABC.h"
#include "Time_ABC.h"
#include "UdpNetwork.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/Karma.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat_ABC.h"
#include "dispatcher/Team_ABC.h"
#include "rpr/EntityTypeResolver.h"
#include "tic/Platform_ABC.h"
#include "tic/PlatformAdapter.h"
#include "tic/PlatformDelegate_ABC.h"

using namespace plugins::dis;

// -----------------------------------------------------------------------------
// Name: AgentProxy constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
AgentProxy::AgentProxy( const Time_ABC& time, IdentifierFactory_ABC& id, const kernel::CoordinateConverter_ABC& converter, UdpNetwork& network, const rpr::EntityTypeResolver& resolver, dispatcher::Agent& holder, unsigned char exercise, bool lagAFrame, std::unique_ptr< tic::PlatformDelegate_ABC > platforms )
    : dispatcher::Observer< sword::UnitAttributes >( holder )
    , time_     ( time )
    , id_       ( id )
    , converter_( converter )
    , network_  ( network )
    , resolver_ ( resolver )
    , holder_   ( holder )
    , forceId_  ( rpr::Other )
    , exercise_ ( exercise )
    , lagAFrame_( lagAFrame )
    , adapted_  ( new tic::PlatformAdapter( holder_, converter ) )
    , platforms_( std::move( platforms ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
AgentProxy::~AgentProxy()
{
    // NOTHING
}

namespace
{
    rpr::ForceIdentifier GetForce( const dispatcher::Agent_ABC& agent )
    {
        const kernel::Karma& karma = agent.GetSuperior().GetTeam().GetKarma();
        if( karma == kernel::Karma::friend_ )
            return rpr::Friendly;
        if( karma == kernel::Karma::enemy_ )
            return rpr::Opposing;
        if( karma == kernel::Karma::neutral_ )
            return rpr::Neutral;
        return rpr::Other;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitAttributes& )
{
    forceId_ = GetForce( holder_ ); // $$$$ SBO 2009-12-11: doesn't need to be computed every time
    if( platforms_.get() )
        platforms_->Accept( *this );
    else
        AddPlatform( *adapted_ );
}

namespace
{
    std::string MakeName( const dispatcher::Agent& entity, const tic::Platform_ABC& platform )
    {
        std::stringstream ss;
        ss << entity.GetId() << "/" << platform.GetType().GetName();
        return ss.str();
    }
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::AddPlatform
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
void AgentProxy::AddPlatform( const tic::Platform_ABC& platform )
{
    T_Identifiers::iterator it = ids_.find( &platform );
    if( it == ids_.end() )
        it = ids_.insert( std::make_pair( &platform, id_.CreateNewIdentifier() ) ).first;

    EntityStatePDU pdu( time_.GetTime(), exercise_, it->second );
    pdu.SetForceId   ( forceId_ );
    pdu.SetEntityName( MakeName( holder_, platform ) );
    rpr::EntityType entityType;
    resolver_.Find( platform.GetType().GetName(), entityType );
    pdu.SetEntityType( entityType );
    switch( platform.GetState() )
    {
    default:
    case tic::Platform_ABC::okay:       pdu.SetAppearance( 0, false, false ); break;
    case tic::Platform_ABC::broken:     pdu.SetAppearance( 1, false, true ); break;
    case tic::Platform_ABC::destroyed:  pdu.SetAppearance( 3, true, true ); break;
    }
    const geometry::Point2f position( lagAFrame_ ? platform.GetLastPosition() : platform.GetPosition() );
    const geometry::Point2d geocoord = converter_.ConvertToGeo( position );
    pdu.SetPosition( geocoord.Y(), geocoord.X(), platform.GetAltitude(), platform.GetSpeed(), platform.GetHeading() );

    network_.Send( pdu );
}
