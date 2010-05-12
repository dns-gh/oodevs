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
#include "Model.h"
#include "Agent.h"
#include "Automat.h"
#include "Formation.h"
#include "KnowledgeGroup.h"
#include "Object.h"
#include "Population.h"
#include "Side.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ModelAdapter constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
ModelAdapter::ModelAdapter( const Model& model )
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
