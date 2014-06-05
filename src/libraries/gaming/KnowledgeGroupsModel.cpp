// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupsModel.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel( KnowledgeGroupFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::~KnowledgeGroupsModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Create
// Created: HBD 2010-06-23
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Create( const sword::KnowledgeGroupCreation& message )
{
    std::unique_ptr< kernel::KnowledgeGroup_ABC > group( factory_.CreateKnowledgeGroup( message ) );
    Register( group->GetId(), *group );
    group.release();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Delete
// Created: HBD 2010-06-23
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Delete( unsigned int id )
{
    delete Find( id );
    Remove( id );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::FindCrowdKnowledgeGroup
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* KnowledgeGroupsModel::FindCrowdKnowledgeGroup( const kernel::Team_ABC& team ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        kernel::KnowledgeGroup_ABC* knowledgeGroup = it->second;
        if( knowledgeGroup && knowledgeGroup->IsCrowd() )
            if( const kernel::CommunicationHierarchies* h = knowledgeGroup->Retrieve< kernel::CommunicationHierarchies >() )
                if( h->GetSuperior() == &team )
                    return knowledgeGroup;
    }
    return 0;
}
