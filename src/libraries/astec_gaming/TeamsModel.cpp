// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "TeamsModel.h"
#include "DIN/DIN_Input.h"
#include "astec_kernel/Team_ABC.h"
#include "TeamFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: TeamsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
TeamsModel::TeamsModel( TeamFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
TeamsModel::~TeamsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void TeamsModel::Purge()
{
    Resolver< Team_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam( DIN::DIN_Input& input )
{
    unsigned long id;
    input >> id;
    if( ! Resolver< Team_ABC >::Find( id ) )
    {
        Team_ABC* team = factory_.CreateTeam( id, input );
        Resolver< Team_ABC >::Register( id, *team );
    }
}
 
// -----------------------------------------------------------------------------
// Name: TeamsModel::GetTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Team_ABC& TeamsModel::GetTeam( unsigned long id )
{
    return Resolver< Team_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( const std::string& team )
{
    for( Resolver< Team_ABC >::CIT_Elements it = Resolver< Team_ABC >::elements_.begin(); it != Resolver< Team_ABC >::elements_.end(); ++it )
        if( it->second->GetName() == team )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup* TeamsModel::FindKnowledgeGroup( const unsigned long& id ) const
{
    for( Resolver< Team_ABC >::CIT_Elements it = Resolver< Team_ABC >::elements_.begin(); it != Resolver< Team_ABC >::elements_.end(); ++it )
    {
        Resolver_ABC< KnowledgeGroup >& team = *it->second;
        KnowledgeGroup* group = team.Find( id );
        if( group )
            return group;
    }
    return 0;
}
