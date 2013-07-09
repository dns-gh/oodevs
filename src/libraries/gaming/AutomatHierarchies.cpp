// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "protocol/Simulation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( Controller& controller, Entity_ABC& holder, const tools::Resolver_ABC< KnowledgeGroup_ABC >& groupResolver, PropertiesDictionary& dictionary )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, 0 )
    , groupResolver_( groupResolver )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const sword::AutomatCreation& message )
{
    SetSuperior( &groupResolver_.Get( message.knowledge_group().id() ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const sword::AutomatChangeKnowledgeGroup& message )
{
    ChangeSuperior( & groupResolver_.Get( message.knowledge_group().id() ) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::AddSuperiorToDictionary
// Created: SBO 2006-10-25
// -----------------------------------------------------------------------------
void AutomatHierarchies::AddSuperiorToDictionary( PropertiesDictionary& dico, Entity_ABC* const& superior ) const
{
    dico.Register( *(const CommunicationHierarchies*)this, tools::translate( "AutomatHierarchies", "Hierarchies/Superior" ), superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool AutomatHierarchies::CanCommunicate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool AutomatHierarchies::IsJammed() const
{
    return false;
}
