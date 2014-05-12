// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "FomBuilder.h"
#include "HlaClass.h"
#include "HlaTacticalObjectClass.h"

#include "AggregateEntity.h"
#include "NetnAggregate.h"
#include "SurfaceVessel.h"
#include "NetnSurfaceVessel.h"
#include "Aircraft.h"
#include "NetnAircraft.h"
#include "GroundVehicle.h"
#include "NetnGroundVehicle.h"
#include "Human.h"
#include "NetnHuman.h"
#include "Minefield.h"
#include "CulturalFeature.h"
#include "BreachableLinearObject.h"
#include "BreachablePointObject.h"
#include "OtherPointObject.h"
#include "OtherArealObject.h"
#include "RawDataHazardContourGroup.h"
#include "ATP45HazardArea.h"

#include "Federate_ABC.h"
#include "MarkingFactory.h"
#include "RemoteAgentResolver_ABC.h"
#include "HlaObjectNameFactory.h"
#include "HlaFactories.h"
#include "ClassBuilders.h"

#include "dispatcher/Logger_ABC.h"

using namespace plugins::hla;

namespace
{
    bool isHla13( xml::xisubstream xis )
    {
        return xis.attribute< std::string >( "protocol", "hla-1516e" ) == "hla-13";
    }
    struct NullFactory : public HlaObjectFactory_ABC
    {
        virtual std::unique_ptr< HlaObject_ABC > Create( Agent_ABC&, const std::string&, unsigned long, rpr::ForceIdentifier, const rpr::EntityType&, const std::string&, const std::string&, const std::vector< char >& ) const
        {
            return std::unique_ptr< HlaObject_ABC >( nullptr );
        }
    };

    struct NullRemoteFactory :  public RemoteHlaObjectFactory_ABC
    {
        virtual std::unique_ptr< HlaObject_ABC > Create( const std::string& /*name*/ ) const
        {
            return std::unique_ptr< HlaObject_ABC >( nullptr );
        }
    };

