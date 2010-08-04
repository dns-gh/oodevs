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
#include "KnowledgeGroup.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::KnowledgeGroupHierarchies( kernel::Controller& controller, kernel::Entity_ABC* superior, kernel::KnowledgeGroup_ABC& holder
                                                    , tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& resolver, bool jam )
    : kernel::EntityHierarchies< kernel::CommunicationHierarchies >( controller, holder, superior )
    , resolver_( resolver )
    , controller_( controller )
    , jam_ ( jam )
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
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchies::DoUpdate( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message )
{
    if( message.has_oid_parent() )
    {
        if( message.oid_parent() != 0 )
            ChangeSuperior( &resolver_.Get( message.oid_parent() ) );
        else
        {
            const kernel::Entity_ABC* top = &resolver_.Get( message.oid() ).Retrieve< kernel::CommunicationHierarchies >()->GetTop();
            ChangeSuperior( const_cast< kernel::Entity_ABC* >( top ) );
        }
    }
    else if( message.has_oid_camp() )
    {
        if( const kernel::Entity_ABC* top = &resolver_.Get( message.oid() ).Retrieve< kernel::CommunicationHierarchies >()->GetTop() )
            ChangeSuperior( const_cast< kernel::Entity_ABC* >( top ) );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool KnowledgeGroupHierarchies::CanCommunicate() const
{   
    return true;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool KnowledgeGroupHierarchies::IsJammed() const
{
    return jam_;
}
