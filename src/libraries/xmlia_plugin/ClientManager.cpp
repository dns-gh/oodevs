// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "ClientManager.h"
#include "dispatcher/ClientPublisher_ABC.h"

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: ClientManager constructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
ClientManager::ClientManager()
: clientProfile_( 0 )
, clientPublisher_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientManager destructor
// Created: SLG 2008-06-09
// -----------------------------------------------------------------------------
ClientManager::~ClientManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientManager::NotifyClientAuthenticated
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::NotifyClient( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    SetClientPublisher( client );
    SetClientProfile( profile );
    SetClientParameters( client, profile );
}

// -----------------------------------------------------------------------------
// Name: ClientManager::NotifyClientLeft
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{
    clientParameters_.erase( client.GetEndpoint() );
}

// -----------------------------------------------------------------------------
// Name: ClientManager::SetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::SetClientPublisher( dispatcher::ClientPublisher_ABC& publisher )
{
    clientPublisher_ = &publisher;
}

// -----------------------------------------------------------------------------
// Name: ClientManager::SetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::SetClientProfile( dispatcher::Profile_ABC& profile )
{
    clientProfile_ = &profile;
}

// -----------------------------------------------------------------------------
// Name: ClientManager::SetClientParameters
// Created: SLG 2009-06-12
// -----------------------------------------------------------------------------
void ClientManager::SetClientParameters( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{
    clientAttributes attributes;
    attributes.clientPublisher = &client;
    attributes.clientProfile = &profile;
    clientParameters_.insert( std::pair< std::string, clientAttributes >( client.GetEndpoint(), attributes ) );
}

// -----------------------------------------------------------------------------
// Name: ClientManager::GetClientProfile
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::Profile_ABC* ClientManager::GetClientProfile() const
{
    return clientProfile_;
}


// -----------------------------------------------------------------------------
// Name: ClientManager::GetClientPublisher
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
dispatcher::ClientPublisher_ABC* ClientManager::GetClientPublisher() const
{
    return clientPublisher_;
}
