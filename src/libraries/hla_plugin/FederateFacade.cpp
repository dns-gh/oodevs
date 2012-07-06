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
#include "HlaObjectNameFactory.h"
#include "MT_Tools/MT_Random.h"
#include "protocol/Simulation.h"
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

using namespace plugins::hla;

namespace
{
    struct NullFactory : public HlaObjectFactory_ABC
    {
        virtual std::auto_ptr< HlaObject_ABC > Create( Agent_ABC&, const std::string&, unsigned int, rpr::ForceIdentifier, const rpr::EntityType&, const std::string& ) const
        {
            return std::auto_ptr< HlaObject_ABC >( 0 );
        }
    };
    std::auto_ptr< Federate_ABC > CreateFederate( xml::xisubstream xis, hla::RtiAmbassador_ABC& ambassador, const FederateAmbassadorFactory_ABC& factory, const std::string& pluginDirectory )
    {
        std::auto_ptr< Federate_ABC > federate = factory.Create( ambassador, xis.attribute< std::string >( "name", "SWORD" ), xis.attribute< int >( "lookahead", -1 ) );
        if( !federate->Connect() )
            throw std::runtime_error( "Could not connect to '" + xis.attribute< std::string >( "host", "default" ) + ":" + xis.attribute< std::string >( "port", "default" ) + "'" );
        const std::string name = xis.attribute< std::string >( "federation", "Federation" );
        const bool joined = federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) );
        if( !joined )
        {
            if( xis.attribute< bool >( "creation", false ) )
            {
                std::string fom = xis.attribute< std::string >( "fom", "ASI_FOM_v2.0.8_2010.xml" );
                std::vector<std::string> fomFiles;

                boost::split(fomFiles, fom, boost::is_any_of(",;"));

                if (fomFiles.size() > 1)
                {
                    if( !federate->Create( name, fomFiles ) )
                        throw std::runtime_error( "Could not create the federation '" + name + "'" );
                }
                else
                {
                    if( !boost::filesystem::path(fom).is_complete() )
                        fom = pluginDirectory + "/" + fom ;
                    if( !federate->Create( name, fom ) )
                        throw std::runtime_error( "Could not create the federation '" + name + "'" );
                }


                if( !federate->Join( name, xis.attribute< bool >( "time-constrained", true ), xis.attribute< bool >( "time-regulating", true ) ) )
                    throw std::runtime_error( "Could not join the federation '" + name + "'" );
            }
            else
                throw std::runtime_error( "Could not join the federation '" + name + "'" );
        }
        return federate;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< HlaObjectFactory_ABC > CreateFactory( bool isNetn, CallsignResolver_ABC& resolver, const MarkingFactory_ABC& markingFactory,
            unsigned short siteID, unsigned short applicationID )
    {
        std::auto_ptr< HlaObjectFactory_ABC > result( new HlaObjectFactory< Rpr >( markingFactory, siteID, applicationID ) );
        if( isNetn )
            return std::auto_ptr< HlaObjectFactory_ABC >( new NetnHlaObjectFactory< Netn >( result, resolver ) );
        return result;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< RemoteHlaObjectFactory_ABC > CreateRemoteFactory( bool isNetn )
    {
        std::auto_ptr< RemoteHlaObjectFactory_ABC > result( new RemoteHlaObjectFactory< Rpr >() );
        if( isNetn )
            return std::auto_ptr< RemoteHlaObjectFactory_ABC >( new NetnRemoteHlaObjectFactory< Netn >( result ) );
        return result;
    }
    template< typename Rpr, typename Netn >
    std::auto_ptr< ClassBuilder_ABC > CreateClassBuilder( bool isNetn )
    {
        if( isNetn )
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
    , rtiFactory_        ( rtiFactory )
    , markingFactory_    ( new MarkingFactory( xis ) )
    , timeFactory_       ( new ::hla::SimpleTimeFactory() )
    , intervalFactory_   ( new ::hla::SimpleTimeIntervalFactory() )
    , ambassador_        ( rtiFactory.CreateAmbassador( *timeFactory_, *intervalFactory_, ::hla::RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "host", "" ), xis.attribute< std::string >( "port", "" ) ) )
    , federate_          ( CreateFederate( xis, *ambassador_, federateFactory, pluginDirectory ) )
    , destructor_        ( xis.attribute< bool >( "destruction", false ) ? new FederateFacade::FederationDestructor( *federate_, xis.attribute< std::string >( "federation", "Federation" ) ) : 0 )
    , nameFactory_       ( new HlaObjectNameFactory( xis.attribute< std::string >( "name", "SWORD" ),
                                boost::lexical_cast< std::string >( MT_Random::GetInstance().rand32() ) ) )
    , aggregateClass_    ( new HlaClass( *federate_, resolver, *nameFactory_,
                                         CreateFactory< AggregateEntity, NetnAggregate >( xis.attribute< bool >( "netn", true ), callsignResolver, *markingFactory_, xis.attribute<unsigned short>("dis-site",1), xis.attribute<unsigned short>("dis-application", 1) ),
                                         CreateRemoteFactory< RemoteAggregate, NetnRemoteAggregate >( xis.attribute< bool >( "netn", true ) ),
                                         CreateClassBuilder< AggregateEntityBuilder, NetnAggregateEntityBuilder >( xis.attribute< bool >( "netn", true ) ) ) )
    , surfaceVesselClass_( new HlaClass( *federate_, resolver, *nameFactory_,
                                         CreateFactory< SurfaceVessel, NetnSurfaceVessel >( xis.attribute< bool >( "netn", true ), callsignResolver, *markingFactory_, xis.attribute<unsigned short>("dis-site",1), xis.attribute<unsigned short>("dis-application", 1) ),
                                         CreateRemoteFactory< RemoteSurfaceVessel, NetnRemoteSurfaceVessel >( xis.attribute< bool >( "netn", true ) ),
                                         CreateClassBuilder< SurfaceVesselBuilder, NetnSurfaceVesselBuilder >( xis.attribute< bool >( "netn", true ) ) ) )
    , aircraftClass_     ( new HlaClass( *federate_, resolver, *nameFactory_,
                                         CreateFactory< Aircraft, NetnAircraft >( xis.attribute< bool >( "netn", true ), callsignResolver, *markingFactory_, xis.attribute<unsigned short>("dis-site",1), xis.attribute<unsigned short>("dis-application", 1) ),
                                         CreateRemoteFactory< RemoteAircraft, NetnRemoteAircraft >( xis.attribute< bool >( "netn", true ) ),
                                         CreateClassBuilder< AircraftBuilder, NetnAircraftBuilder >( xis.attribute< bool >( "netn", true ) ) ) )
    , rprAggregateClass_ ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", true ) ?
                                new HlaClass( *federate_, resolver, *nameFactory_,
                                     std::auto_ptr< HlaObjectFactory_ABC >( new NullFactory ),
                                     CreateRemoteFactory< RemoteAggregate, NetnRemoteAggregate >( false ),
                                     CreateClassBuilder< AggregateEntityBuilder, NetnAggregateEntityBuilder >( false )
                                     )
                                : 0 )
    , rprSurfaceVesselClass_ ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", true ) ?
                                new HlaClass( *federate_, resolver, *nameFactory_,
                                     std::auto_ptr< HlaObjectFactory_ABC >( new NullFactory ),
                                     CreateRemoteFactory< RemoteSurfaceVessel, NetnRemoteSurfaceVessel >( false ),
                                     CreateClassBuilder< SurfaceVesselBuilder, NetnSurfaceVesselBuilder >( false)
                                     )
                                : 0 )
    , rprAircraftClass_ ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", true ) ?
                                new HlaClass( *federate_, resolver, *nameFactory_,
                                     std::auto_ptr< HlaObjectFactory_ABC >( new NullFactory ),
                                     CreateRemoteFactory< RemoteAircraft, NetnRemoteAircraft >( false ),
                                     CreateClassBuilder< AircraftBuilder, NetnAircraftBuilder >( false )
                                    )
                                : 0 )
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
    aircraftClass_.reset( 0 );
    surfaceVesselClass_.reset( 0 );
    aggregateClass_.reset( 0 );
    destructor_.reset( 0 );
    federate_.reset( 0 );
    rtiFactory_.DeleteAmbassador( ambassador_ );
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
void FederateFacade::Register( ClassListener_ABC& listener )
{
    aggregateClass_->Register( listener );
    surfaceVesselClass_->Register( listener );
    aircraftClass_->Register( listener );
    if( rprAggregateClass_.get() )
        rprAggregateClass_->Register( listener );
    if( rprSurfaceVesselClass_.get() )
        rprSurfaceVesselClass_->Register( listener );
    if( rprAircraftClass_.get() )
        rprAircraftClass_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Unregister
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void FederateFacade::Unregister( ClassListener_ABC& listener )
{
    aircraftClass_->Unregister( listener );
    surfaceVesselClass_->Unregister( listener );
    aggregateClass_->Unregister( listener );
    if( rprAggregateClass_.get() )
        rprAggregateClass_->Unregister( listener );
    if( rprSurfaceVesselClass_.get() )
        rprSurfaceVesselClass_->Unregister( listener );
    if( rprAircraftClass_.get() )
        rprAircraftClass_->Unregister( listener );
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
// Name: FederateFacade::Create
// Created: DGE 2012-03-28
// -----------------------------------------------------------------------------
bool FederateFacade::Create( const std::string& federation, const T_FomFiles& fomFiles )
{
    return federate_->Create( federation, fomFiles );
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
