// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "BmlPlugin.h"
#include "ExtensionFactory.h"
#include "Publisher.h"
#include "UpdateListener.h"
#include "dispatcher/Model.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: BmlPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::BmlPlugin( dispatcher::Model& model, xml::xistream& xis, dispatcher::SimulationPublisher_ABC& simulation )
    : model_( model )
    , publisher_( new Publisher( xis ) )
    , factory_( new ExtensionFactory( *publisher_ ) )
    , listener_( new UpdateListener( *publisher_, model_, simulation ) )
{
    model_.RegisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin destructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::~BmlPlugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::Receive
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_begin_tick )
        listener_->Update( *message.msg.u.msg_control_begin_tick );
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, dispatcher::Profile_ABC& /*profile*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/ )
{
    // NOTHING
}
