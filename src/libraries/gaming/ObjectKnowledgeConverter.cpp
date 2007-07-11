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
#include "ObjectKnowledge_ABC.h"

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
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::Team_ABC& owner )
{
    T_KnowledgeMap knowledges = knowledges_[ &owner ];
    for( T_KnowledgeMap::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
        if( it->second->GetId() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const ObjectKnowledge_ABC& base, const Team_ABC& owner )
{
    const Object_ABC* real = base.GetEntity();
    return real ? Find( *real, owner ) : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const Object_ABC& base, const Team_ABC& owner )
{
    return knowledges_[ & owner ][ & base ];
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::NotifyCreated
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ObjectKnowledgeConverter::NotifyCreated( const ObjectKnowledge_ABC& k )
{
    knowledges_[ & k.GetOwner() ][ k.GetEntity() ] = &k;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void ObjectKnowledgeConverter::NotifyDeleted( const ObjectKnowledge_ABC& k )
{
    knowledges_[ & k.GetOwner() ].erase( k.GetEntity() );
}
