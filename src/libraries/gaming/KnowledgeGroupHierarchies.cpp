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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::KnowledgeGroupHierarchies( Controller& controller, Entity_ABC* superior, KnowledgeGroup_ABC& holder
                                                    , tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, superior )
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
void KnowledgeGroupHierarchies::DoUpdate( const ASN1T_MsgKnowledgeGroupUpdate& message )
{
    if( message.m.oid_knowledgegroup_parentPresent )
        ChangeSuperior( &resolver_.Get( message.oid_knowledgegroup_parent ) );
    resolver_.Get( message.oid ).DoUpdate( message );
}

