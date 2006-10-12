// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Controllers.h"
#include "ObjectPositions.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
    , idManager_( idManager )
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
// Name: ObjectFactory::CreateObject
// Created: SBO 2006-09-11
// -----------------------------------------------------------------------------
kernel::Object_ABC* ObjectFactory::CreateObject( kernel::ObjectType& type, kernel::Team_ABC& team, const kernel::Location_ABC& location )
{
    Object* result = new Object( controllers_.controller_, static_.coordinateConverter_, type, team, idManager_ );
    result->Attach< Positions >( *new ObjectPositions( static_.coordinateConverter_, location ) );
    
    switch( type.id_ )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        result->Attach< CampAttributes_ABC >( *new CampAttributes( controllers_ ) );
        break;
    case EnumObjectType::itineraire_logistique:
        result->Attach< LogisticRouteAttributes_ABC >( *new LogisticRouteAttributes() );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        result->Attach< NBCAttributes_ABC >( *new NBCAttributes() );
        break;
    case EnumObjectType::rota:
        result->Attach< RotaAttributes_ABC >( *new RotaAttributes() );
        break;
    case EnumObjectType::site_franchissement:
        result->Attach< CrossingSiteAttributes_ABC >( *new CrossingSiteAttributes() );
        break;
    default:
        break;
    };
    result->Polish();
    return result;
}
