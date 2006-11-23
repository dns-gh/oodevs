// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "MergingTacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
MergingTacticalHierarchies::MergingTacticalHierarchies( Controller& controller, Entity_ABC& entity, Entity_ABC* superior )
     : EntityHierarchies< TacticalHierarchies >( controller, entity, superior )
     , controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies destructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
MergingTacticalHierarchies::~MergingTacticalHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
std::string MergingTacticalHierarchies::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UpdateSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UpdateSymbol()
{
    const std::string oldSymbol = symbol_;
    symbol_.clear();
    Iterator< const Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
        MergeSymbol( it.NextElement() );
    if( symbol_ != oldSymbol )
        controller_.Update( *(Symbol_ABC*)this );
    if( TacticalHierarchies* superior = SuperiorHierarchy() )
        superior->UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::RegisterSubordinate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::RegisterSubordinate(       kernel::Entity_ABC& entity )
{
    EntityHierarchies< TacticalHierarchies >::RegisterSubordinate( entity );
    MergeSymbol( entity );
    if( TacticalHierarchies* superior = SuperiorHierarchy() )
        superior->UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::UnregisterSubordinate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::UnregisterSubordinate( const kernel::Entity_ABC& entity )
{
    EntityHierarchies< TacticalHierarchies >::UnregisterSubordinate( entity );
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: MergingTacticalHierarchies::MergeSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void MergingTacticalHierarchies::MergeSymbol( const kernel::Entity_ABC& entity )
{
    const std::string newChildSymbol = entity.Get< TacticalHierarchies >().GetSymbol();
    if( symbol_.empty() )
        symbol_ = newChildSymbol;
    else
    {
        unsigned i = 0;
        const unsigned max = std::min( symbol_.length(), newChildSymbol.length() );
        while( i < max && symbol_[ i ] == newChildSymbol[ i ] )
            ++i;
        symbol_ = symbol_.substr( 0, i );
    }
}
