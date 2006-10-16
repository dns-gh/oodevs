// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledgeFactory.h"
#include "ObjectKnowledge.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "CampAttributes.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "ObjectKnowledgePositions.h"
#include "ObjectPerceptions.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::~ObjectKnowledgeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory::Create
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledge_ABC* ObjectKnowledgeFactory::Create( const Team_ABC& owner, const ASN1T_MsgObjectKnowledgeCreation& message )
{
    ObjectKnowledge* knowledge = new ObjectKnowledge( owner, message, controllers_.controller_, static_.coordinateConverter_, model_.objects_, static_.objectTypes_ );
    knowledge->Attach< Positions >( *new ObjectKnowledgePositions( static_.coordinateConverter_ ) );
    knowledge->Attach( *new ObjectPerceptions( controllers_.controller_, model_.agents_ ) );
    switch( message.type )
    {
    case EnumObjectType::camp_prisonniers:
    case EnumObjectType::camp_refugies:
        knowledge->Attach< CampAttributes_ABC >( *new CampAttributes( controllers_.controller_, model_.agents_ ) );
        break;
    case EnumObjectType::itineraire_logistique:
        knowledge->Attach< LogisticRouteAttributes_ABC >( *new LogisticRouteAttributes( controllers_.controller_ ) );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        knowledge->Attach< NBCAttributes_ABC >( *new NBCAttributes( controllers_.controller_, static_.objectTypes_ ) );
        break;
    case EnumObjectType::rota:
        knowledge->Attach< RotaAttributes_ABC >( *new RotaAttributes( controllers_.controller_, static_.objectTypes_ ) );
        break;
    case EnumObjectType::site_franchissement:
        knowledge->Attach< CrossingSiteAttributes_ABC >( *new CrossingSiteAttributes( controllers_.controller_ ) );
        break;
    default:
        ;
    };
    knowledge->Polish();
    return knowledge;
}
