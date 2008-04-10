// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gearth_plugin_pch.h"
#include "GearthPlugin.h"
#include "ConnectorFacade.h"
#include "dispatcher/Config.h"
#include "game_asn/Messenger.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace gearth;

// -----------------------------------------------------------------------------
// Name: GearthPlugin constructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
GearthPlugin::GearthPlugin( dispatcher::Model& model, tools::MessageDispatcher_ABC& dispatcher, const dispatcher::Config& config, xml::xistream& xis)
    : facade_ ( new ConnectorFacade( config, model, xis ) )
{
    dispatcher.RegisterMessage( *this, &GearthPlugin::OnReceiveMessengerToClient );
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: GearthPlugin destructor
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
GearthPlugin::~GearthPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GearthPlugin::Receive
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void GearthPlugin::Receive( const ASN1T_MsgsSimToClient& asnMsg )
{
    facade_->Receive( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: GearthPlugin::OnReceiveMessengerToClient
// Created: RDS 2008-04-10
// -----------------------------------------------------------------------------
void GearthPlugin::OnReceiveMessengerToClient(const std::string& str, const ASN1T_MsgsMessengerToClient& asnMsg)
{
    facade_->OnReceiveMessengerToClient(str, asnMsg);
}


// -----------------------------------------------------------------------------
// Name: GearthPlugin::NotifyClientAuthenticated
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void GearthPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectorFacade::NotifyClientLeft
// Created: JCR 2007-08-29
// -----------------------------------------------------------------------------
void GearthPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}