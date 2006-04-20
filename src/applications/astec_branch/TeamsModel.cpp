// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TeamsModel.h"
#include "DIN/DIN_Input.h"
#include "Team.h"
#include "App.h"
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
    Resolver< Team >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam( DIN::DIN_Input& input )
{
    unsigned long id;
    input >> id;
    if( ! Resolver< Team >::Find( id ) )
    {
        Team* team = factory_.CreateTeam( id, input );
        Resolver< Team >::Register( id, *team );
    }
}
 
// -----------------------------------------------------------------------------
// Name: TeamsModel::GetTeam
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Team& TeamsModel::GetTeam( unsigned long id )
{
    return Resolver< Team >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team* TeamsModel::FindTeam( const std::string& team )
{
    for( Resolver< Team >::CIT_Elements it = Resolver< Team >::elements_.begin(); it != Resolver< Team >::elements_.end(); ++it )
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
    for( Resolver< Team >::CIT_Elements it = Resolver< Team >::elements_.begin(); it != Resolver< Team >::elements_.end(); ++it )
    {
        Resolver_ABC< KnowledgeGroup >& team = *it->second;
        KnowledgeGroup* group = team.Find( id );
        if( group )
            return group;
    }
    return 0;
}
