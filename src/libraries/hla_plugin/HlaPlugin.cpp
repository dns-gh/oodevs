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
#include "ObjectResolver.h"
#include "Stepper.h"
#include "ContextFactory.h"
#include "UnitTypeResolver.h"
#include "MunitionTypeResolver.h"
#include "LocalAgentResolver.h"
#include "RemoteAgentResolver.h"
#include "SimulationFacade.h"
#include "InteractionsFacade.h"
#include "ComponentTypes.h"
#include "TransportationFacade.h"
#include "CallsignResolver.h"
#include "Subordinates.h"
#include "MissionResolver.h"
#include "SideChecker.h"
#include "Transporters.h"
#include "tools/MessageController.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "dispatcher/Config.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Model_ABC.h"
#include "protocol/Simulation.h"
#include "rpr/EntityTypeResolver.h"
#include <hla/HLAException.h>
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
    std::string ReadMapping( xml::xisubstream xis, const std::string& mapping )
    {
        xis >> xml::start( "mappings" );
        return xis.content< std::string >( mapping );
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::HlaPlugin( dispatcher::Model_ABC& dynamicModel, const dispatcher::StaticModel& staticModel,
                      dispatcher::SimulationPublisher_ABC& publisher, const dispatcher::Config& config,
                      xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : dynamicModel_               ( dynamicModel )
    , staticModel_                ( staticModel )
    , publisher_                  ( publisher )
    , config_                     ( config )
    , pXisSession_                ( new xml::xibufferstream( xis ) )
    , pXisConfiguration_          ( new xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + xis.attribute< std::string >( "configuration", "configuration.xml" ) ) )
    , pXis_                       ( new xml::ximultistream( *pXisSession_, *pXisConfiguration_ >> xml::start( "configuration" ) ) )
    , logger_                     ( logger )
    , pContextFactory_            ( new ContextFactory() )
    , pObjectResolver_            ( new ObjectResolver() )
    , pRtiFactory_                ( new RtiAmbassadorFactory( *pXis_, xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/protocols.xml" ) ) )
    , pDebugRtiFactory_           ( new DebugRtiAmbassadorFactory( *pRtiFactory_, logger, *pObjectResolver_ ) )
    , pFederateFactory_           ( new FederateAmbassadorFactory( ReadTimeStep( config.GetSessionFile() ) ) )
    , pDebugFederateFactory_      ( new DebugFederateAmbassadorFactory( *pFederateFactory_, logger, *pObjectResolver_ ) )
    , pAggregateTypeResolver_     ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "aggregate" ) ) ) )
    , pSurfaceVesselTypeResolver_ ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "surface-vessel" ) ) ) )
    , pComponentTypeResolver_     ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "component" ) ) ) )
    , pEntityMunitionTypeResolver_( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "munition" ) ) ) )
    , pComponentTypes_            ( new ComponentTypes( staticModel.types_ ) )
    , pUnitTypeResolver_          ( new UnitTypeResolver( *pAggregateTypeResolver_, staticModel.types_ ) )
    , pMunitionTypeResolver_      ( new MunitionTypeResolver( *pEntityMunitionTypeResolver_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , pLocalAgentResolver_        ( new LocalAgentResolver() )
    , pCallsignResolver_          ( new CallsignResolver() )
    , pMissionResolver_           ( new MissionResolver( staticModel.types_ ) )
    , pSubordinates_              ( new Subordinates( *pCallsignResolver_, dynamicModel.Automats() ) )
    , pMessageController_         ( new tools::MessageController< sword::SimToClient_Content >() )
    , pSubject_                   ( 0 )
    , pFederate_                  ( 0 )
    , pSimulationFacade_          ( 0 )
    , pRemoteAgentResolver_       ( 0 )
    , pInteractionsFacade_        ( 0 )
    , pSideChecker_               ( 0 )
    , pTransporters_              ( 0 )
    , pTransportationFacade_      ( 0 )
    , pStepper_                   ( 0 )
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
void HlaPlugin::Receive( const sword::SimToClient& message )
{
    try
    {
        pMessageController_->Dispatch( message.message(), message.has_context() ? message.context() : -1 );
        if( message.message().has_control_end_tick() && !pSimulationFacade_.get() )
        {
            pSubject_.reset( new AgentController( *pXis_, dynamicModel_, *pAggregateTypeResolver_, *pSurfaceVesselTypeResolver_, *pComponentTypeResolver_, *pComponentTypes_ ) );
            pFederate_.reset( new FederateFacade( *pXis_, *pMessageController_, *pSubject_, *pLocalAgentResolver_,
                                                  pXis_->attribute< bool >( "debug", false ) ? *pDebugRtiFactory_ : *pRtiFactory_,
                                                  pXis_->attribute< bool >( "debug", false ) ? *pDebugFederateFactory_ : *pFederateFactory_,
                                                  config_.BuildPluginDirectory( "hla" ), *pCallsignResolver_ ) );
            pSimulationFacade_.reset( new SimulationFacade( *pXis_, *pContextFactory_, *pMessageController_, publisher_, dynamicModel_, *pComponentTypeResolver_, staticModel_, *pUnitTypeResolver_, *pFederate_, *pComponentTypes_, *pCallsignResolver_ ) );
            pRemoteAgentResolver_.reset( new RemoteAgentResolver( *pFederate_, *pSimulationFacade_ ) );
            pInteractionsFacade_.reset( new InteractionsFacade( *pFederate_, publisher_, *pMessageController_, *pRemoteAgentResolver_, *pLocalAgentResolver_, *pContextFactory_, *pMunitionTypeResolver_, *pFederate_, pXis_->attribute< std::string >( "name", "SWORD" ) ) );
            pSideChecker_.reset( new SideChecker( *pSubject_, *pFederate_, *pRemoteAgentResolver_ ) );
            pTransporters_.reset( new Transporters( *pSubject_, *pCallsignResolver_, *pSideChecker_ ) );
            pTransportationFacade_.reset( pXis_->attribute< bool >( "netn", true ) ? new TransportationFacade( *pXis_, *pMissionResolver_, *pMessageController_, *pCallsignResolver_, *pSubordinates_, *pFederate_, *pContextFactory_, *pTransporters_ ) : 0 );
            pStepper_.reset( new Stepper( *pXis_, *pMessageController_, publisher_ ) );
            pSubject_->Visit( dynamicModel_ );
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
