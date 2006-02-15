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
#include "ObjectFactory_ABC.h"
#include "AgentFactory_ABC.h"
#include "Agent.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "Agent.h"
#include "Population.h"
#include "Object_ABC.h"
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
    Resolver< KnowledgeGroup >::DeleteAll(); 
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
//    application_.NotifyTeamCreated( *team );
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
// Name: TeamsModel::CreateKnowledgeGroup
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void TeamsModel::CreateKnowledgeGroup( DIN::DIN_Input& input )
{
    unsigned long id, teamId;
    input >> id >> teamId;
    if( ! Resolver< KnowledgeGroup >::Find( id ) )
    {
        Team& team = GetTeam( teamId );
        KnowledgeGroup* gtia = team.CreateKnowledgeGroup( id );
        Resolver< KnowledgeGroup >::Register( id, *gtia );
//    application_.NotifyKnowledgeGroupCreated( *gtia );
    }
}

// -----------------------------------------------------------------------------
// Name: TeamsModel::GetKnowledgeGroup
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
KnowledgeGroup& TeamsModel::GetKnowledgeGroup( unsigned long id )
{
    return Resolver< KnowledgeGroup >::Get( id );
}
