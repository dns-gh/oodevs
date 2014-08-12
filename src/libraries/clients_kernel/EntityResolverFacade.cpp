// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "EntityResolverFacade.h"
#include "Model_ABC.h"
#include "Agent_ABC.h"
#include "Automat_ABC.h"
#include "Inhabitant_ABC.h"
#include "KnowledgeGroup_ABC.h"
#include "Object_ABC.h"
#include "Population_ABC.h"
#include "Formation_ABC.h"
#include "Team_ABC.h"
#include "UrbanObject_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade constructor
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
EntityResolverFacade::EntityResolverFacade( Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade destructor
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
EntityResolverFacade::~EntityResolverFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindAgent
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Agent_ABC* EntityResolverFacade::FindAgent( unsigned int id ) const
{
    return model_.GetAgentResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetAgent
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Agent_ABC& EntityResolverFacade::GetAgent( unsigned int id ) const
{
    return model_.GetAgentResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindAutomat
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Automat_ABC* EntityResolverFacade::FindAutomat( unsigned int id ) const
{
    return model_.GetAutomatResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetAutomat
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Automat_ABC& EntityResolverFacade::GetAutomat( unsigned int id ) const
{
    return model_.GetAutomatResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindKnowledgeGroup
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC* EntityResolverFacade::FindKnowledgeGroup( unsigned int id ) const
{
    return model_.GetKnowledgeGroupResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetKnowledgeGroup
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC& EntityResolverFacade::GetKnowledgeGroup( unsigned int id ) const
{
    return model_.GetKnowledgeGroupResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Object_ABC* EntityResolverFacade::FindObject( unsigned int id ) const
{
    return model_.GetObjectResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Object_ABC& EntityResolverFacade::GetObject( unsigned int id ) const
{
    return model_.GetObjectResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindPopulation
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Population_ABC* EntityResolverFacade::FindPopulation( unsigned int id ) const
{
    return model_.GetPopulationResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetPopulation
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Population_ABC& EntityResolverFacade::GetPopulation( unsigned int id ) const
{
    return model_.GetPopulationResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindInhabitant
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Inhabitant_ABC* EntityResolverFacade::FindInhabitant( unsigned int id ) const
{
    return model_.GetInhabitantResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetInhabitant
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Inhabitant_ABC& EntityResolverFacade::GetInhabitant( unsigned int id ) const
{
    return model_.GetInhabitantResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindFormation
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Formation_ABC* EntityResolverFacade::FindFormation( unsigned int id ) const
{
    return model_.GetFormationResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetFormation
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Formation_ABC& EntityResolverFacade::GetFormation( unsigned int id ) const
{
    return model_.GetFormationResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindTeam
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Team_ABC* EntityResolverFacade::FindTeam( unsigned int id ) const
{
    return model_.GetTeamResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetTeam
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
Team_ABC& EntityResolverFacade::GetTeam( unsigned int id ) const
{
    return model_.GetTeamResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindUrbanObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
UrbanObject_ABC* EntityResolverFacade::FindUrbanObject( unsigned int id ) const
{
    return model_.GetUrbanObjectResolver().Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetUrbanObject
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
UrbanObject_ABC& EntityResolverFacade::GetUrbanObject( unsigned int id ) const
{
    return model_.GetUrbanObjectResolver().Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindEntity
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
kernel::Entity_ABC* EntityResolverFacade::FindEntity( unsigned int id ) const
{
    if( id == 0 )
        return 0;
    kernel::Entity_ABC* entity = FindAgent( id );
    if( !entity )
        entity = FindAutomat( id );
    if( !entity )
        entity = FindInhabitant( id );
    if( !entity )
        entity = FindKnowledgeGroup( id );
    if( !entity )
        entity = FindObject( id );
    if( !entity )
        entity = FindPopulation( id );
    if( !entity )
        entity = FindFormation( id );
    if( !entity )
        entity = FindTeam( id );
    if( !entity )
        entity = FindUrbanObject( id );
    return entity;
}
