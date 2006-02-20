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
#include "Object_ABC.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "ObjectTypes.h"

// -----------------------------------------------------------------------------
// Name: ObjectFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
ObjectFactory::ObjectFactory( Controller& controller, Model& model )
    : controller_( controller )
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
Object_ABC* ObjectFactory::Create( const ASN1T_MsgObjectCreation& message )
{
    Object_ABC* result = new Object_ABC( message, controller_, model_.teams_, model_.objectTypes_ );
    switch( message.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        result->Attach( *new CampAttributes( controller_, model_.agents_ ) );
        result->Update( message );
        break;
    case EnumObjectType::itineraire_logistique:
        result->Attach( *new LogisticRouteAttributes( controller_ ) );
        result->Update( message );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        result->Attach( *new NBCAttributes( controller_ ) );
        result->Update( message );
        break;
    case EnumObjectType::rota:
        result->Attach( *new RotaAttributes( controller_ ) );
        result->Update( message );
        break;
    case EnumObjectType::site_franchissement:
        result->Attach( *new CrossingSiteAttributes( controller_ ) );
        result->Update( message );
    default:
        ;
    };
    return result;
}
