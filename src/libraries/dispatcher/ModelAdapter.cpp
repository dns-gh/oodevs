// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ModelAdapter.h"
#include "Model_ABC.h"
#include "Agent.h"
#include "Automat.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Object.h"
#include "UrbanObject.h"
#include "Population.h"
#include "Inhabitant.h"
#include "Side.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ModelAdapter constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
ModelAdapter::ModelAdapter( const Model_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter destructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
ModelAdapter::~ModelAdapter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindAgent
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Agent_ABC* ModelAdapter::FindAgent( unsigned int id ) const
{
    return model_.Agents().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetAgent
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Agent_ABC& ModelAdapter::GetAgent( unsigned int id ) const
{
    return model_.Agents().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindAutomat
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Automat_ABC* ModelAdapter::FindAutomat( unsigned int id ) const
{
    return model_.Automats().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetAutomat
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Automat_ABC& ModelAdapter::GetAutomat( unsigned int id ) const
{
    return model_.Automats().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindKnowledgeGroup
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* ModelAdapter::FindKnowledgeGroup( unsigned int id ) const
{
    return model_.KnowledgeGroups().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetKnowledgeGroup
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC& ModelAdapter::GetKnowledgeGroup( unsigned int id ) const
{
    return model_.KnowledgeGroups().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindObject
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Object_ABC* ModelAdapter::FindObject( unsigned int id ) const
{
    return model_.Objects().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetObject
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Object_ABC& ModelAdapter::GetObject( unsigned int id ) const
{
    return model_.Objects().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindPopulation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Population_ABC* ModelAdapter::FindPopulation( unsigned int id ) const
{
    return model_.Populations().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetPopulation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Population_ABC& ModelAdapter::GetPopulation( unsigned int id ) const
{
    return model_.Populations().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindFormation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Formation_ABC* ModelAdapter::FindFormation( unsigned int id ) const
{
    return model_.Formations().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetFormation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Formation_ABC& ModelAdapter::GetFormation( unsigned int id ) const
{
    return model_.Formations().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindTeam
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Team_ABC* ModelAdapter::FindTeam( unsigned int id ) const
{
    return model_.Sides().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetTeam
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Team_ABC& ModelAdapter::GetTeam( unsigned int id ) const
{
    return model_.Sides().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindInhabitant
// Created: FPO 2011-06-01
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* ModelAdapter::FindInhabitant( unsigned int id ) const
{
    return model_.Inhabitants().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetInhabitant
// Created: FPO 2011-06-01
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC& ModelAdapter::GetInhabitant( unsigned int id ) const
{
    return model_.Inhabitants().Get( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindUrbanObject
// Created: LGY 2011-03-01
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* ModelAdapter::FindUrbanObject( unsigned int id ) const
{
    return model_.UrbanBlocks().Find( id );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::GetUrbanObject
// Created: LGY 2011-03-01
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC& ModelAdapter::GetUrbanObject( unsigned int id ) const
{
    return model_.UrbanBlocks().Get( id );
}

kernel::Pathfind_ABC* ModelAdapter::FindPathfind( unsigned int /*id*/ ) const
{
    throw MASA_EXCEPTION( "cannot adapt a dispatcher::Pathfind into a kernel::Pathfind_ABC" );
}

// -----------------------------------------------------------------------------
// Name: ModelAdapter::FindEntity
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
kernel::Entity_ABC* ModelAdapter::FindEntity( unsigned int id ) const
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
