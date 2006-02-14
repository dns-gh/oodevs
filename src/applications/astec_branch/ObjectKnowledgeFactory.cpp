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
#include "ObjectKnowledgeAttributes.h"
#include "ObjectKnowledgeAutomataPerception.h"
#include "LogisticRouteAttributes.h"
#include "NBCAttributes.h"
#include "RotaAttributes.h"
#include "CrossingSiteAttributes.h"

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::ObjectKnowledgeFactory( Controller& controller, const Resolver_ABC< Agent >& agentResolver, const Resolver_ABC< Object_ABC >& objectResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , objectResolver_( objectResolver )
{

}
    
// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledgeFactory::~ObjectKnowledgeFactory()
{

}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeFactory::Create
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
ObjectKnowledge* ObjectKnowledgeFactory::Create( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    ObjectKnowledge* knowledge = new ObjectKnowledge( message, objectResolver_  );
    knowledge->Attach( *new ObjectKnowledgeAutomataPerception( agentResolver_ ) );
    knowledge->Attach( *new ObjectKnowledgeAttributes( controller_, objectResolver_ ) );
    
    switch( message.type )
    {
    case EnumObjectType::itineraire_logistique:
        knowledge->Attach( *new LogisticRouteAttributes() );
        break;
    case EnumObjectType::nuage_nbc:
    case EnumObjectType::zone_nbc:
        knowledge->Attach( *new NBCAttributes() );
        break;
    case EnumObjectType::rota:
        knowledge->Attach( *new RotaAttributes() );
        break;
    case EnumObjectType::site_franchissement:
        knowledge->Attach( *new CrossingSiteAttributes() );
    default:
        ;
    };
    return knowledge;
}
