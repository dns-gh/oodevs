// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamFactory.h"
#include "Model.h"
#include "StaticModel.h"
#include "TeamsModel.h"
#include "Team.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamCommunications.h"
#include "Inhabitants.h"
#include "Objects.h"
#include "Populations.h"
#include "EntityIntelligences.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model, const StaticModel& staticModel, IdManager& idManager )
    : controllers_( controllers )
    , model_      ( model )
    , staticModel_( staticModel )
    , idManager_  ( idManager )
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
Team_ABC* TeamFactory::CreateTeam()
{
    Team* result = new Team( controllers_.controller_, idManager_ );
    result->Attach( *new Objects() );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Diplomacies_ABC >( *new Diplomacies( controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Attach( *new Inhabitants() );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", staticModel_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team* result = new Team( xis, controllers_.controller_, idManager_ );
    result->Attach( *new Objects() );
    PropertiesDictionary& dico = result->Get< PropertiesDictionary >();
    result->Attach< Diplomacies_ABC >( *new Diplomacies( xis, controllers_.controller_, model_.teams_, *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< IntelligenceHierarchies >( *new EntityIntelligences( controllers_.controller_, *result, 0 ) );
    result->Attach( *new Populations() );
    result->Attach( *new Inhabitants() );
    result->Attach( *new DictionaryExtensions( controllers_, "orbat-attributes", xis, staticModel_.extensions_ ) );
    result->Polish();
    return result;
}
