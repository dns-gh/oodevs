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
#include "dispatcher/Model.h"

using namespace bml;

// -----------------------------------------------------------------------------
// Name: BmlPlugin constructor
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
BmlPlugin::BmlPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis )
    : model_( model )
    , publisher_( new Publisher( xis ) )
    , factory_( new ExtensionFactory( *publisher_ ) )
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

}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& client, dispatcher::Profile_ABC& profile )
{

}

// -----------------------------------------------------------------------------
// Name: BmlPlugin::NotifyClientLeft
// Created: SBO 2008-02-29
// -----------------------------------------------------------------------------
void BmlPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& client )
{

}
