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
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ObjectKnowledge_ABC.h"

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
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::KnowledgeGroup_ABC& owner ) const
{
    auto it = knowledges_.find( &owner );
    if( it != knowledges_.end() )
    {
        const T_KnowledgeMap& knowledges = it->second;
        for( T_KnowledgeMap::const_iterator it = knowledges.begin(); it != knowledges.end(); ++it )
            if( it->second->GetId() == id )
                return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    if( const kernel::Entity_ABC* entity = owner.Get< kernel::CommunicationHierarchies >().GetSuperior() )
        return Find( id, static_cast< const kernel::KnowledgeGroup_ABC& >( *entity ) );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const ObjectKnowledge_ABC& base, const KnowledgeGroup_ABC& owner ) const
{
    const Object_ABC* real = base.GetEntity();
    return real ? Find( *real, owner ) : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const Object_ABC& base, const KnowledgeGroup_ABC& owner ) const
{
    auto it = knowledges_.find( &owner );
    if( it == knowledges_.end() )
        return 0;
    auto kit = it->second.find( & base );
    if( kit == it->second.end() )
        return 0;
    return kit->second;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const
{
    if( const kernel::KnowledgeGroup_ABC* kg = dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &owner ) )
        if( const ObjectKnowledge_ABC* result = Find( base, *kg ) )
            return result;
    const kernel::Hierarchies* hierarchies = owner.Retrieve< kernel::CommunicationHierarchies >();
    if( hierarchies )
    {
        const kernel::Entity_ABC* entity = hierarchies->GetSuperior();
        if( entity )
            return Find( base, static_cast< const kernel::KnowledgeGroup_ABC& >( *entity ) );
    }
    return 0;
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
