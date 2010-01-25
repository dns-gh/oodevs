// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "gaming/KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::KnowledgeGroupHierarchies( kernel::Controller& controller, kernel::Entity_ABC* superior, kernel::KnowledgeGroup_ABC& holder
                                                    , tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver )
    : kernel::EntityHierarchies< kernel::CommunicationHierarchies >( controller, holder, superior )
    , resolver_( resolver )
    , controller_( controller )
{
    // NOTHING   
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::~KnowledgeGroupHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::DoUpdate
// Created: FHD 2009-12-21
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchies::DoUpdate( const ASN1T_MsgKnowledgeGroupChangeSuperior& message )
{
    if( message.oid_knowledgegroup_parent )
    {
        if( kernel::KnowledgeGroup_ABC* parent = resolver_.Find( message.oid_knowledgegroup_parent ) )
            ChangeSuperior( parent );
    }
    else if( message.oid_camp )
    {
        if( const kernel::Entity_ABC* top = &resolver_.Get( message.oid ).Retrieve< kernel::CommunicationHierarchies >()->GetTop() )
            ChangeSuperior( const_cast< kernel::Entity_ABC* >( top ) );
    }
}
