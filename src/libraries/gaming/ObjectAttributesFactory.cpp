// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectAttributesFactory.h"

#include "ConstructionAttribute.h"
#include "BypassAttribute.h"
#include "ObstacleAttribute.h"
#include "MineAttribute.h"
#include "SupplyRouteAttribute.h"
#include "CrossingSiteAttribute.h"
#include "LogisticAttribute.h"
#include "ActivityTimeAttribute.h"
#include "DelayAttribute.h"
#include "NBCAttribute.h"
#include "FireAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectAttributesFactory::ObjectAttributesFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
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

// -----------------------------------------------------------------------------
// Name: ObjectAttributesFactory::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectAttributesFactory::Register( kernel::Entity_ABC& entity, const sword::ObjectAttributes& attributes ) const
{
    if( attributes.has_logistic() && entity.Retrieve< kernel::LogisticAttribute_ABC >() == 0 )
        entity.Attach< kernel::LogisticAttribute_ABC >( *new LogisticAttribute( controllers_.controller_, model_.agents_ ) );

    if( attributes.has_construction() && entity.Retrieve< kernel::ConstructionAttribute_ABC >() == 0 )
        entity.Attach< kernel::ConstructionAttribute_ABC >( *new ConstructionAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_mine() && entity.Retrieve< kernel::MineAttribute_ABC >() == 0 )
        entity.Attach< kernel::MineAttribute_ABC >( *new MineAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_bypass() && entity.Retrieve< kernel::BypassAttribute_ABC >() == 0 )
        entity.Attach< kernel::BypassAttribute_ABC >( *new BypassAttribute( controllers_.controller_ ) );

    if( attributes.has_obstacle() && entity.Retrieve< kernel::ObstacleAttribute_ABC >() == 0 )
        entity.Attach< kernel::ObstacleAttribute_ABC >( *new ObstacleAttribute( controllers_.controller_ ) );

    if( attributes.has_activity_time() && entity.Retrieve< kernel::ActivityTimeAttribute_ABC >() == 0 )
        entity.Attach< kernel::ActivityTimeAttribute_ABC >( *new ActivityTimeAttribute( controllers_.controller_ ) );

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

    if( attributes.has_medical_treatment() && entity.Retrieve< kernel::MedicalTreatmentAttribute_ABC >() == 0 )
        entity.Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_medical_treatment() && entity.Retrieve< kernel::MedicalTreatmentAttribute_ABC >() == 0 )
        entity.Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_resource_networks() && entity.Retrieve< kernel::ResourceNetwork_ABC >() == 0 )
        model_.resourceNetwork_.Create( entity, attributes.resource_networks() );
}


