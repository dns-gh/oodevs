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
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "MineAttributes.h"
#include "Explosions.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Controllers.h"
#include "ObjectPositions.h"
#include "ObjectHierarchies.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "StaticModel.h"

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
    Object* result = new Object( message, controllers_.controller_, static_.coordinateConverter_, static_.objectTypes_, static_.objectTypes_ );
    result->Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    result->Attach< Positions >( *new ObjectPositions( static_.coordinateConverter_ ) );
    result->Attach< TacticalHierarchies >( *new ObjectHierarchies( *result, model_.teams_.GetTeam( message.camp ) ) );
    switch( message.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        result->Attach< CampAttributes_ABC >( *new CampAttributes( controllers_.controller_, model_.agents_ ) );
        result->Update( message );
        break;
    case EnumObjectType::itineraire_logistique:
        result->Attach< LogisticRouteAttributes_ABC >( *new LogisticRouteAttributes( controllers_.controller_ ) );
        result->Update( message );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        result->Attach< NBCAttributes_ABC >( *new NBCAttributes( controllers_.controller_, static_.objectTypes_ ) );
        result->Update( message );
        break;
    case EnumObjectType::rota:
        result->Attach< RotaAttributes_ABC >( *new RotaAttributes( controllers_.controller_, static_.objectTypes_ ) );
        result->Update( message );
        break;
    case EnumObjectType::site_franchissement:
        result->Attach< CrossingSiteAttributes_ABC >( *new CrossingSiteAttributes( controllers_.controller_ ) );
        result->Update( message );
    case EnumObjectType::bouchon_mines:
    case EnumObjectType::zone_minee_lineaire:
    case EnumObjectType::zone_minee_par_dispersion:
        result->Attach< MineAttributes_ABC >( *new MineAttributes( controllers_.controller_, message.type != EnumObjectType::bouchon_mines ) );
        result->Update( message );
    default:
        ;
    };
    result->Update( message );
    result->Polish();
    return result;
}
