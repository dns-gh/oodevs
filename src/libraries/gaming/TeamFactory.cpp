// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "TeamsModel.h"
#include "ObjectKnowledges.h"
#include "AgentKnowledges.h"
#include "PopulationKnowledges.h"
#include "KnowledgeGroup.h"
#include "Team.h"
#include "Diplomacies.h"
#include "AgentKnowledgeFactory.h"
#include "KnowledgeGroupHierarchies.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InstanciationComplete.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::~TeamFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam( unsigned long id, DIN::DIN_Input& input )
{
    Team* result = new Team( id, input, controllers_.controller_, *this );
    result->Attach( *new ObjectKnowledges( *result, controllers_.controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new Diplomacies( controllers_.controller_, model_.teams_ ) );
    result->Attach< Hierarchies >( *new TeamHierarchies( controllers_.controller_, *result ) );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( unsigned long id, Team_ABC& team  )
{
    KnowledgeGroup_ABC* result = new KnowledgeGroup( id, controllers_.controller_, team );
    result->Attach( *new AgentKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    result->Attach( *new PopulationKnowledges( controllers_.controller_, *result, model_.agentsKnowledgeFactory_ ) );
    result->Attach< Hierarchies >( *new KnowledgeGroupHierarchies( controllers_.controller_, team, *result ) ); // $$$$ AGE 2006-09-20: 
    result->Update( InstanciationComplete() );
    return result;
}
