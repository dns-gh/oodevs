// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "KnowledgeGroupsModel.h"
#include "TeamsModel.h"

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroupsModel::KnowledgeGroupsModel( TeamsModel& teams )
    : teams_( teams )
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
// Name: KnowledgeGroupsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void KnowledgeGroupsModel::Purge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Find
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup* KnowledgeGroupsModel::Find( const unsigned long& identifier ) const
{
    return teams_.FindKnowledgeGroup( identifier );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupsModel::Get
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup& KnowledgeGroupsModel::Get( const unsigned long& identifier ) const
{
    KnowledgeGroup* group = Find( identifier );
    if( ! group )
        throw std::runtime_error( "KnowledgeGroup not found" );
    return *group;
}
