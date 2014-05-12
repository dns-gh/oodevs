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
#include "HlaTacticalObjectClass.h"

#include "Federate_ABC.h"
#include "FederateAmbassadorFactory_ABC.h"
#include "RtiAmbassadorFactory_ABC.h"
#include "AgentSubject_ABC.h"
#include "TacticalObjectSubject_ABC.h"
#include "FederateFactory.h"

#include "FomBuilder.h"

#include "MarkingFactory.h"
#include "RemoteAgentResolver_ABC.h"
#include "HlaObjectNameFactory.h"
#include "MT_Tools/MT_Random.h"
#include "protocol/Simulation.h"
#include "rpr/EntityType.h"
#include <hla/SimpleTimeFactory.h>
#include <hla/SimpleTimeIntervalFactory.h>
#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins::hla;

namespace
{
    bool isNetn2( xml::xisubstream xis)
    {
        return xis.attribute< bool >( "netn", true ) &&
                xis.attribute< int >( "netn-version", 1 ) == 2;
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
                throw MASA_EXCEPTION( "Could not destroy the federation '" + federation_ + "'" );
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
                                const FederateAmbassadorFactory_ABC& federateFactory, const tools::Path& pluginDirectory, CallsignResolver_ABC& callsignResolver,
                                TacticalObjectSubject_ABC& tacticalObjectSubject,
                                OwnershipStrategy_ABC& ownershipStrategy, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer,
                                dispatcher::Logger_ABC& logger )
    : subject_           ( subject )
    , tacticalObjectSubject_( tacticalObjectSubject )
    , rtiFactory_        ( rtiFactory )
    , markingFactory_    ( new MarkingFactory( xis ) )
    , timeFactory_       ( new ::hla::SimpleTimeFactory() )
    , intervalFactory_   ( new ::hla::SimpleTimeIntervalFactory() )
    , ambassador_        ( rtiFactory.CreateAmbassador( *timeFactory_, *intervalFactory_, ::hla::RtiAmbassador_ABC::TimeStampOrder, xis.attribute< std::string >( "lrcSettings", "" ) ) )
    , federate_          ( FederateFactory::CreateFederate( xis, *ambassador_, federateFactory, pluginDirectory ) )
    , destructor_        ( xis.attribute< bool >( "destruction", false ) ? new FederateFacade::FederationDestructor( *federate_, xis.attribute< std::string >( "federation", "Federation" ) ) : 0 )
    , nameFactory_       ( new HlaObjectNameFactory( xis.attribute< std::string >( "name", "SWORD" ),
                                boost::lexical_cast< std::string >( MT_Random::GetInstance().rand32() ) ) )
    , fomBuilder_        ( new FomBuilder( xis, *federate_, resolver, *nameFactory_, callsignResolver, *markingFactory_, entityIdentifierResolver, fomSerializer, ownershipStrategy, logger ) )
    , aggregateClass_    ( fomBuilder_->CreateAggregateClass() )
    , surfaceVesselClass_( fomBuilder_->CreateSurfaceVesselClass() )
    , aircraftClass_     ( fomBuilder_->CreateAircraftClass() )
    , groundVehicleClass_( xis.attribute< bool >( "disaggregate", false ) ? fomBuilder_->CreateGroundVehicleClass() : std::unique_ptr< HlaClass >( nullptr ) )
    , humanClass_        ( xis.attribute< bool >( "disaggregate", false ) ? fomBuilder_->CreateHumanClass() : std::unique_ptr< HlaClass >( nullptr ) )
    , rprAggregateClass_ ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", false ) ? fomBuilder_->CreateRprAggregateClass() : std::unique_ptr< HlaClass >( nullptr ) )
    , rprSurfaceVesselClass_ ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", false ) ? fomBuilder_->CreateRprSurfaceVesselClass() : std::unique_ptr< HlaClass >( nullptr ) )
    , rprAircraftClass_  ( xis.attribute< bool >( "netn", true ) && xis.attribute< bool >( "netn-subscribe-rpr", false ) ? fomBuilder_->CreateRprAircraftClass() : std::unique_ptr< HlaClass >( nullptr ) )
    , minefieldClass_    ( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateMinefieldClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , culturalFeatureClass_( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateCulturalFeatureClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , breachableLinearObjectClass_( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateBreachableLinearObjectClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , breachablePointObjectClass_( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateBreachablePointObjectClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , otherPointObjectClass_( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateOtherPointObjectClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , otherArealObjectClass_( xis.attribute< bool >( "handle-objects", true ) ? fomBuilder_->CreateOtherArealObjectClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , rawDataHazardContourGroupClass_( xis.attribute< bool >( "handle-objects", true ) && isNetn2( xis ) ?
            fomBuilder_->CreateRawDataHazardContourGroupClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
    , atp45HazardAreaClass_( xis.attribute< bool >( "handle-objects", true ) && isNetn2( xis ) ?
            fomBuilder_->CreateATP45HazardAreaClass() : std::unique_ptr< HlaTacticalObjectClass >( nullptr ) )
{
    subject_.Register( *this );
    tacticalObjectSubject_.Register( *this );
    CONNECT( controller, *this, control_end_tick );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade destructor
// Created: SBO 2008-02-18
// -----------------------------------------------------------------------------
FederateFacade::~FederateFacade()
{
    tacticalObjectSubject_.Unregister( *this );
    subject_.Unregister( *this );
    groundVehicleClass_.reset( 0 );
    humanClass_.reset( 0 );
    rprAggregateClass_.reset( 0 );
    rprAircraftClass_.reset( 0 );
    rprSurfaceVesselClass_.reset( 0 );
    aircraftClass_.reset( 0 );
    surfaceVesselClass_.reset( 0 );
    aggregateClass_.reset( 0 );
    minefieldClass_.reset( 0 );
    culturalFeatureClass_.reset( 0 );
    breachableLinearObjectClass_.reset( 0 );
    breachablePointObjectClass_.reset( 0 );
    otherPointObjectClass_.reset( 0 );
    otherArealObjectClass_.reset( 0 );
    rawDataHazardContourGroupClass_.reset( 0 );
    atp45HazardAreaClass_.reset( 0 );
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
    if( aggregateClass_ )
        aggregateClass_->Register( listener );
    if( surfaceVesselClass_ )
        surfaceVesselClass_->Register( listener );
    if( aircraftClass_ )
        aircraftClass_->Register( listener );
	if( groundVehicleClass_ )
		groundVehicleClass_->Register( listener );
	if( humanClass_ )
		humanClass_->Register( listener );
    if( rprAggregateClass_ )
        rprAggregateClass_->Register( listener );
    if( rprSurfaceVesselClass_ )
        rprSurfaceVesselClass_->Register( listener );
    if( rprAircraftClass_ )
        rprAircraftClass_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Unregister
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void FederateFacade::Unregister( ClassListener_ABC& listener )
{
    if( aircraftClass_ )
        aircraftClass_->Unregister( listener );
    if( surfaceVesselClass_ )
        surfaceVesselClass_->Unregister( listener );
    if( aggregateClass_ )
        aggregateClass_->Unregister( listener );
	if( groundVehicleClass_.get() )
		groundVehicleClass_->Unregister( listener );
	if( humanClass_.get() )
		humanClass_->Unregister( listener );
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
void FederateFacade::AggregateCreated( Agent_ABC& agent, unsigned long identifier, const std::string& name, rpr::ForceIdentifier force,
        const rpr::EntityType& type, const std::string& symbol, bool isLocal, const std::vector< char >& uniqueId )
{
    // TODO Handle vessels and aircrafts
    if( isLocal && aggregateClass_ )
        aggregateClass_->Created( agent, identifier, name, force, type, symbol, uniqueId );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::DivestRequest
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void FederateFacade::DivestRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    federate_->DivestRequest( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::AcquisitionRequest
// Created: AHC 2012-03-01
// -----------------------------------------------------------------------------
void FederateFacade::AcquisitionRequest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    federate_->AcquisitionRequest( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::Tick
// Created: AHC 2012-03-21
// -----------------------------------------------------------------------------
void FederateFacade::Tick()
{
    federate_->Tick();
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::UnconditionalDivest
// Created: AHC 2012-04-16
// -----------------------------------------------------------------------------
void FederateFacade::UnconditionalDivest( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes )
{
    federate_->UnconditionalDivest( objectID, attributes );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::UnconditionalDivest
// Created: AHC 2012-04-17
// -----------------------------------------------------------------------------
void FederateFacade::UnconditionalAcquisition( const ::hla::ObjectIdentifier& objectID, const T_AttributeIdentifiers& attributes, const ::hla::VariableLengthData& tag )
{
    federate_->UnconditionalAcquisition( objectID, attributes, tag );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::PlatformCreated
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void FederateFacade::PlatformCreated( Agent_ABC& agent, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force,
        const rpr::EntityType& type, const std::string& symbol, const std::vector< char >& uniqueId )
{
    if( type.Kind() == static_cast< char >( rpr::EntityType::kind_LIFEFORM ) )
    {   
        if( humanClass_ )
	        humanClass_->Created( agent, identifier, name, force, type, symbol, uniqueId );
    }
    else
    {
        switch( type.Domain() )
        {
        case rpr::EntityType::domain_LAND:
		    if( groundVehicleClass_ )
			    groundVehicleClass_->Created( agent, identifier, name, force, type, symbol, uniqueId );
            break;
        case rpr::EntityType::domain_AIR:
            if( aircraftClass_ )
                aircraftClass_->Created( agent, identifier, name, force, type, symbol, uniqueId );
            break;
        case rpr::EntityType::domain_SURFACE:
            if( surfaceVesselClass_ )
                surfaceVesselClass_->Created( agent, identifier, name, force, type, symbol, uniqueId );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::ObjectCreated
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void FederateFacade::ObjectCreated( TacticalObject_ABC& object, unsigned int identifier, const std::string& name, rpr::ForceIdentifier force, const rpr::EntityType& type,
    bool isBreachable, TacticalObjectListener_ABC::GeometryType geometry, bool isPropagation )
{
    if( isPropagation )
    {
        if( rawDataHazardContourGroupClass_ )
        {
            rawDataHazardContourGroupClass_->Created( object, identifier, name, force, type );
        }
    }
    else if( isBreachable )
    {
        switch( geometry )
        {
            case TacticalObjectListener_ABC::eGeometryType_Point:
                if( breachablePointObjectClass_ )
                    breachablePointObjectClass_->Created( object, identifier, name, force, type );
                break;
            case TacticalObjectListener_ABC::eGeometryType_Line:
                if( breachableLinearObjectClass_ )
                    breachableLinearObjectClass_->Created( object, identifier, name, force, type );
                break;
            case TacticalObjectListener_ABC::eGeometryType_Polygon:
                if( minefieldClass_ )
                    minefieldClass_->Created( object, identifier, name, force, type );
                break;
        }
    }
    else
    {
        switch( geometry )
        {
            case TacticalObjectListener_ABC::eGeometryType_Point:
                if( otherPointObjectClass_ )
                    otherPointObjectClass_->Created( object, identifier, name, force, type );
                break;
            case TacticalObjectListener_ABC::eGeometryType_Line:
                if( breachableLinearObjectClass_ ) // TODO replace when available in RPR-FOM
                    breachableLinearObjectClass_->Created( object, identifier, name, force, type );
                break;
            case TacticalObjectListener_ABC::eGeometryType_Polygon:
                if( otherArealObjectClass_ )
                    otherArealObjectClass_->Created( object, identifier, name, force, type );
                break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::RegisterTactical
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void FederateFacade::RegisterTactical( ClassListener_ABC& listener )
{
    if( minefieldClass_ )
        minefieldClass_->Register( listener );
    if( breachableLinearObjectClass_ )
        breachableLinearObjectClass_->Register( listener );
    if( breachablePointObjectClass_ )
        breachablePointObjectClass_->Register( listener );
    if( otherPointObjectClass_ )
        otherPointObjectClass_->Register( listener );
    if( otherArealObjectClass_ )
        otherArealObjectClass_->Register( listener );
    if( rawDataHazardContourGroupClass_ )
        rawDataHazardContourGroupClass_->Register( listener );
    if( atp45HazardAreaClass_ )
        atp45HazardAreaClass_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::UnregisterTactical
// Created: AHC 2012-09-07
// -----------------------------------------------------------------------------
void FederateFacade::UnregisterTactical( ClassListener_ABC& listener )
{
    if( minefieldClass_ )
        minefieldClass_->Unregister( listener );
    if( breachableLinearObjectClass_ )
        breachableLinearObjectClass_->Unregister( listener );
    if( breachablePointObjectClass_ )
        breachablePointObjectClass_->Unregister( listener );
    if( otherPointObjectClass_ )
        otherPointObjectClass_->Unregister( listener );
    if( otherArealObjectClass_ )
        otherArealObjectClass_->Unregister( listener );
    if( rawDataHazardContourGroupClass_ )
        rawDataHazardContourGroupClass_->Unregister( listener );
    if( atp45HazardAreaClass_ )
        atp45HazardAreaClass_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: FederateFacade::GetFederateHandle
// Created: AHC 2013-03-29
// -----------------------------------------------------------------------------
const ::hla::FederateIdentifier& FederateFacade::GetFederateHandle() const
{
    return federate_->GetFederateHandle();
}
