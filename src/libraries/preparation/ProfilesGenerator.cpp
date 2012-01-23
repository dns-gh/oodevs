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
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

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
// Name: ProfilesGenerator::GenerateANIBAS
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateANIBAS()
{
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Team_ABC& team = it.NextElement();
        if( team.Get< Diplomacies_ABC >().GetKarma() == Karma::friend_ )
            GenerateLowLevelFormations( team, "anibas" );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateENIEX
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateENIEX()
{
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Team_ABC& team = it.NextElement();
        if( team.Get< Diplomacies_ABC >().GetKarma() == Karma::enemy_ )
            GenerateLowLevelFormations( team, "eniex" );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateSUPERVISOR
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateSUPERVISOR()
{
    GenerateSUPERVISOR( "Superviseur", "supervisor" );
}

namespace
{
    QString GetEntityName( const Entity_ABC& entity )
    {
        std::string longName = gui::LongNameHelper::GetEntityLongName( entity );
        if( longName.empty() )
            return entity.GetName();
        return longName.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateSIDESUPERVISOR
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateSIDESUPERVISOR()
{
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& entity = it.NextElement();

        profiles_.CreateProfile( "Superviseur " + GetEntityName( entity ), "supervisor", entity, true );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateDIREX
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateDIREX()
{
    GenerateSUPERVISOR( "Direx", "direx" );
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateANALYSIS
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateANALYSIS()
{
    GenerateSUPERVISOR( "Analyste", "analysis" );
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateGESTIM
// Created: LGY 2012-01-23
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateGESTIM()
{
    GenerateSUPERVISOR( "Gestim", "service-gestim" );
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateSUPERVISOR
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateSUPERVISOR( const QString& name, const std::string& userRole )
{
    std::vector< const Entity_ABC* > entities;
    tools::Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Entity_ABC& entity = it.NextElement();
        entities.push_back( &entity );
    }
    profiles_.CreateProfile( name, userRole, entities, true );
}

// -----------------------------------------------------------------------------
// Name: ProfilesGenerator::GenerateLowLevelFormations
// Created: JSR 2011-12-08
// -----------------------------------------------------------------------------
void ProfilesGenerator::GenerateLowLevelFormations( const Entity_ABC& entity, const std::string& userRole )
{
    const TacticalHierarchies& hierarchies = entity.Get< TacticalHierarchies >();
    tools::Iterator< const Entity_ABC& > subIt = hierarchies.CreateSubordinateIterator();
    std::vector< const Entity_ABC* > automats;
    while( subIt.HasMoreElements() )
    {
        const Entity_ABC& subEntity = subIt.NextElement();
        QString typeName = subEntity.GetTypeName();
        if( typeName == Automat_ABC::typeName_ )
            automats.push_back( &subEntity );
        else if( typeName == Formation_ABC::typeName_ )
            GenerateLowLevelFormations( subEntity, userRole );
    }
    if( !automats.empty() )
        profiles_.CreateProfile( GetEntityName( entity), userRole, automats, false );
}
