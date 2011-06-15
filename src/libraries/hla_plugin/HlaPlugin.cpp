// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "HlaPlugin.h"
#include "AgentController.h"
#include "FederateFacade.h"
#include "RtiAmbassadorFactory.h"
#include "DebugRtiAmbassadorFactory.h"
#include "FederateAmbassadorFactory.h"
#include "DebugFederateAmbassadorFactory.h"
#include "dispatcher/Config.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/Model_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#include "rpr/EntityTypeResolver.h"
#include <hla/HLAException.h>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>

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
HlaPlugin::HlaPlugin( dispatcher::Model_ABC& model, dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Config& config, xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : timeConstrained_      ( xis.attribute< bool >( "time-constrained", true ) )
    , model_                ( model )
    , logger_               ( logger )
    , publisher_            ( publisher )
    , pRtiFactory_          ( new RtiAmbassadorFactory() )
    , pDebugRtiFactory_     ( new DebugRtiAmbassadorFactory( *pRtiFactory_, logger ) )
    , pFederateFactory_     ( new FederateAmbassadorFactory( ReadTimeStep( config.GetSessionFile() ) ) )
    , pDebugFederateFactory_( new DebugFederateAmbassadorFactory( *pFederateFactory_, logger ) )
    , pResolver_            ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + xis.attribute< std::string >( "dis", "dis.xml" ) ) ) )
    , pSubject_             ( new AgentController( model, *pResolver_ ) )
    , federate_             ( new FederateFacade( xis, *pSubject_,
                                                  xis.attribute< bool >( "debug", false ) ? *pDebugRtiFactory_ : *pRtiFactory_,
                                                  xis.attribute< bool >( "debug", false ) ? *pDebugFederateFactory_ : *pFederateFactory_,
                                                  config.BuildPluginDirectory( "hla" ) ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::~HlaPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::Receive
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
void HlaPlugin::Receive( const sword::SimToClient& wrapper )
{
    try
    {
        if( wrapper.message().has_control_end_tick() )
        {
            federate_->Step();
            if( timeConstrained_ )
            {
                simulation::ControlResume message;
                message().set_tick( 1 );
                message.Send( publisher_ );
            }
        }
    }
    catch( ::hla::HLAException& e )
    {
        logger_.LogError( "Step failed cause hla exception: " + std::string( e.what() ) );
    }
    catch( std::exception& e )
    {
        logger_.LogError( "Step failed cause: " + std::string( e.what() ) );
    }
    catch( ... )
    {
        logger_.LogError( "Step failed (unhandled error)." );
    }
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
