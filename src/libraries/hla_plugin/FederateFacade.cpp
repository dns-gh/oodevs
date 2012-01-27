// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FederateFacade.h"
#include "HlaClass.h"
#include "Federate_ABC.h"
#include "FederateAmbassadorFactory_ABC.h"
#include "RtiAmbassadorFactory_ABC.h"
#include "AgentSubject_ABC.h"
#include "HlaFactories.h"
#include "ClassBuilders.h"
#include "AggregateEntity.h"
#include "NetnAggregate.h"
#include "RemoteAggregate.h"
#include "NetnRemoteAggregate.h"
#include "SurfaceVessel.h"
#include "NetnSurfaceVessel.h"
#include "RemoteSurfaceVessel.h"
#include "NetnRemoteSurfaceVessel.h"
#include "Aircraft.h"
#include "NetnAircraft.h"
#include "RemoteAircraft.h"
#include "NetnRemoteAircraft.h"
#include "MarkingFactory.h"
#include "protocol/Simulation.h"
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <xeumeuleu/xml.hpp>

using namespace plugins::hla;

namespace
{
    std::auto_ptr< Federate_ABC > CreateFederate( xml::xisubstream xis, hla::RtiAmbassador_ABC& ambassador, const FederateAmbassadorFactory_ABC& factory, const std::string& pluginDirectory )
    {
        std::auto_ptr< Federate_ABC > federate = factory.Create( ambassador, xis.attribute< std::string >( "name", "SWORD" ), xis.attribute< int >( "lookahead", -1 ) );
        if( !federate->Connect() )
            throw std::runtime_error( "Could not connect to '" + xis.attribute< std::string >( "host", "localhost" ) + ":" + xis.attribute< std::string >( "port", "8989" ) + "'" );
        const std::string name = xis.attribute< std::string >( "federation", "Federation" );
        const bool joined = federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) );
        if( !joined )
        {
            if( xis.attribute< bool >( "creation", false ) )
            {
                const std::string fom = xis.attribute< std::string >( "fom", "ASI_FOM_v2.0.8_2010.xml" );
                if( !federate->Create( name, pluginDirectory + "/" + fom ) )
                    throw std::runtime_error( "Could not create the federation '" + name + "'" );
                if( !federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) ) )
                    throw std::runtime_error( "Could not join the federation '" + name + "'" );
            }
            else
                throw std::runtime_error( "Could not join the federation '" + name + "'" );
        }
        return federate;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< HlaObjectFactory_ABC > CreateFactory( xml::xisubstream xis, CallsignResolver_ABC& resolver, const MarkingFactory_ABC& markingFactory )
    {
        std::auto_ptr< HlaObjectFactory_ABC > result( new HlaObjectFactory< Rpr >( markingFactory ) );
        if( xis.attribute< bool >( "netn", true ) )
            return std::auto_ptr< HlaObjectFactory_ABC >( new NetnHlaObjectFactory< Netn >( result, resolver ) );
        return result;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< RemoteHlaObjectFactory_ABC > CreateRemoteFactory( xml::xisubstream xis )
    {
        std::auto_ptr< RemoteHlaObjectFactory_ABC > result( new RemoteHlaObjectFactory< Rpr >() );
        if( xis.attribute< bool >( "netn", true ) )
            return std::auto_ptr< RemoteHlaObjectFactory_ABC >( new NetnRemoteHlaObjectFactory< Netn >( result ) );
        return result;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< ClassBuilder_ABC > CreateClassBuilder( xml::xisubstream xis )
    {
        if( xis.attribute< bool >( "netn", true ) )
            return std::auto_ptr< ClassBuilder_ABC >( new Netn() );
        return std::auto_ptr< ClassBuilder_ABC >( new Rpr() );
    }
}

struct FederateFacade::FederationDestructor : private boost::noncopyable
{
public:
    FederationDestructor( Federate_ABC& federate, const std::string& federation )
        : federate_  ( federate )
        , federation_( federation )
    {
        // NOTHING
    }
    virtual ~FederationDestructor()
    {
        try
        {
            if( !federate_.Destroy( federation_ ) )
                throw std::runtime_error( "Could not destroy the federation '" + federation_ + "'" );
        }
        catch( ... )
        {
            // NOTHING
        }
    }
private:
    Federate_ABC& federate_;
    const std::string federation_;
};

// -----------------------------------------------------------------------------
// Name: FederateFacade constructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::FederateFacade( xml::xisubstream xis, tools::MessageController_ABC< sword::SimToClient_Content >& controller,
                                AgentSubject_ABC& subject, LocalAgentResolver_ABC& resolver, const RtiAmbassadorFactory_ABC& rtiFactory,
                                const FederateAmbassadorFactory_ABC& federateFactory, const std::string& pluginDirectory, CallsignResolver_ABC& callsignResolver )
    : subject_           ( subject )
    , markingFactory_    ( new MarkingFactory( xis ) )
    , timeFactory_       ( new ::hla::SimpleTimeFactory() )
    , intervalFactory_   ( new ::hla::SimpleTimeIntervalFactory() )
    , ambassador_        ( rtiFactory.CreateAmbassador( *timeFactory_, *intervalFactory_, ::hla::RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host", "localhost" ), xis.attribute< std::string >( "port", "8989" ) ) )
    , federate_          ( CreateFederate( xis, *ambassador_, federateFactory, pluginDirectory ) )
    , destructor_        ( xis.attribute< bool >( "destruction", false ) ? new FederateFacade::FederationDestructor( *federate_, xis.attribute< std::string >( "federation", "Federation" ) ) : 0 )
    , aggregateClass_    ( new HlaClass( *federate_, resolver,
                                         CreateFactory< AggregateEntity, NetnAggregate >( xis, callsignResolver, *markingFactory_ ),
                                         CreateRemoteFactory< RemoteAggregate, NetnRemoteAggregate >( xis ),
                                         CreateClassBuilder< AggregateEntityBuilder, NetnAggregateEntityBuilder >( xis ) ) )
    , surfaceVesselClass_( new HlaClass( *federate_, resolver,
                                         CreateFactory< SurfaceVessel, NetnSurfaceVessel >( xis, callsignResolver, *markingFactory_ ),
                                         CreateRemoteFactory< RemoteSurfaceVessel, NetnRemoteSurfaceVessel >( xis ),
                                         CreateClassBuilder< SurfaceVesselBuilder, NetnSurfaceVesselBuilder >( xis ) ) )
    , aircraftClass_     ( new HlaClass( *federate_, resolver,
                                         CreateFactory< Aircraft, NetnAircraft >( xis, callsignResolver, *markingFactory_ ),
                                         CreateRemoteFactory< RemoteAircraft, NetnRemoteAircraft >( xis ),
                                         CreateClassBuilder< AircraftBuilder, NetnAircraftBuilder >( xis ) ) )
{
    subject_.Register( *this );
    CONNECT( controller, *this, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    subject_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Notify
// Created: SLI 2011-06-24
// -----------------------------------------------------------------------------
void FederateFacade::Notify( const sword::ControlEndTick& /*message*/, int /*context*/ )
{
    federate_->Step();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Register
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void FederateFacade::Register( RemoteAgentListener_ABC& listener )
{
    aggregateClass_->Register( listener );
    surfaceVesselClass_->Register( listener );
    aircraftClass_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Unregister
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void FederateFacade::Unregister( RemoteAgentListener_ABC& listener )
{
    aircraftClass_->Unregister( listener );
    surfaceVesselClass_->Unregister( listener );
    aggregateClass_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Connect
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
bool FederateFacade::Connect()
{
    return federate_->Connect();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Disconnect
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Disconnect()
{
    federate_->Disconnect();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Create
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
bool FederateFacade::Create( const std::string& federation, const std::string& fomFile )
{
    return federate_->Create( federation, fomFile );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Destroy
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
bool FederateFacade::Destroy( const std::string& federation )
{
    return federate_->Destroy( federation );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Join
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
bool FederateFacade::Join( const std::string& federation, bool timeConstrained, bool timeRegulating )
{
    return federate_->Join( federation, timeConstrained, timeRegulating );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Resign
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Resign()
{
    federate_->Resign();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Step
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Step()
{
    federate_->Step();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Register
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Register( const ::hla::ClassIdentifier& classID, ::hla::Class_ABC& objectClass, bool publish, bool subscribe )
{
    federate_->Register( classID, objectClass, publish, subscribe );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Register
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Register( const ::hla::InteractionIdentifier& interactionID, ::hla::Interaction_ABC& interactionClass, bool publish, bool subscribe )
{
    federate_->Register( interactionID, interactionClass, publish, subscribe );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Register
// Created: SLI 2011-09-27
// -----------------------------------------------------------------------------
void FederateFacade::Register( ::hla::FederateAmbassador_ABC& listener )
{
    federate_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::AggregateCreated
// Created: SLI 2011-10-05
// -----------------------------------------------------------------------------
void FederateFacade::AggregateCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type, const std::string& symbol )
{
    aggregateClass_->Created( agent, identifier, name, force, type, symbol );
}
