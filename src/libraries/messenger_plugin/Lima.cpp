// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "messenger_plugin_pch.h"
#include "Lima.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/MessengerSenders.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::messenger;

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::Lima( unsigned int id, const sword::PhaseLineCreationRequest& message )
    : TacticalLine_ABC( id, message.tactical_line() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima constructor
// Created: RDS 2008-04-03
// -----------------------------------------------------------------------------
Lima::Lima( unsigned int id, xml::xistream& xis, const sword::Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter )
    : TacticalLine_ABC( id, xis, diffusion, converter)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima destructor
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
Lima::~Lima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lima::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::Update( const sword::PhaseLineUpdateRequest& message )
{
    TacticalLine_ABC::Update( message.tactical_line() );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendCreation
// Created: NLD 2006-11-17
// -----------------------------------------------------------------------------
void Lima::SendCreation( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::PhaseLineCreation creation ;
    creation.mutable_id()->set_id( GetID() );
    TacticalLine_ABC::Send( *creation.mutable_tactical_line() );
    plugins::messenger::PhaseLineCreation message;
    message() = creation;
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendUpdate
// Created: RDS 2008-04-07
// -----------------------------------------------------------------------------
void Lima::SendUpdate( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::PhaseLineUpdate update;
    update.mutable_id()->set_id( GetID() );
    TacticalLine_ABC::Send( *update.mutable_tactical_line() );
    plugins::messenger::PhaseLineUpdate message( update );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Lima::SendDestruction( dispatcher::ClientPublisher_ABC& client ) const
{
    sword::PhaseLineDestruction destruction;
    destruction.mutable_id()->set_id( GetID() );
    plugins::messenger::PhaseLineDestruction message( destruction );
    message.Send( client );
}

// -----------------------------------------------------------------------------
// Name: Lima::SendFullUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void Lima::SendFullState( dispatcher::ClientPublisher_ABC& client ) const
{
    SendCreation( client );
}

// -----------------------------------------------------------------------------
// Name: Lima::Write
// Created: RDS 2008-04-09
// -----------------------------------------------------------------------------
void Lima::Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const
{
    xos << xml::start( "lima" ) ;
        TacticalLine_ABC::Write( xos, converter );
    xos << xml::end;
}
