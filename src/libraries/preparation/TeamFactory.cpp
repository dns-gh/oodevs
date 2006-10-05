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
#include "TeamsModel.h"
#include "Team.h"
#include "KnowledgeGroup.h"
#include "Diplomacies.h"
#include "TeamHierarchies.h"
#include "TeamCommunications.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InstanciationComplete.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
TeamFactory::TeamFactory( Controllers& controllers, Model& model, IdManager& idManager )
    : controllers_( controllers )
    , model_( model )
    , idManager_( idManager )
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
    Team_ABC* result = new Team( controllers_.controller_, *this, idManager_ );
    result->Attach( *new Diplomacies( controllers_.controller_, model_.teams_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateTeam
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::Team_ABC* TeamFactory::CreateTeam( xml::xistream& xis )
{
    Team_ABC* result = new Team( xis, controllers_.controller_, *this, idManager_ );
    result->Attach( *new Diplomacies( controllers_.controller_, model_.teams_, *result ) );
    result->Attach< kernel::TacticalHierarchies >( *new TeamHierarchies( controllers_.controller_, *result, 0 ) );
    result->Attach< kernel::CommunicationHierarchies >( *new TeamCommunications( controllers_.controller_, *result, 0 ) );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( kernel::Team_ABC& team )
{
    KnowledgeGroup_ABC* result = new KnowledgeGroup( controllers_.controller_, team, idManager_ );
    result->Attach< kernel::CommunicationHierarchies >( *new ::CommunicationHierarchies( controllers_.controller_, *result, &team ) );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: TeamFactory::CreateKnowledgeGroup
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* TeamFactory::CreateKnowledgeGroup( xml::xistream& xis, kernel::Team_ABC& team )
{
    KnowledgeGroup_ABC* result = new KnowledgeGroup( xis, controllers_.controller_, team, idManager_ );
    result->Attach< kernel::CommunicationHierarchies >( *new ::CommunicationHierarchies( controllers_.controller_, *result, &team ) );
    result->Update( InstanciationComplete() );
    return result;
}
