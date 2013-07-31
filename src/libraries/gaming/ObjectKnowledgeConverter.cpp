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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
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
// Name: ObjectKnowledgeConverter::FindInKnowledgeGroups
// Created: JSR 2013-07-31
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::FindInKnowledgeGroups( const kernel::Entity_ABC* key, unsigned int id ) const
{
    auto it = knowledges_.find( key );
    if( it != knowledges_.end() )
    {
        const T_KnowledgeMap& knowledges = it->second;
        for( auto it = knowledges.begin(); it != knowledges.end(); ++it )
            if( it->second->GetId() == id )
                return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::FindInKnowledgeGroups
// Created: JSR 2013-07-31
// -----------------------------------------------------------------------------
const kernel::ObjectKnowledge_ABC* ObjectKnowledgeConverter::FindInKnowledgeGroups( const kernel::Entity_ABC* key, const kernel::Object_ABC* object ) const
{
    auto it = knowledges_.find( key );
    if( it != knowledges_.end() )
    {
        auto kit = it->second.find( object );
        if( kit != it->second.end() )
            return kit->second;
    }
    return 0;
}

namespace
{
    const kernel::Entity_ABC* GetKnowledgeGroup( const kernel::Entity_ABC& owner, const kernel::CommunicationHierarchies& h )
    {
        if( owner.GetTypeName() == kernel::Agent_ABC::typeName_ )
        {
            if( h.CanCommunicate() )
                return &h.GetUp( 2 );
            else
                return &h.GetUp();
        }
        else if( owner.GetTypeName() == kernel::Automat_ABC::typeName_ )
            return &h.GetUp();
        else if( owner.GetTypeName() == kernel::KnowledgeGroup_ABC::typeName_ )
            return &owner;
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    const kernel::CommunicationHierarchies* h = owner.Retrieve< kernel::CommunicationHierarchies >();
    if( !h )
        return 0;
    const kernel::Entity_ABC* team = &h->GetTop();
    const kernel::Entity_ABC* kg = GetKnowledgeGroup( owner, *h );
    const ObjectKnowledge_ABC* ret = 0;
    if( kg )
        ret = FindInKnowledgeGroups( kg, id );
    if( !ret )
        ret = FindInKnowledgeGroups( team, id );
    return ret;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const ObjectKnowledge_ABC& base, const kernel::Entity_ABC& owner ) const
{
    const Object_ABC* real = base.GetEntity();
    return real ? Find( *real, owner ) : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledgeConverter::Find
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
const ObjectKnowledge_ABC* ObjectKnowledgeConverter::Find( const kernel::Object_ABC& base, const kernel::Entity_ABC& owner ) const
{
    const kernel::CommunicationHierarchies* h = owner.Retrieve< kernel::CommunicationHierarchies >();
    if( !h )
        return 0;
    const kernel::Entity_ABC* team = &h->GetTop();
    const kernel::Entity_ABC* kg = GetKnowledgeGroup( owner, *h );
    const ObjectKnowledge_ABC* ret = 0;
    if( kg )
        ret = FindInKnowledgeGroups( kg, &base );
    if( !ret )
        ret = FindInKnowledgeGroups( team, &base );
    return ret;
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
