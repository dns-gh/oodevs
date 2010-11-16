// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanKnowledgeConverter.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/UrbanKnowledge_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter constructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
UrbanKnowledgeConverter::UrbanKnowledgeConverter( Controllers& controller )
    : controller_( controller )
{
    controller_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter destructor
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
UrbanKnowledgeConverter::~UrbanKnowledgeConverter()
{
    controller_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::Find
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( unsigned long id, const kernel::Team_ABC& owner ) const
{
    CIT_Knowledges it = knowledges_.find( &owner );
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
// Name: UrbanKnowledgeConverter::Find
// Created: SBO 2007-05-24
// -----------------------------------------------------------------------------
const UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( unsigned long id, const kernel::Entity_ABC& owner ) const
{
    return Find( id, static_cast< const kernel::Team_ABC& >( owner.Get< kernel::CommunicationHierarchies >().GetTop() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::Find
// Created: MGD 2010-11-16
// -----------------------------------------------------------------------------
const kernel::UrbanKnowledge_ABC* UrbanKnowledgeConverter::Find( const kernel::Entity_ABC& base, const kernel::Team_ABC& owner ) const
{
    CIT_Knowledges it = knowledges_.find( &owner );
    if( it != knowledges_.end() )
    {
        CIT_KnowledgeMap itFind = it->second.find( &base );
        if( itFind != it->second.end() )
            return itFind->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::NotifyCreated
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void UrbanKnowledgeConverter::NotifyCreated( const UrbanKnowledge_ABC& k )
{
    knowledges_[ & k.GetOwner() ][ k.GetEntity() ] = &k;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-09-15
// -----------------------------------------------------------------------------
void UrbanKnowledgeConverter::NotifyDeleted( const UrbanKnowledge_ABC& k )
{
   knowledges_[ & k.GetOwner() ].erase( k.GetEntity() );
}
