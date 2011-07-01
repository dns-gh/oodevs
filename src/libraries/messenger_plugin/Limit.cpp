// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Limit.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::Limit( unsigned int id, const sword::LimitCreationRequest& message )
    : TacticalLine_ABC( id, message.tactical_line() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit constructor
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
Limit::Limit( unsigned int id, xml::xistream& xis, const sword::Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( id, xis, diffusion, converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Limit::~Limit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Limit::Update
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Limit::Update( const sword::LimitUpdateRequest& message )
{
    TacticalLine_ABC::Update( message.tactical_line() );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendCreation
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Limit::SendCreation( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::LimitCreation creation ;
    creation.mutable_id()->set_id( GetID() );
    TacticalLine_ABC::Send( *creation.mutable_tactical_line() );
    plugins::messenger::LimitCreation message( creation );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendUpdate
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void Limit::SendUpdate( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::LimitUpdate update;
    update.mutable_id()->set_id( GetID() );
    TacticalLine_ABC::Send( *update.mutable_tactical_line() );
    plugins::messenger::LimitUpdate message( update );
    message.Send( client );
}


// -----------------------------------------------------------------------------
// Name: Limit::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Limit::SendDestruction( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::LimitDestruction destruction;
    destruction.mutable_id()->set_id( GetID() );
    plugins::messenger::LimitDestruction message( destruction );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: Limit::SendFullState
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void Limit::SendFullState( dispatcher::ClientPublisher_ABC& client ) const
{
    SendCreation( client );
}

// -----------------------------------------------------------------------------
// Name: Limit::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void Limit::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::start( "limit" ) ;
        TacticalLine_ABC::Write( xos, converter );
    xos << xml::end;
}
