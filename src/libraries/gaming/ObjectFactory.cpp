// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectFactory.h"
#include "Object.h"
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
#include "ToxicCloudAttribute.h"
#include "StockAttribute.h"
#include "SealOffAttribute.h"
#include "Explosions.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "UrbanModel.h"
#include "ObjectPositions.h"
#include "ResourceNetwork.h"
#include "ResourceNetworkModel.h"
#include "StaticModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ObjectHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Team_ABC.h"
#include "protocol/Simulation.h"

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::~ObjectFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
kernel::Object_ABC* ObjectFactory::Create( const MsgsSimToClient::MsgObjectCreation& message )
{
    Object* result = new Object( message, controllers_.controller_, static_.coordinateConverter_, static_.objectTypes_ );
    result->Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    result->Attach< kernel::Positions >( *new ObjectPositions( result->GetType(), static_.coordinateConverter_ ) );
    result->Attach( *new kernel::PropertiesDictionary( controllers_.controller_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new kernel::ObjectHierarchies( *result, &model_.teams_.GetTeam( message.party().id() ) ) );

    Register( *result, message.attributes() );

    result->Update( message );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectFactory::Register( kernel::Object_ABC& result, const Common::MsgObjectAttributes& attributes ) const
{
    if( attributes.has_logistic()  )
        result.Attach< kernel::LogisticAttribute_ABC >( *new LogisticAttribute( controllers_.controller_, model_.agents_ ) );

    if( attributes.has_construction()  )
        result.Attach< kernel::ConstructionAttribute_ABC >( *new ConstructionAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_mine()  )
        result.Attach< kernel::MineAttribute_ABC >( *new MineAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_bypass()  )
        result.Attach< kernel::BypassAttribute_ABC >( *new BypassAttribute( controllers_.controller_ ) );

    if( attributes.has_obstacle()  )
        result.Attach< kernel::ObstacleAttribute_ABC >( *new ObstacleAttribute( controllers_.controller_ ) );

    if( attributes.has_crossing_site()  )
        result.Attach< kernel::CrossingSiteAttribute_ABC >( *new CrossingSiteAttribute( controllers_.controller_ ) );

    if( attributes.has_supply_route()  )
        result.Attach< kernel::SupplyRouteAttribute_ABC >( *new SupplyRouteAttribute( controllers_.controller_ ) );

    if( attributes.has_nbc()  )
        result.Attach< kernel::NBCAttribute_ABC >( *new NBCAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_activity_time()  )
        result.Attach< kernel::ActivityTimeAttribute_ABC >( *new ActivityTimeAttribute( controllers_.controller_ ) );

    if( attributes.has_effect_delay()  )
        result.Attach< kernel::DelayAttribute_ABC >( *new DelayAttribute( controllers_.controller_ ) );

    if( attributes.has_toxic_cloud()  )
        result.Attach< kernel::ToxicCloudAttribute_ABC >( *new ToxicCloudAttribute( controllers_.controller_, static_.coordinateConverter_ ) );

    if( attributes.has_fire()  )
        result.Attach< kernel::FireAttribute_ABC >( *new FireAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_medical_treatment()  )
        result.Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_stock() )
        result.Attach< kernel::StockAttribute_ABC >( *new StockAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.has_sealoff()  )
        result.Attach< kernel::SealOffAttribute_ABC >( *new SealOffAttribute( controllers_.controller_ ) );

    if( attributes.has_resource_networks() )
        model_.resourceNetwork_.Create( result, attributes.resource_networks() );
}
