// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamsModel.h"
#include "Team.h"
#include "TeamFactory_ABC.h"
#include "clients_gui/Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamsModel constructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::TeamsModel( TeamFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamsModel destructor
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
TeamsModel::~TeamsModel()
{
    // NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: TeamsModel::Purge
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::Purge()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateTeam
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::CreateTeam()
{
    Team_ABC* team = factory_.CreateTeam( GenerateTeamName() );
    Register( team->GetName(), *team );
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void TeamsModel::CreateKnowledgeGroup( kernel::Team_ABC& team )
{
    static_cast< Team& >( team ).CreateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
bool TeamsModel::Rename( kernel::Team_ABC& team, const QString& name )
{
    if( !FindTeam( name ) )
    {
        Remove( team.GetName() );
        static_cast< Team& >( team ).Rename( name );
        Register( team.GetName(), team );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindTeam
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Team_ABC* TeamsModel::FindTeam( const QString& team ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->GetName() == team )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::FindKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC* TeamsModel::FindKnowledgeGroup( const unsigned long& id ) const
{
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        Resolver_ABC< KnowledgeGroup_ABC >& team = *it->second;
        KnowledgeGroup_ABC* group = team.Find( id );
        if( group )
            return group;
    }
    return 0;
}
// -----------------------------------------------------------------------------
// Name: TeamsModel::GenerateTeamName
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
QString TeamsModel::GenerateTeamName() const
{
    QString name = tools::translate( "Preparation", "Armée %1" );
    unsigned int i = 1;
    for( ; FindTeam( name.arg( i ) ); ++i )
        ;
    return name.arg( i );
}
