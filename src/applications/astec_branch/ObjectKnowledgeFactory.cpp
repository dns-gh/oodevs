// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ObjectKnowledgeFactory.h"
#include "ObjectKnowledge.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
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
ObjectKnowledge* ObjectKnowledgeFactory::Create( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    ObjectKnowledge* knowledge = new ObjectKnowledge( message, controllers_.controller_, model_.coordinateConverter_, model_.objects_, model_.agents_ );
    
    switch( message.type )
    {
    case EnumObjectType::itineraire_logistique:
        knowledge->Attach( *new LogisticRouteAttributes( controllers_.controller_ ) );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        knowledge->Attach( *new NBCAttributes( controllers_.controller_ ) );
        break;
    case EnumObjectType::rota:
        knowledge->Attach( *new RotaAttributes( controllers_.controller_ ) );
        break;
    case EnumObjectType::site_franchissement:
        knowledge->Attach( *new CrossingSiteAttributes( controllers_.controller_ ) );
    default:
        ;
    };
    return knowledge;
}
