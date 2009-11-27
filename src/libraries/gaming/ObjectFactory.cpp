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
#include "NBCAttribute.h"
#include "FireAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "ToxicCloudAttribute.h"
#include "StockAttribute.h"
#include "Explosions.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "ObjectPositions.h"
#include "StaticModel.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ObjectHierarchies.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( Controllers& controllers, Model& model, const StaticModel& staticModel )
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
Object_ABC* ObjectFactory::Create( const ASN1T_MsgObjectCreation& message )
{
    Object* result = new Object( message, controllers_.controller_, static_.coordinateConverter_, static_.objectTypes_ );
    result->Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    result->Attach< Positions >( *new ObjectPositions( result->GetType(), static_.coordinateConverter_ ) );
    result->Attach< TacticalHierarchies >( *new ObjectHierarchies( *result, &model_.teams_.GetTeam( message.team ) ) );
    
    Register( *result, message.attributes );
    
    result->Update( message );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ObjectFactory::Register
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void ObjectFactory::Register( Object_ABC& result, const ASN1T_ObjectAttributes& attributes ) const
{
    if ( attributes.m.logisticPresent )
        result.Attach< LogisticAttribute_ABC >( *new LogisticAttribute( controllers_.controller_, model_.agents_ ) );    
    
    if ( attributes.m.constructionPresent )
        result.Attach< ConstructionAttribute_ABC >( *new ConstructionAttribute( controllers_.controller_, static_.objectTypes_ ) );
    
    if ( attributes.m.minePresent )
        result.Attach< MineAttribute_ABC >( *new MineAttribute( controllers_.controller_, static_.objectTypes_ ) );
    
    if ( attributes.m.bypassPresent )
        result.Attach< BypassAttribute_ABC >( *new BypassAttribute( controllers_.controller_ ) );
    
    if ( attributes.m.obstaclePresent )
        result.Attach< ObstacleAttribute_ABC >( *new ObstacleAttribute( controllers_.controller_ ) );
    
    if ( attributes.m.crossing_sitePresent )
        result.Attach< CrossingSiteAttribute_ABC >( *new CrossingSiteAttribute( controllers_.controller_ ) );
    
    if ( attributes.m.supply_routePresent )
        result.Attach< SupplyRouteAttribute_ABC >( *new SupplyRouteAttribute( controllers_.controller_ ) );    
    
    if ( attributes.m.nbcPresent )
        result.Attach< NBCAttribute_ABC >( *new NBCAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if ( attributes.m.activity_timePresent )
        result.Attach< ActivityTimeAttribute_ABC >( *new ActivityTimeAttribute( controllers_.controller_ ) );

    if ( attributes.m.toxic_cloudPresent )
        result.Attach< ToxicCloudAttribute_ABC >( *new ToxicCloudAttribute( controllers_.controller_, static_.coordinateConverter_ ) );

    if( attributes.m.firePresent )
        result.Attach< FireAttribute_ABC >( *new FireAttribute( controllers_.controller_, static_.objectTypes_ ) );

    if( attributes.m.medical_treatmentPresent )
        result.Attach< MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( controllers_.controller_, static_.objectTypes_ ) );
    
    if( attributes.m.stockPresent )
        result.Attach< StockAttribute_ABC >( *new StockAttribute( controllers_.controller_, static_.objectTypes_ ) );
}
