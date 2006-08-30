// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Team.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( const QString& name, Controller& controller, KnowledgeGroupFactory_ABC& factory )
    : controller_( controller )
    , factory_( factory )
    , name_( name )
    , id_( 0 )
{
    controller_.Create( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    controller_.Delete( *(Team_ABC*)this );
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Team::GetName
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
QString Team::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Team::GetId
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
unsigned long Team::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Team::CreateKnowledgeGroup
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::CreateKnowledgeGroup()
{
    unsigned long id = GenerateKnowledgeGroupId();
    KnowledgeGroup_ABC* group = factory_.CreateKnowledgeGroup( id, *this );
    Resolver< KnowledgeGroup_ABC >::Register( id, *group );
    controller_.Update( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
}

// -----------------------------------------------------------------------------
// Name: Team::GenerateKnowledgeGroupId
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
unsigned long Team::GenerateKnowledgeGroupId() const
{
    unsigned long i = 1;
    for( ; Resolver< KnowledgeGroup_ABC >::Find( i ); ++i )
        ;
    return i;
}
