// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathematiques Appliquees SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Plugin_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/Replay.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

UnicastPublisher::UnicastPublisher( ClientPublisher_ABC& publisher,
    const std::string& link, int32_t clientId, int32_t context )
    : publisher_( publisher )
    , link_( link )
    , clientId_( clientId )
    , context_( context )
{
}

const std::string& UnicastPublisher::GetLink() const
{
    return link_;
}

void UnicastPublisher::Send( sword::ReplayToClient& message )
{
    message.set_client_id( clientId_ );
    message.set_context( context_ );
    publisher_.Send( message );
}

void UnicastPublisher::Send( sword::SimToClient& message )
{
    message.set_client_id( clientId_ );
    message.set_context( context_ );
    publisher_.Send( message );
}

