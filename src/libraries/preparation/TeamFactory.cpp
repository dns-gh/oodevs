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
#include "FormationModel.h"
#include "Team.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamCommunications.h"
#include "Inhabitants.h"
#include "Objects.h"
#include "Populations.h"
#include "clients_gui/Color.h"
#include "clients_gui/NoSideHelpers.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Karma.h"

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( kernel::Controllers& controllers, Model& model, const ::StaticModel& staticModel, tools::IdManager& idManager )
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
kernel::Team_ABC* TeamFactory::CreateTeam()
{
    Team* result = new Team( controllers_, idManager_ );
    result->Attach( *new Objects( controllers_ ) );
    gui::PropertiesDictionary& dico = result->Get< gui::PropertiesDictionary >();
    result->Attach< kernel::Diplomacies_ABC >( *new Diplomacies( controllers_.controller_, model_.GetTeamResolver(), *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::Color_ABC >( *new gui::Color() );
    result->Attach( *new Populations( controllers_ ) );
    result->Attach( *new Inhabitants() );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", staticModel_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team* result = new Team( xis, controllers_, idManager_ );
    result->Attach( *new Objects( controllers_ ) );
    gui::PropertiesDictionary& dico = result->Get< gui::PropertiesDictionary >();
    result->Attach< kernel::Diplomacies_ABC >( *new Diplomacies( xis, controllers_.controller_, model_.GetTeamResolver(), *result, dico, staticModel_.teamKarmas_ ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::Color_ABC >( *new gui::Color( xis ) );
    result->Attach( *new Populations( controllers_ ) );
    result->Attach( *new Inhabitants() );
    result->Attach( *new kernel::DictionaryExtensions( controllers_, "orbat-attributes", xis, staticModel_.extensions_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateNoSideTeam
// Created: JSR 2011-11-10
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateNoSideTeam()
{
    kernel::Team_ABC* result = new gui::NoSideTeam( controllers_.controller_, 0, tools::translate( "TeamFactory", "No side" ) );
    result->Attach( *new Objects( controllers_ ) );
    result->Attach< kernel::Color_ABC >( *new gui::Color() );
    result->Attach< kernel::Diplomacies_ABC >( *new gui::NoSideDiplomacy() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::ConfigureNoSideTeam
// Created: LGY 2013-03-26
// -----------------------------------------------------------------------------
void TeamFactory::ConfigureNoSideTeam( kernel::Team_ABC& team, xml::xistream& xis )
{
    team.Get< kernel::Color_ABC >().ChangeColor( xis );
}
