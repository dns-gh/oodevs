// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "TeamsModel.h"
#include "ObjectKnowledges.h"
#include "AgentKnowledges.h"
#include "PopulationKnowledges.h"
#include "KnowledgeGroup.h"
#include "Team.h"
#include "Diplomacies.h"

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controller& controller, Model& model )
    : controller_( controller )
    , model_( model )
{

}

// -----------------------------------------------------------------------------
// Name: TeamFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::~TeamFactory()
{

}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Team* TeamFactory::CreateTeam( unsigned long id, DIN::DIN_Input& input )
{
    Team* result = new Team( id, input, controller_, *this );
    result->Attach( *new ObjectKnowledges( controller_, model_.objectKnowledgeFactory_ ) );
    result->Attach( *new Diplomacies( controller_, model_.teams_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
KnowledgeGroup* TeamFactory::CreateKnowledgeGroup( unsigned long id )
{
    KnowledgeGroup* result = new KnowledgeGroup( id, controller_ );
    result->Attach( *new AgentKnowledges( controller_, model_.agentsKnowledgeFactory_ ) );
    result->Attach( *new PopulationKnowledges( controller_ ) );
    return result;
}
