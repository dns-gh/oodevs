// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EntityIntelligences.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/HierarchyLevel_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Entity_ABC& holder, kernel::Entity_ABC* superior, const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, superior )
    , holder_( 0 )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Intelligence_ABC& holder, const kernel::Resolver_ABC< kernel::Formation_ABC >& resolver )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, 0 )
    , holder_( &holder )
    , resolver_( resolver )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences destructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::~EntityIntelligences()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::ChangeSuperior
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
void EntityIntelligences::ChangeSuperior( Entity_ABC& superior )
{
    EntityHierarchies< IntelligenceHierarchies >::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::GetSymbol
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
std::string EntityIntelligences::GetSymbol() const
{
    if( holder_ )
        return holder_->GetSymbol();
    return GetEntity().Get< kernel::TacticalHierarchies >().GetSymbol();
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::GetLevel
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
std::string EntityIntelligences::GetLevel() const
{
    if( holder_ )
        return holder_->GetLevel().GetSymbol();
    return GetEntity().Get< kernel::TacticalHierarchies >().GetLevel();
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::DoUpdate
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void EntityIntelligences::DoUpdate( const ASN1T_MsgIntelligenceCreation& message )
{
    SetSuperior( &resolver_.Get( message.intelligence.formation ) );
}
