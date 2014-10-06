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
#include "DotationTypeResolver.h"
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
#include "NETNv1_InteractionBuilder.h"
#include "NETNv2_InteractionBuilder.h"
#include "InteractionBuilder.h"
#include "ExtentResolver_ABC.h"
#include "TacticalObjectController.h"
#include "OwnershipStrategy.h"
#include "OwnershipController.h"
#include "LocationOwnershipPolicy.h"
#include "RprTransferSender.h"
#include "Netn2TransferSender.h"
#include "NullTransferSender.h"
#include "RprTransferSender.h"
#include "EntityIdentifierResolver.h"
#include "FOM_Serializer.h"
#include "SideResolver.h"
#include "SimulationTimeManager.h"
#include "ExternalOwnershipPolicy.h"
#include "MRMController.h"
#include "tools/FileWrapper.h"
#include "tools/MessageController.h"
#include "tools/XmlStreamOperators.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter.h"
#include "dispatcher/Config.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Model_ABC.h"
#include "protocol/Simulation.h"
#include "protocol/Messenger.h"
#include "rpr/EntityTypeResolver.h"
#include "rpr/EntityIdentifier.h"
#include "tic/PlatformDelegateFactory.h"
#include <hla/HLAException.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace plugins::hla;

namespace
{
    unsigned ReadTimeStep( const tools::Path& session )
    {
        tools::Xifstream xis( session );
        unsigned step;
        xis >> xml::start( "session" ) >> xml::start( "config" )
                >> xml::start( "simulation" ) >> xml::start( "time" )
                    >> xml::attribute( "step", step );
        return step;
    }
    tools::Path ReadMapping( xml::xisubstream xis, const std::string& mapping )
    {
        xis >> xml::start( "mappings" );
        return xis.content< tools::Path >( mapping );
    }
    std::string ReadDivestitureZone( xml::xisubstream xis )
    {
        xis >> xml::start( "drawings" );
        return xis.content< std::string >( "divesture-area" );
    }

    class LogFilter : public dispatcher::Logger_ABC
    {
    public:
        explicit LogFilter( dispatcher::Logger_ABC& logger )
            : logger_( logger )
        {}
        virtual void LogInfo( const std::string& /*message*/ ) {}
        virtual void LogWarning( const std::string& message ) 
        { 
            logger_.LogWarning( message );
        }
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
        ExtentResolver( xml::xisubstream xis, dispatcher::Logger_ABC& logger, const kernel::CoordinateConverter_ABC& converter )
            : logger_   ( logger )
            , useExtent_( xis.attribute< bool >( "extent", false ) )
            , converter_( converter )
        {}
        virtual bool IsInBoundaries( const geometry::Point2d& geoPoint ) const
        {
            if( !useExtent_ )
                return true;
            geometry::Point2f xyPoint;
            try
            {
                xyPoint = converter_.ConvertFromGeo( geoPoint );
            }
            catch( const std::exception& e )
            {
                logger_.LogError( "Failed to convert point "
                                  "Latitude '"  + boost::lexical_cast< std::string >( geoPoint.X() ) + "' "
                                  "Longitude '" + boost::lexical_cast< std::string >( geoPoint.Y() ) + "' "
                                  ": " + tools::GetExceptionMsg( e ) );
                return false;
            }
            return converter_.IsInBoundaries( xyPoint );
        }
    private:
        dispatcher::Logger_ABC& logger_;
        const bool useExtent_;
        const kernel::CoordinateConverter_ABC& converter_;
    };
    TransferSender_ABC* CreateTransferSender( xml::xisubstream xis, const rpr::EntityIdentifier& federateID, const std::string& federateName,
            const ::hla::FederateIdentifier& fedHandle, const ContextFactory_ABC& ctxtFactory, const InteractionBuilder& builder, OwnershipStrategy_ABC& strategy,
            OwnershipController_ABC& controller,  dispatcher::Logger_ABC& logger,
            const LocalAgentResolver_ABC& agentResolver, const CallsignResolver_ABC& callsignResolver )
    {
        const bool useNetn(  xis.attribute< bool >( "netn", false ) );
        TransferSender_ABC* transferResult = NULL;
        if( !useNetn )
        {
            try
            {
                transferResult = new RprTransferSender( federateID, ctxtFactory, builder, strategy, controller, logger );
            }
            catch( const ::hla::HLAException& e )
            {
                logger.LogError( "Unable to create RPR transfer sender with error: " + tools::GetExceptionMsg( e ) + ".\n  Creating NullTransferSender instead.  Ownership transfer will be disabled." );
            }
        }
        else
        {
            try
            {
                transferResult = new Netn2TransferSender( federateName, fedHandle, ctxtFactory, builder, strategy, controller, logger, agentResolver, callsignResolver );
            }
            catch( const ::hla::HLAException& e )
            {
                logger.LogError( "Unable to create NETN transfer sender with error: " + tools::GetExceptionMsg( e ) + ".\n  Creating NullTransferSender instead.  Ownership transfer will be disabled." );
            }
        }

        if (transferResult == NULL)
            transferResult = new NullTransferSender( federateID, ctxtFactory, builder, strategy, controller, logger);

        return transferResult;
    }

