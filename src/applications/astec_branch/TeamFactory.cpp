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
#include "ObjectKnowledges.h"
#include "Team.h"

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
    Team* result = new Team( id, input );
    result->Attach( *new ObjectKnowledges( controller_, model_.objectKnowledgeFactory_ ) );
    return result;
}
