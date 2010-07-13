// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaPlugin.h"
#include "FederateFacade.h"
#include "ExtensionFactory.h"
#include "AggregateEntityClass.h"
#include "dispatcher/Config.h"
#include "dispatcher/Model.h"
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    unsigned ReadTimeStep( const std::string& session )
    {
        xml::xifstream xis( session );
        unsigned step;
        xis >> xml::start( "session" ) >> xml::start( "config" )
                >> xml::start( "simulation" ) >> xml::start( "time" )
                    >> xml::attribute( "step", step );
        return step;
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::HlaPlugin( dispatcher::Model& model, const dispatcher::Config& config, xml::xistream& xis )
    : model_     ( model )
    , agentClass_( new AggregateEntityClass() )
    , factory_   ( new ExtensionFactory( *agentClass_ ) )
    , federate_  ( new FederateFacade( xis.attribute< std::string >( "name" ), ReadTimeStep( config.GetSessionFile() ) ) )
{
    model_.RegisterFactory( *factory_ );
    federate_->AddClass( *agentClass_ );
    federate_->Join( xis.attribute< std::string >( "federation" ) );
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::~HlaPlugin()
{
    model_.UnregisterFactory( *factory_ );
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::Receive
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::Receive( const MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_end_tick() )
        federate_->Step();
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::NotifyClientAuthenticated
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC&, dispatcher::Profile_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::NotifyClientLeft
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::NotifyClientLeft( dispatcher::ClientPublisher_ABC& )
{
    // NOTHING
}
