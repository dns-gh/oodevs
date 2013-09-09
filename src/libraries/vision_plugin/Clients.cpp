// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Clients.h"
#include "protocol/ClientPublisher_ABC.h"

using namespace plugins::vision;

void Clients::Register( dispatcher::ClientPublisher_ABC& publisher )
{
    clients_.insert( &publisher );
}

void Clients::Unregister( dispatcher::ClientPublisher_ABC& publisher )
{
    clients_.erase( &publisher );
}

void Clients::Send( const sword::SimToClient& message )
{
    for( auto it = clients_.begin(); it != clients_.end(); ++it )
        (*it)->Send( message );
}

bool Clients::IsRegistered( dispatcher::ClientPublisher_ABC& publisher ) const
{
    return clients_.find( &publisher ) != clients_.end();
}

bool Clients::IsRegistered() const
{
    return ! clients_.empty();
}
