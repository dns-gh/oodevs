// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectFactory.h"
#include "Object.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "Explosions.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "ObjectTypes.h"
#include "Controllers.h"
#include "ObjectPositions.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
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
Object* ObjectFactory::Create( const ASN1T_MsgObjectCreation& message )
{
    Object* result = new Object( message, controllers_.controller_, model_.coordinateConverter_, model_.teams_, model_.objectTypes_, model_.objectTypes_ );
    result->Attach( *new Explosions( controllers_.controller_, model_.fireResultsFactory_ ) );
    result->Attach< Positions >( *new ObjectPositions( model_.coordinateConverter_ ) );
    switch( message.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        result->Attach( *new CampAttributes( controllers_.controller_, model_.agents_ ) );
        result->Update( message );
        break;
    case EnumObjectType::itineraire_logistique:
        result->Attach( *new LogisticRouteAttributes( controllers_.controller_ ) );
        result->Update( message );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        result->Attach( *new NBCAttributes( controllers_.controller_, model_.objectTypes_ ) );
        result->Update( message );
        break;
    case EnumObjectType::rota:
        result->Attach( *new RotaAttributes( controllers_.controller_, model_.objectTypes_ ) );
        result->Update( message );
        break;
    case EnumObjectType::site_franchissement:
        result->Attach( *new CrossingSiteAttributes( controllers_.controller_ ) );
        result->Update( message );
    default:
        ;
    };
    result->Update( message );
    return result;
}
