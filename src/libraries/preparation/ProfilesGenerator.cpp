// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ProfilesGenerator.h"
#include "Model.h"
#include "ProfilesModel.h"
#include "TeamsModel.h"
#include "FormationModel.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator constructor
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
ProfilesGenerator::ProfilesGenerator( const Model& model, ProfilesModel& profiles )
    : model_( model )
    , profiles_( profiles )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator destructor
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
ProfilesGenerator::~ProfilesGenerator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateTeams
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateTeams( bool readonly )
{
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& entity = it.NextElement();
        GenerateProfile( entity.GetName(), entity, readonly );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateTopLevelFormations
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateTopLevelFormations( bool readonly )
{
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& entity = it.NextElement();
        const TacticalHierarchies& hierarchies = entity.Get< TacticalHierarchies >();
        tools::Iterator< const Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
        while( subIt.HasMoreElements() )
        {
            const Entity_ABC& subEntity = subIt.NextElement();
            GenerateProfile( QString( "%1%2" ).arg( subEntity.GetName() ).arg( subEntity.GetId() ), subEntity, readonly );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateFormations
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateFormations( bool readonly )
{
    tools::Iterator< const Formation_ABC& > it = model_.GetFormationResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& entity = it.NextElement();
        GenerateProfile( QString( "%1%2" ).arg( entity.GetName() ).arg( entity.GetId() ), entity, readonly );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateProfile
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateProfile( const QString& name, const kernel::Entity_ABC& entity, bool readonly )
{
    profiles_.CreateProfile( name, entity, readonly );
}
