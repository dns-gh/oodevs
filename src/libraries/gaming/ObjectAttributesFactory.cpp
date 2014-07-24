// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectAttributesFactory.h"

#include "AltitudeModifierAttribute.h"
#include "ConstructionAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "MineAttribute.h"
#include "SupplyRouteAttribute.h"
#include "CrossingSiteAttribute.h"
#include "LodgingAttribute.h"
#include "LogisticAttribute.h"
#include "TimeLimitedAttribute.h"
#include "DelayAttribute.h"
#include "NBCAttribute.h"
#include "FireAttribute.h"
#include "BurnAttribute.h"
#include "FloodAttribute.h"
#include "BurnSurfaceAttribute.h"
#include "StockAttribute.h"
#include "ToxicCloudAttribute.h"
#include "UndergroundAttribute.h"
#include "TrafficabilityAttribute.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "PropagationAttribute.h"
#include "LocationPositions.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Location_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Point.h"

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectAttributesFactory::ObjectAttributesFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                                                  const kernel::Time_ABC& simulation )
    : controllers_( controllers )
    , simulation_ ( simulation )
    , model_      ( model )
    , static_     ( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectAttributesFactory::~ObjectAttributesFactory()
{
    // NOTHING
}

namespace
{
    bool HasSinglePoint( const kernel::Entity_ABC* entity )
    {
        if( !entity )
            return false;
        const LocationPositions* pPositions = dynamic_cast< const LocationPositions* >( entity->Retrieve< kernel::Positions >() );
        if( !pPositions )
            return false;
        if( pPositions && pPositions->GetLocation() )
            return ( dynamic_cast< const kernel::Point* >( pPositions->GetLocation() ) != nullptr );
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectAttributesFactory::Register( kernel::Object_ABC& entity, const sword::ObjectAttributes& attributes ) const
{
    Register( entity, attributes, HasSinglePoint( &entity ) );
    if( attributes.has_propagation() && entity.Retrieve< kernel::DisasterAttribute_ABC >() == 0 )
        entity.Attach< kernel::DisasterAttribute_ABC >( *new PropagationAttribute( controllers_.controller_, static_.coordinateConverter_,
                                                                                   entity.GetType(), static_.disasterTypes_, simulation_ ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory::Register
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void ObjectAttributesFactory::Register( kernel::ObjectKnowledge_ABC& entity, const sword::ObjectAttributes& attributes ) const
{
    Register( entity, attributes, HasSinglePoint( entity.GetEntity() ) );
    if( attributes.has_propagation() && entity.Retrieve< kernel::DisasterAttribute_ABC >() == 0 )
    {
        const kernel::Object_ABC* object = entity.GetEntity();
        if( object )
            entity.Attach< kernel::DisasterAttribute_ABC >( *new PropagationAttribute( controllers_.controller_, static_.coordinateConverter_,
                                                                                       object->GetType(), static_.disasterTypes_, simulation_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory::Register
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void ObjectAttributesFactory::Register( kernel::Entity_ABC& entity, const sword::ObjectAttributes& attributes, bool singlePoint ) const
{
    if( attributes.has_logistic() && entity.Retrieve< kernel::LogisticAttribute_ABC >() == 0 )
        entity.Attach< kernel::LogisticAttribute_ABC >( *new LogisticAttribute( controllers_.controller_, model_.agents_, model_.teams_ ) );

    if( attributes.has_lodging() && entity.Retrieve< kernel::LodgingAttribute_ABC >() == 0 )
        entity.Attach< kernel::LodgingAttribute_ABC >( *new LodgingAttribute( controllers_.controller_, model_.agents_ ) );

    if( attributes.has_construction() && entity.Retrieve< kernel::ConstructionAttribute_ABC >() == 0 )
        entity.Attach< kernel::ConstructionAttribute_ABC >( *new ConstructionAttribute( controllers_.controller_, static_.objectTypes_, singlePoint ) );

    if( attributes.has_mine() && entity.Retrieve< kernel::MineAttribute_ABC >() == 0 )
        entity.Attach< kernel::MineAttribute_ABC >( *new MineAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_bypass() && entity.Retrieve< kernel::BypassAttribute_ABC >() == 0 )
        entity.Attach< kernel::BypassAttribute_ABC >( *new BypassAttribute( controllers_.controller_, singlePoint ) );

    if( attributes.has_obstacle() && entity.Retrieve< kernel::ObstacleAttribute_ABC >() == 0 )
        entity.Attach< kernel::ObstacleAttribute_ABC >( *new ObstacleAttribute( controllers_.controller_, singlePoint ) );

    if( attributes.has_life_time() && entity.Retrieve< kernel::TimeLimitedAttribute_ABC >() == 0 )
        entity.Attach< kernel::TimeLimitedAttribute_ABC >( *new TimeLimitedAttribute( controllers_.controller_ ) );

    if( attributes.has_crossing_site() && entity.Retrieve< kernel::CrossingSiteAttribute_ABC >() == 0 )
        entity.Attach< kernel::CrossingSiteAttribute_ABC >( *new CrossingSiteAttribute( controllers_.controller_ ) );

    if( attributes.has_effect_delay() && entity.Retrieve< kernel::DelayAttribute_ABC >() == 0 )
        entity.Attach< kernel::DelayAttribute_ABC >( *new DelayAttribute( controllers_.controller_ ) );

    if( attributes.has_supply_route() && entity.Retrieve< kernel::SupplyRouteAttribute_ABC >() == 0 )
        entity.Attach< kernel::SupplyRouteAttribute_ABC >( *new SupplyRouteAttribute( controllers_.controller_ ) );

    if( attributes.has_nbc() && entity.Retrieve< kernel::NBCAttribute_ABC >() == 0 )
        entity.Attach< kernel::NBCAttribute_ABC >( *new NBCAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_fire() && entity.Retrieve< kernel::FireAttribute_ABC >() == 0 )
        entity.Attach< kernel::FireAttribute_ABC >( *new FireAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_burn() && entity.Retrieve< kernel::BurnAttribute_ABC >() == 0 )
        entity.Attach< kernel::BurnAttribute_ABC >( *new BurnAttribute( controllers_.controller_ ) );

    if( attributes.has_burn_surface() && entity.Retrieve< kernel::BurnSurfaceAttribute_ABC >() == 0 )
        entity.Attach< kernel::BurnSurfaceAttribute_ABC >( *new BurnSurfaceAttribute( controllers_.controller_, model_.publisher_, entity ) );

    if( attributes.has_resource_networks() && entity.Retrieve< gui::ResourceNetwork_ABC >() == 0 )
        model_.resourceNetwork_.Create( entity, attributes.resource_networks() );

    if( attributes.has_flood() && entity.Retrieve< kernel::FloodAttribute_ABC >() == 0 )
        entity.Attach< kernel::FloodAttribute_ABC >( *new FloodAttribute( controllers_.controller_, model_.floodProxy_, entity.Get< kernel::Positions >() ) );

    if( attributes.has_altitude_modifier() && entity.Retrieve< kernel::AltitudeModifierAttribute_ABC >() == 0 )
        entity.Attach< kernel::AltitudeModifierAttribute_ABC >( *new AltitudeModifierAttribute( controllers_.controller_, static_.detection_, entity ) );

    if( attributes.has_stock() && entity.Retrieve< kernel::StockAttribute_ABC >() == 0 )
        entity.Attach< kernel::StockAttribute_ABC >( *new StockAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_toxic_cloud() && entity.Retrieve< kernel::ToxicCloudAttribute_ABC >() == 0 )
        entity.Attach< kernel::ToxicCloudAttribute_ABC >( *new ToxicCloudAttribute( controllers_.controller_, static_.coordinateConverter_ ) );

    if( attributes.has_underground() && entity.Retrieve< kernel::UndergroundAttribute_ABC >() == 0 )
        entity.Attach< kernel::UndergroundAttribute_ABC >( *new UndergroundAttribute( controllers_.controller_ ) );

    if( attributes.has_trafficability() && entity.Retrieve< kernel::TrafficabilityAttribute_ABC >() == 0 )
        entity.Attach< kernel::TrafficabilityAttribute_ABC >( *new TrafficabilityAttribute( controllers_.controller_ ) );
}