    template< typename Rpr, typename Netn >
    std::unique_ptr< HlaObjectFactory_ABC > CreateFactory( bool isNetn, CallsignResolver_ABC& resolver, const MarkingFactory_ABC& markingFactory,
            EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
    {
        std::unique_ptr< HlaObjectFactory_ABC > result( new HlaObjectFactory< Rpr >( markingFactory, entityIdentifierResolver, fomSerializer ) );
        if( isNetn )
            return std::unique_ptr< HlaObjectFactory_ABC >( new NetnHlaObjectFactory< Netn >( std::move( result ), resolver, fomSerializer ) );
        return result;
    }

    template< typename Rpr, typename Netn >
    std::unique_ptr< RemoteHlaObjectFactory_ABC > CreateRemoteFactory( bool isNetn, EntityIdentifierResolver_ABC& entityIdentifierResolver, FOM_Serializer_ABC& fomSerializer )
    {
        std::unique_ptr< RemoteHlaObjectFactory_ABC > result( new RemoteHlaObjectFactory< Rpr >( entityIdentifierResolver, fomSerializer ) );
        if( isNetn )
            return std::unique_ptr< RemoteHlaObjectFactory_ABC >( new NetnRemoteHlaObjectFactory< Netn >( std::move( result ), fomSerializer ) );
        return result;
    }

    template< typename Rpr, typename Netn >
    std::unique_ptr< ClassBuilder_ABC > CreateClassBuilder( bool isNetn, bool isHla13 )
    {
        if( isNetn )
            return std::unique_ptr< ClassBuilder_ABC >( new Netn() );
        return std::unique_ptr< ClassBuilder_ABC >( new Rpr( isHla13 ) );
    }

    std::unique_ptr< HlaClass > CreateClass( Federate_ABC& federate, LocalAgentResolver_ABC& resolver, HlaObjectNameFactory_ABC& nameFactory,
            std::unique_ptr< HlaObjectFactory_ABC > factory, std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory, std::unique_ptr< ClassBuilder_ABC > builder,
            OwnershipStrategy_ABC& ownershipStrategy, dispatcher::Logger_ABC& logger, const std::string& message )
    {
        try
        {
            return std::unique_ptr< HlaClass >( new HlaClass( federate, resolver, nameFactory, std::move( factory ), std::move( remoteFactory ), std::move( builder ), ownershipStrategy) );
        }
        catch( const std::exception& e )
        {
            logger.LogError(message + " : " + e.what() );
        }
        return nullptr;
    }
    std::unique_ptr< HlaTacticalObjectClass > CreateTacticalClass( Federate_ABC& federate, HlaObjectNameFactory_ABC& nameFactory,
            std::unique_ptr< HlaTacticalObjectFactory_ABC > factory, std::unique_ptr< RemoteHlaObjectFactory_ABC > remoteFactory, std::unique_ptr< ClassBuilder_ABC > builder,
            dispatcher::Logger_ABC& logger, const std::string& message )
    {
        try
        {
            return std::unique_ptr< HlaTacticalObjectClass >( new HlaTacticalObjectClass( federate, nameFactory, std::move( factory ), std::move( remoteFactory ), std::move( builder ) ) );
        }
        catch( const std::exception& e)
        {
            logger.LogError(message + " : " + e.what() );
        }
        return nullptr;
    }
}

// -----------------------------------------------------------------------------
// Name: FomBuilder constructor
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
FomBuilder::FomBuilder( xml::xistream& xis, Federate_ABC& federate, LocalAgentResolver_ABC& resolver, HlaObjectNameFactory_ABC& nameFactory,
        CallsignResolver_ABC& callsignResolver, MarkingFactory_ABC& markingFactory, EntityIdentifierResolver_ABC& entityIdentifierResolver,
        FOM_Serializer_ABC& fomSerializer, OwnershipStrategy_ABC& ownershipStrategy, dispatcher::Logger_ABC& logger )
        : xis_( xis )
        , federate_( federate )
        , resolver_( resolver )
        , nameFactory_( nameFactory )
        , callsignResolver_( callsignResolver )
        , markingFactory_( markingFactory )
        , entityIdentifierResolver_( entityIdentifierResolver )
        , fomSerializer_( fomSerializer )
        , ownershipStrategy_( ownershipStrategy )
        , logger_( logger )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FomBuilder destructor
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
FomBuilder::~FomBuilder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateAggregateClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateAggregateClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< AggregateEntity, NetnAggregate >( xis_.attribute< bool >( "netn", true ), callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< AggregateEntity, NetnAggregate >( xis_.attribute< bool >( "netn", true ), entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< AggregateEntityBuilder, NetnAggregateEntityBuilder >( xis_.attribute< bool >( "netn", true ), isHla13( xis_ ) ), ownershipStrategy_,
                        logger_, "Could not create AggregateClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateSurfaceVesselClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< SurfaceVessel, NetnSurfaceVessel >( xis_.attribute< bool >( "netn", true ), callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< SurfaceVessel, NetnSurfaceVessel >( xis_.attribute< bool >( "netn", true ), entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< SurfaceVesselBuilder, NetnSurfaceVesselBuilder >( xis_.attribute< bool >( "netn", true ), isHla13( xis_ )  ), ownershipStrategy_,
                        logger_, "Could not create SurfaceVesselClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateAircraftClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< Aircraft, NetnAircraft >( xis_.attribute< bool >( "netn", true ), callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< Aircraft, NetnAircraft >( xis_.attribute< bool >( "netn", true ), entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< AircraftBuilder, NetnAircraftBuilder >( xis_.attribute< bool >( "netn", true ), isHla13( xis_ )  ), ownershipStrategy_,
                        logger_, "Could not create AircraftClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateGroundVehicleClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                         CreateFactory< GroundVehicle, NetnGroundVehicle >( xis_.attribute< bool >( "netn", true ), callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                         std::unique_ptr< RemoteHlaObjectFactory_ABC >( new NullRemoteFactory ),
                         CreateClassBuilder< GroundVehicleBuilder, NetnGroundVehicleBuilder >( xis_.attribute< bool >( "netn", true ), isHla13( xis_ )  ), ownershipStrategy_,
                         logger_, "Could not create GroundVehicleClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateRprAggregateClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< AggregateEntity, NetnAggregate >( false, callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< AggregateEntity, NetnAggregate >( false, entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< AggregateEntityBuilder, NetnAggregateEntityBuilder >( false, isHla13( xis_ )  ), ownershipStrategy_,
                        logger_, "Could not create RprAggregateClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateRprSurfaceVesselClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< SurfaceVessel, NetnSurfaceVessel >( false, callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< SurfaceVessel, NetnSurfaceVessel >( false, entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< SurfaceVesselBuilder, NetnSurfaceVesselBuilder >( false, isHla13( xis_ )  ), ownershipStrategy_,
                        logger_, "Could not create RprSurfaceVesselClass" );

}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateRprAircraftClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                        CreateFactory< Aircraft, NetnAircraft >( false, callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                        CreateRemoteFactory< Aircraft, NetnAircraft >( false, entityIdentifierResolver_, fomSerializer_ ),
                        CreateClassBuilder< AircraftBuilder, NetnAircraftBuilder >( false, isHla13( xis_ )  ), ownershipStrategy_,
                        logger_, "Could not create AircraftClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateMinefieldClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateMinefieldClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< Minefield >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< Minefield >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< MinefieldBuilder, MinefieldBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create MinefieldClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateSurfaceVesselClass
// Created: AHC 2011-11-12
// -----------------------------------------------------------------------------
std::unique_ptr< HlaClass > FomBuilder::CreateHumanClass()
{
    return CreateClass( federate_, resolver_, nameFactory_,
                         CreateFactory< Human, NetnHuman >( xis_.attribute< bool >( "netn", true ), callsignResolver_, markingFactory_, entityIdentifierResolver_, fomSerializer_ ),
                         std::unique_ptr< RemoteHlaObjectFactory_ABC >( new NullRemoteFactory ),
                         CreateClassBuilder< HumanBuilder, NetnHumanBuilder >( xis_.attribute< bool >( "netn", true ), isHla13( xis_ )  ), ownershipStrategy_,
                         logger_, "Could not create HumanClass" );
}

// -----------------------------------------------------------------------------
// Name: FomBuilder::CreateCulturalFeatureClass
// Created: AHC 2013-01-21
// -----------------------------------------------------------------------------
std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateCulturalFeatureClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< CulturalFeature >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< CulturalFeature >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< CulturalFeaturedBuilder, CulturalFeaturedBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create CulturalFeatureClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateBreachablePointObjectClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< BreachablePointObject >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< BreachablePointObject >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< BreachablePointObjectBuilder, BreachablePointObjectBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create BreachablePointObjectClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateBreachableLinearObjectClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< BreachableLinearObject >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< BreachableLinearObject >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< BreachableLinearObjectBuilder, BreachableLinearObjectBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create BreachableLinearObjectClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateOtherPointObjectClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< OtherPointObject >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< OtherPointObject >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< OtherPointObjectBuilder, OtherPointObjectBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create OtherPointObjectClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateOtherArealObjectClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< OtherArealObject >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< OtherArealObject >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< OtherArealObjectBuilder, OtherArealObjectBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create OtherArealObjectClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateATP45HazardAreaClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< ATP45HazardArea >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< ATP45HazardArea >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< ATP45HazardAreaBuilder, ATP45HazardAreaBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create ATP45HazardAreaClass" );
}

std::unique_ptr< HlaTacticalObjectClass > FomBuilder::CreateRawDataHazardContourGroupClass()
{
    return CreateTacticalClass( federate_, nameFactory_,
                                std::unique_ptr< HlaTacticalObjectFactory_ABC > ( new TacticalObjectFactory< RawDataHazardContourGroup >( entityIdentifierResolver_ ) ) ,
                                std::unique_ptr< RemoteHlaObjectFactory_ABC > ( new RemoteHlaObjectFactory< RawDataHazardContourGroup >( entityIdentifierResolver_, fomSerializer_ ) ),
                                CreateClassBuilder< RawDataHazardContourGroupBuilder, RawDataHazardContourGroupBuilder >( false, isHla13( xis_ ) ),
                                logger_, "Could not create RawDataHazardContourGroupClass" );
}