    OwnershipPolicy_ABC* CreateOwnershipPolicy( xml::xisubstream xis, const std::string& federateName, OwnershipController_ABC& ownershipController, TransferSender_ABC& transferSender,
            dispatcher::Logger_ABC& logger, InteractionBuilder& interactionBuilder, RemoteAgentSubject_ABC& remoteSubject, const LocalAgentResolver_ABC& agentResolver,
            const CallsignResolver_ABC& callsignResolver, tools::MessageController_ABC< sword::MessengerToClient_Content >& controller, const std::string& divestZone )
    {
        const std::string policy( xis.attribute< std::string >( "ownership-policy", "internal" ) );
        if( policy == "external" )
        {
            return new ExternalOwnershipPolicy( federateName, interactionBuilder, ownershipController, logger, remoteSubject, agentResolver, callsignResolver, transferSender );
        }
        return new LocationOwnershipPolicy( controller, ownershipController, remoteSubject, transferSender, divestZone );
    }
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
    , pXisConfiguration_          ( new tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / xis.attribute< tools::Path >( "configuration", "configuration.xml" ) ) ) )
    , pXis_                       ( new xml::ximultistream( *pXisSession_, *pXisConfiguration_ >> xml::start( "configuration" ) ) )
    , logFilter_                  ( new LogFilter( logger ) )
    , logger_                     ( pXis_->attribute< bool >( "debug", false ) ? logger : *logFilter_ )
    , pContextFactory_            ( new ContextFactory() )
    , pObjectResolver_            ( new ObjectResolver() )
    , pRtiFactory_                ( new RtiAmbassadorFactory( *pXis_, tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / "protocols.xml" ) ) ) )
    , pDebugRtiFactory_           ( new DebugRtiAmbassadorFactory( *pRtiFactory_, logger_, *pObjectResolver_ ) )
    , pFederateFactory_           ( new FederateAmbassadorFactory( ReadTimeStep( config.GetSessionFile() ) ) )
    , pDebugFederateFactory_      ( new DebugFederateAmbassadorFactory( *pFederateFactory_, logger_, *pObjectResolver_ ) )
    , pAggregateTypeResolver_     ( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "aggregate" ) ) ) ) )
    , pAutomatAggregateResolver_  ( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "automat" ) ) ) ) )
    , pSurfaceVesselTypeResolver_ ( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "surface-vessel" ) ) ) ) )
    , pComponentTypeResolver_     ( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "component" ) ) ) ) )
    , pEntityMunitionTypeResolver_( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "munition" ) ) ) ) )
    , pEntityObjectTypeResolver_  ( new rpr::EntityTypeResolver( tools::Xifstream( ( config.BuildPluginDirectory( "hla" ) / ReadMapping( *pXis_, "object" ) ) ) ) )
    , pComponentTypes_            ( new ComponentTypes( staticModel.types_ ) )
    , pUnitTypeResolver_          ( new UnitTypeResolver< kernel::AgentType >( *pAggregateTypeResolver_, staticModel.types_, logger ) )
    , pAutomatTypeResolver_       ( new UnitTypeResolver< kernel::AutomatType >( *pAutomatAggregateResolver_, staticModel.types_, logger ) )
    , pMunitionTypeResolver_      ( new DotationTypeResolver( *pEntityMunitionTypeResolver_, staticModel.objectTypes_, staticModel.objectTypes_ ) )
    , pLocalAgentResolver_        ( new LocalAgentResolver() )
    , pCallsignResolver_          ( new CallsignResolver( *pXis_ ) )
    , pMissionResolver_           ( new MissionResolver( staticModel.types_, staticModel.types_ ) )
    , pSubordinates_              ( new Subordinates( *pCallsignResolver_, dynamicModel.Automats() ) )
    , pMessageController_         ( new tools::MessageController< sword::SimToClient_Content >() )
    , pAutomatChecker_            ( new AutomatChecker( dynamicModel.Agents() ) )
    , pConverter_                 ( new kernel::CoordinateConverter( config ) )
    , pExtentResolver_            ( new ExtentResolver( *pXis_, logger_, *pConverter_ ) )
    , platforms_                  ( new tic::PlatformDelegateFactory( *pConverter_, static_cast< float >( ReadTimeStep( config.GetSessionFile() ) ) ) )
    , pMessengerMessageController_( new tools::MessageController< sword::MessengerToClient_Content >() )
    , pOwnershipStrategy_         ( new OwnershipStrategy( *pXis_ ) )
    , pEntityIdentifierResolver_  ( new EntityIdentifierResolver( pXis_->attribute< unsigned short >( "dis-site", 1 ), pXis_->attribute< unsigned short >( "dis-application", 1 ) ) )
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
        rpr::EntityIdentifier federateID(pXis_->attribute< unsigned short >( "dis-site", 1 ),
                pXis_->attribute< unsigned short >( "dis-application", 1 ), 0xFFFF );
        pMessageController_->Dispatch( message.message(), message.has_context() ? message.context() : -1 );
        if( message.message().has_control_end_tick() && !pSimulationFacade_.get() )
        {

            pSimulationTimeManager_.reset( new SimulationTimeManager( *pMessageController_ ) );
            pSideResolver_.reset( new SideResolver( dynamicModel_, logger_ ) );
            pFomSerializer_.reset( new FOM_Serializer( pXis_->attribute< int >( "netn-version", 1 ) ) );
            pSubject_.reset( new AgentController( dynamicModel_, *pAggregateTypeResolver_, *pComponentTypeResolver_, *pComponentTypes_,
                                                    *platforms_, *pConverter_, pXis_->attribute< bool >( "disaggregate", false ), 
                                                    *pSideResolver_, *pLocalAgentResolver_, pXis_->attribute< bool >( "send-full-orbat", false ), logger_, *pAggregateTypeResolver_, pXis_->attribute< int >( "netn-version", 1 ) ) );
            pTacticalObjectSubject_.reset( new TacticalObjectController ( dynamicModel_, *pConverter_, *pEntityObjectTypeResolver_, *pEntityMunitionTypeResolver_, logger_, *pSimulationTimeManager_ ) );
            pFederate_.reset( new FederateFacade( *pXis_, *pMessageController_, *pSubject_, *pLocalAgentResolver_,
                                                  pXis_->attribute< bool >( "debug", false ) ? *pDebugRtiFactory_ : *pRtiFactory_,
                                                  pXis_->attribute< bool >( "debug", false ) ? *pDebugFederateFactory_ : *pFederateFactory_,
                                                  config_.BuildPluginDirectory( "hla" ), *pCallsignResolver_, *pTacticalObjectSubject_,
                                                  *pOwnershipStrategy_, *pEntityIdentifierResolver_, *pFomSerializer_, logger_ ) );
            pNetnInteractionBuilder_.reset( pXis_->attribute< int >( "netn-version", 1 ) == 1 ?
                    static_cast< NETN_InteractionBuilder_ABC* >( new NETNv1_InteractionBuilder( logger_, *pFederate_ ) ):
                    static_cast< NETN_InteractionBuilder_ABC* >( new NETNv2_InteractionBuilder( logger_, *pFederate_ ) ) );
            pInteractionBuilder_.reset( new InteractionBuilder( logger_, *pFederate_, *pNetnInteractionBuilder_ ) );
            pSimulationFacade_.reset( new SimulationFacade( *pXis_, *pContextFactory_, *pMessageController_, simulationPublisher_, dynamicModel_,
                    *pComponentTypeResolver_, *pUnitTypeResolver_, *pAutomatTypeResolver_, *pFederate_, *pComponentTypes_, *pCallsignResolver_, logger_,
                    *pExtentResolver_, *pSubject_, *pLocalAgentResolver_, *pSideResolver_, *pEntityObjectTypeResolver_, *pFederate_, *pMissionResolver_, config_, *pSimulationTimeManager_ ) );
            pRemoteAgentResolver_.reset( new RemoteAgentResolver( *pFederate_, *pSimulationFacade_ ) );
            pDetonationFacade_.reset( new DetonationFacade( simulationPublisher_, *pMessageController_, *pRemoteAgentResolver_, *pLocalAgentResolver_, *pContextFactory_, *pMunitionTypeResolver_, *pFederate_, pXis_->attribute< std::string >( "name", "SWORD" ), *pInteractionBuilder_, *pSubject_ ) );
            pSideChecker_.reset( new SideChecker( *pSubject_, *pFederate_, *pRemoteAgentResolver_ ) );
            pTransportationFacade_.reset( pXis_->attribute< bool >( "netn", true ) ? new TransportationFacade( logger_, *pXis_, *pMissionResolver_, *pMessageController_, *pCallsignResolver_, *pSubordinates_, *pInteractionBuilder_, *pContextFactory_, simulationPublisher_, clientsPublisher_ ) : 0 );
            pStepper_.reset( new Stepper( *pXis_, *pMessageController_, simulationPublisher_ ) );
            pOwnershipController_.reset( new OwnershipController( federateID, *pFederate_, logger_ ) );
            transferSender_.reset( CreateTransferSender( *pXis_, federateID, pXis_->attribute< std::string >( "name", "SWORD" ), pFederate_->GetFederateHandle(),
                    *pContextFactory_, *pInteractionBuilder_, *pOwnershipStrategy_, *pOwnershipController_, logger_, *pLocalAgentResolver_, *pCallsignResolver_ ) );
            pOwnershipPolicy_.reset( CreateOwnershipPolicy(  *pXis_, pXis_->attribute< std::string >( "name", "SWORD" ), *pOwnershipController_, *transferSender_ ,
                    logger_, *pInteractionBuilder_, *pFederate_, *pLocalAgentResolver_, *pCallsignResolver_, *pMessengerMessageController_, ReadDivestitureZone( *pXisConfiguration_ ) ) ),
            pMrmController.reset( new MRMController( pXis_->attribute< std::string >( "name", "SWORD" ), *pInteractionBuilder_,  *pFederate_, logger_,
                    *pLocalAgentResolver_, *pCallsignResolver_ ) );
            // must be last action
            pSubject_->Visit( dynamicModel_ );
            pTacticalObjectSubject_->Visit( dynamicModel_ );
        }
    }
    catch( ::hla::HLAException& e )
    {
        logger_.LogError( "Step failed cause hla exception: " + tools::GetExceptionMsg( e ) );
    }
    catch( const std::exception& e )
    {
        logger_.LogError( "Step failed cause: " + tools::GetExceptionMsg( e ) );
    }
    catch( ... )
    {
        logger_.LogError( "Step failed (unhandled error)." );
    }
}

// -----------------------------------------------------------------------------
// Name: HlaPlugin::Receive
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void HlaPlugin::Receive( const sword::MessengerToClient& message )
{
    pMessengerMessageController_->Dispatch( message.message(), message.has_context() ? message.context() : -1 );
}
