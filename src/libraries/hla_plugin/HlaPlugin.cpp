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
#include "DetonationFacade.h"
#include "ComponentTypes.h"
#include "TransportationFacade.h"
#include "CallsignResolver.h"
#include "Subordinates.h"
#include "MissionResolver.h"
#include "SideChecker.h"
#include "AutomatChecker.h"
#include "Transporters.h"
#include "InteractionBuilder.h"
#include "ExtentResolver_ABC.h"
#include "tools/MessageController.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Model_ABC.h"
#include "protocol/Simulation.h"
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
    std::string ReadMapping( xml::xisubstream xis, const std::string& mapping )
    {
        xis >> xml::start( "mappings" );
        return xis.content< std::string >( mapping );
    }
    class LogFilter : public dispatcher::Logger_ABC
    {
    public:
        explicit LogFilter( dispatcher::Logger_ABC& logger )
            : logger_( logger )
        {}
        virtual void LogInfo( const std::string& /*message*/ ) {}
        virtual void LogWarning( const std::string& /*message*/ ) {}
        virtual void LogError( const std::string& message )
        {
            logger_.LogError( message );
        }
    private:
        dispatcher::Logger_ABC& logger_;
    };
    class ExtentResolver : public ExtentResolver_ABC
    {
    public:
        ExtentResolver( xml::xisubstream xis, dispatcher::Logger_ABC& logger, const dispatcher::Config& config )
            : logger_   ( logger )
            , useExtent_( xis.attribute< bool >( "extent", false ) )
            , converter_( new kernel::CoordinateConverter( config ) )
        {}
        virtual bool IsInBoundaries( const geometry::Point2d& geoPoint ) const
        {
            if( !useExtent_ )
                return true;
            geometry::Point2f xyPoint;
            try
            {
                xyPoint = converter_->ConvertFromGeo( geoPoint );
            }
            catch( std::exception& e )
            {
                logger_.LogError( "Failed to convert point "
                    "Latitude '"  + boost::lexical_cast< std::string >( geoPoint.X() ) + "' "
                    "Longitude '" + boost::lexical_cast< std::string >( geoPoint.Y() ) + "' "
                    ": " + e.what() );
                return false;
            }
            return converter_->IsInBoundaries( xyPoint );
        }
    private:
        dispatcher::Logger_ABC& logger_;
        const bool useExtent_;
        std::auto_ptr< kernel::CoordinateConverter_ABC > converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
HlaPlugin::HlaPlugin( dispatcher::Model_ABC& dynamicModel, const dispatcher::StaticModel& staticModel,
                      dispatcher::SimulationPublisher_ABC& simulationPublisher, dispatcher::ClientPublisher_ABC& clientsPublisher,
                      const dispatcher::Config& config, xml::xistream& xis, dispatcher::Logger_ABC& logger )
    : dynamicModel_               ( dynamicModel )
    , staticModel_                ( staticModel )
    , simulationPublisher_        ( simulationPublisher )
    , clientsPublisher_           ( clientsPublisher )
    , config_                     ( config )
    , pXisSession_                ( new xml::xibufferstream( xis ) )
    , pXisConfiguration_          ( new xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + xis.attribute< std::string >( "configuration", "configuration.xml" ) ) )
    , pXis_                       ( new xml::ximultistream( *pXisSession_, *pXisConfiguration_ >> xml::start( "configuration" ) ) )
    , logFilter_                  ( new LogFilter( logger ) )
    , logger_                     ( pXis_->attribute< bool >( "debug", false ) ? logger : *logFilter_ )
    , pContextFactory_            ( new ContextFactory() )
    , pObjectResolver_            ( new ObjectResolver() )
    , pRtiFactory_                ( new RtiAmbassadorFactory( *pXis_, xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/protocols.xml" ) ) )
    , pDebugRtiFactory_           ( new DebugRtiAmbassadorFactory( *pRtiFactory_, logger_, *pObjectResolver_ ) )
    , pFederateFactory_           ( new FederateAmbassadorFactory( ReadTimeStep( config.GetSessionFile() ) ) )
    , pDebugFederateFactory_      ( new DebugFederateAmbassadorFactory( *pFederateFactory_, logger_, *pObjectResolver_ ) )
    , pAggregateTypeResolver_     ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "aggregate" ) ) ) )
    , pSurfaceVesselTypeResolver_ ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "surface-vessel" ) ) ) )
    , pComponentTypeResolver_     ( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "component" ) ) ) )
    , pEntityMunitionTypeResolver_( new rpr::EntityTypeResolver( xml::xifstream( config.BuildPluginDirectory( "hla" ) + "/" + ReadMapping( *pXis_, "munition" ) ) ) )
    , pComponentTypes_            ( new ComponentTypes( staticModel.types_ ) )
    , pUnitTypeResolver_          ( new UnitTypeResolver( *pAggregateTypeResolver_, staticModel.types_ ) )
    , pMunitionTypeResolver_      ( new MunitionTypeResolver( *pEntityMunitionTypeResolver_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , pLocalAgentResolver_        ( new LocalAgentResolver() )
    , pCallsignResolver_          ( new CallsignResolver() )
    , pMissionResolver_           ( new MissionResolver( staticModel.types_, staticModel.types_ ) )
    , pSubordinates_              ( new Subordinates( *pCallsignResolver_, dynamicModel.Automats() ) )
    , pMessageController_         ( new tools::MessageController< sword::SimToClient_Content >() )
    , pAutomatChecker_            ( new AutomatChecker( dynamicModel.Agents() ) )
    , pExtentResolver_            ( new ExtentResolver( *pXis_, logger_, config ) )
    , pSubject_                   ( 0 )
    , pFederate_                  ( 0 )
    , pInteractionBuilder_        ( 0 )
    , pSimulationFacade_          ( 0 )
    , pRemoteAgentResolver_       ( 0 )
    , pDetonationFacade_          ( 0 )
    , pSideChecker_               ( 0 )
    , pTransporters_              ( 0 )
    , pTransportationFacade_      ( 0 )
    , pStepper_                   ( 0 )
{
    logger_.LogInfo( "Debug log enabled" );
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
            pInteractionBuilder_.reset( new InteractionBuilder( logger_, *pFederate_ ) );
            pSimulationFacade_.reset( new SimulationFacade( *pXis_, *pContextFactory_, *pMessageController_, simulationPublisher_, dynamicModel_, *pComponentTypeResolver_, staticModel_, *pUnitTypeResolver_, *pFederate_, *pComponentTypes_, *pCallsignResolver_, logger_, *pExtentResolver_ ) );
            pRemoteAgentResolver_.reset( new RemoteAgentResolver( *pFederate_, *pSimulationFacade_ ) );
            pDetonationFacade_.reset( new DetonationFacade( simulationPublisher_, *pMessageController_, *pRemoteAgentResolver_, *pLocalAgentResolver_, *pContextFactory_, *pMunitionTypeResolver_, *pFederate_, pXis_->attribute< std::string >( "name", "SWORD" ), *pInteractionBuilder_ ) );
            pSideChecker_.reset( new SideChecker( *pSubject_, *pFederate_, *pRemoteAgentResolver_ ) );
            pTransporters_.reset( new Transporters( *pSubject_, *pCallsignResolver_, *pSideChecker_, *pAutomatChecker_ ) );
            pTransportationFacade_.reset( pXis_->attribute< bool >( "netn", true ) ? new TransportationFacade( *pXis_, *pMissionResolver_, *pMessageController_, *pCallsignResolver_, *pSubordinates_, *pInteractionBuilder_, *pContextFactory_, *pTransporters_, simulationPublisher_, clientsPublisher_ ) : 0 );
            pStepper_.reset( new Stepper( *pXis_, *pMessageController_, simulationPublisher_ ) );
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
