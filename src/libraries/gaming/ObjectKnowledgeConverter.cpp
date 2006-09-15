// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ObjectKnowledgeConverter.h"
#include "clients_kernel/Controllers.h"
#include "ObjectKnowledge.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter constructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
ObjectKnowledgeConverter::ObjectKnowledgeConverter( Controllers& controller )
    : controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter destructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
ObjectKnowledgeConverter::~ObjectKnowledgeConverter()
{
    controller_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge* ObjectKnowledgeConverter::Find( const ObjectKnowledge& base, const Team_ABC& owner )
{
    const Object_ABC* real = base.GetRealObject();
    return real ? Find( *real, owner ) : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge* ObjectKnowledgeConverter::Find( const Object_ABC& base, const Team_ABC& owner )
{
    return knowledges_[ & owner ][ & base ];
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::NotifyCreated
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ObjectKnowledgeConverter::NotifyCreated( const ObjectKnowledge& k )
{
    knowledges_[ & k.GetTeam() ][ k.GetRealObject() ] = &k;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ObjectKnowledgeConverter::NotifyDeleted( const ObjectKnowledge& k )
{
    knowledges_[ & k.GetTeam() ].erase( k.GetRealObject() );
}
