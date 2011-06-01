// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EntityResolverFacade.h"
#include "AgentsModel.h"
#include "KnowledgeGroupsModel.h"
#include "Model.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "ObjectsModel.h"
#include "UrbanModel.h"
#include "TeamsModel.h"

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade constructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
EntityResolverFacade::EntityResolverFacade( Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade destructor
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
EntityResolverFacade::~EntityResolverFacade()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindAgent
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Agent_ABC* EntityResolverFacade::FindAgent( unsigned int id ) const
{
    return model_.agents_.FindAgent( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetAgent
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Agent_ABC& EntityResolverFacade::GetAgent( unsigned int id ) const
{
    return model_.agents_.GetAgent( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindAutomat
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Automat_ABC* EntityResolverFacade::FindAutomat( unsigned int id ) const
{
    return model_.agents_.FindAutomat( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetAutomat
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Automat_ABC& EntityResolverFacade::GetAutomat( unsigned int id ) const
{
    return model_.agents_.GetAutomat( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindKnowledgeGroup
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC* EntityResolverFacade::FindKnowledgeGroup( unsigned int id ) const
{
    return model_.knowledgeGroups_.Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetKnowledgeGroup
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::KnowledgeGroup_ABC& EntityResolverFacade::GetKnowledgeGroup( unsigned int id ) const
{
    return model_.knowledgeGroups_.Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindObject
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Object_ABC* EntityResolverFacade::FindObject( unsigned int id ) const
{
    return model_.objects_.FindObject( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetObject
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Object_ABC& EntityResolverFacade::GetObject( unsigned int id ) const
{
    return model_.objects_.GetObject( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindUrbanObject
// Created: LGY 2011-03-01
// -----------------------------------------------------------------------------
kernel::Object_ABC* EntityResolverFacade::FindUrbanObject( unsigned int id ) const
{
    return model_.urbanObjects_.FindObject( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetUrbanObject
// Created: LGY 2011-03-01
// -----------------------------------------------------------------------------
kernel::Object_ABC& EntityResolverFacade::GetUrbanObject( unsigned int id ) const
{

    return model_.urbanObjects_.GetObject( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindPopulation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Population_ABC* EntityResolverFacade::FindPopulation( unsigned int id ) const
{
    return model_.agents_.FindPopulation( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetPopulation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Population_ABC& EntityResolverFacade::GetPopulation( unsigned int id ) const
{
    return model_.agents_.GetPopulation( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindInhabitant
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC* EntityResolverFacade::FindInhabitant( unsigned int id ) const
{
    return model_.agents_.FindInhabitant( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetInhabitant
// Created: FPO 2011-05-30
// -----------------------------------------------------------------------------
kernel::Inhabitant_ABC& EntityResolverFacade::GetInhabitant( unsigned int id ) const
{
    return model_.agents_.GetInhabitant( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindFormation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Formation_ABC* EntityResolverFacade::FindFormation( unsigned int id ) const
{
    return model_.teams_.tools::Resolver< kernel::Formation_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetFormation
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Formation_ABC& EntityResolverFacade::GetFormation( unsigned int id ) const
{
    return model_.teams_.tools::Resolver< kernel::Formation_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::FindTeam
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Team_ABC* EntityResolverFacade::FindTeam( unsigned int id ) const
{
    return model_.teams_.tools::Resolver< kernel::Team_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: EntityResolverFacade::GetTeam
// Created: SBO 2010-05-10
// -----------------------------------------------------------------------------
kernel::Team_ABC& EntityResolverFacade::GetTeam( unsigned int id ) const
{
    return model_.teams_.tools::Resolver< kernel::Team_ABC >::Get( id );
}
