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
    {
        if( message.oid_knowledgegroup_parent != 0 )
            ChangeSuperior( &resolver_.Get( message.oid_knowledgegroup_parent ) );    
        else
        {
            const kernel::Entity_ABC* top = &resolver_.Get( message.oid ).Retrieve< kernel::CommunicationHierarchies >()->GetTop();
            ChangeSuperior( const_cast< kernel::Entity_ABC* >( top ) );
        }
    }
    if( const kernel::CommunicationHierarchies* hierarchies = resolver_.Get( message.oid ).Retrieve< kernel::CommunicationHierarchies >() )
    {
        KnowledgeGroup *kg = (KnowledgeGroup *)&hierarchies->GetEntity();
        if( kg->IsActivated() != message.enabled )
            kg->SetActivated( message.enabled ? true:false );
    }
    
    resolver_.Get( message.oid ).DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::DoUpdate
// Created: FHD 2009-12-29
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchies::DoUpdate( const ASN1T_MsgKnowledgeGroupDelete& deleteMessage )
{
    if( kernel::CommunicationHierarchies* hierarchies = resolver_.Get( deleteMessage.oid ).Retrieve< kernel::CommunicationHierarchies >() )
    {
        // all child automats and knowledge groups must be moved one level up
        kernel::CommunicationHierarchies* suphierarchies = 0;
        const Entity_ABC *sup = hierarchies->GetSuperior();
        const Entity_ABC& topHierarchy = hierarchies->GetTop();
        if( sup == &topHierarchy )
            suphierarchies = const_cast< kernel::CommunicationHierarchies* >( sup->Retrieve< kernel::CommunicationHierarchies >() );
        else if( sup )
            suphierarchies = resolver_.Get( sup->GetId() ).Retrieve< kernel::CommunicationHierarchies >();
        if( suphierarchies )
        {
            tools::Iterator< const Entity_ABC& > children = CreateSubordinateIterator();
            while( children.HasMoreElements() )
            {
                const Entity_ABC& entity = children.NextElement();
                hierarchies->RemoveSubordinate( entity );
                suphierarchies->AddSubordinate( const_cast< Entity_ABC& >( entity ) );
            }
            suphierarchies->RemoveSubordinate( hierarchies->GetEntity() );
        }
    }
}

