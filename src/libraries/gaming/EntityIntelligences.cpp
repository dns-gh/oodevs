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
#include "clients_kernel/SymbolFactory.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Entity_ABC& holder, kernel::Entity_ABC* superior, const tools::Resolver_ABC< kernel::Formation_ABC >& resolver )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, superior )
    , controller_       ( controller )
    , holder_           ( 0 )
    , resolver_         ( resolver )
    , holderLevelSymbol_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences constructor
// Created: SBO 2007-10-18
// -----------------------------------------------------------------------------
EntityIntelligences::EntityIntelligences( Controller& controller, Intelligence_ABC& holder, const tools::Resolver_ABC< kernel::Formation_ABC >& resolver, kernel::SymbolFactory& factory )
    : EntityHierarchies< IntelligenceHierarchies >( controller, holder, 0 )
    , controller_       ( controller )
    , holder_           ( &holder )
    , resolver_         ( resolver )
    , holderLevelSymbol_( factory.CreateLevelSymbol( holder_->GetLevel() ) )
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
        return holderLevelSymbol_;
    return GetEntity().Get< kernel::TacticalHierarchies >().GetLevel();
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::DoUpdate
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
void EntityIntelligences::DoUpdate( const sword::IntelligenceCreation& message )
{
    SetSuperior( &resolver_.Get( message.intelligence().formation().id() ) );
}

// -----------------------------------------------------------------------------
// Name: EntityIntelligences::UpdateSymbol
// Created: SBO 2007-11-02
// -----------------------------------------------------------------------------
void EntityIntelligences::UpdateSymbol( bool /*up = true*/ )
{
    controller_.Update( *this );
    controller_.Update( *(Symbol_ABC*)this );
}
