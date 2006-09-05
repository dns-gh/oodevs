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
#include "clients_gui/Tools.h"

using namespace kernel;

unsigned long Team::idManager_ = 1;

// -----------------------------------------------------------------------------
// Name: Team constructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::Team( Controller& controller, KnowledgeGroupFactory_ABC& factory )
    : controller_( controller )
    , factory_( factory )
    , id_( idManager_++ )
{
    name_ = tools::translate( "Preparation", "Armée %1" ).arg( id_ );
    controller_.Create( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team destructor
// Created: SBO 2006-08-29
// -----------------------------------------------------------------------------
Team::~Team()
{
    DeleteAll();
    controller_.Delete( *(Team_ABC*)this );
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
    KnowledgeGroup_ABC* group = factory_.CreateKnowledgeGroup( *this );
    Resolver< KnowledgeGroup_ABC >::Register( group->GetId(), *group );
    controller_.Update( *(Team_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Team::Rename
// Created: SBO 2006-08-30
// -----------------------------------------------------------------------------
void Team::Rename( const QString& name )
{
    name_ = name;
    controller_.Update( *(Team_ABC*)this );
}
