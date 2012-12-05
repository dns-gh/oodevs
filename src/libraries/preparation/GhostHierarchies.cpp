// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "GhostHierarchies.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Karma.h"

// -----------------------------------------------------------------------------
// Name: GhostHierarchies constructor
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
GhostHierarchies::GhostHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const std::string& level, const std::string& symbol, kernel::Entity_ABC* superior )
    : TacticalHierarchies( controller, holder, 0 )
    , level_        ( level )
    , baseSymbol_   ( symbol )
    , symbol_       ( symbol )
    , superior_     ( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies destructor
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
GhostHierarchies::~GhostHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::SerializeAttributes
// Created: ABR 2012-06-28
// -----------------------------------------------------------------------------
void GhostHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const kernel::Entity_ABC* child = it->second;
        if( dynamic_cast< const kernel::Ghost_ABC* >( child ) )
        {
            xos << xml::start( "phantom" );
            it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string GhostHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
std::string GhostHierarchies::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void GhostHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    if( superior_ )
        UpdateKarma( *superior_ );
    SetSuperior( superior_ );
    TacticalHierarchies::DoUpdate( ic );
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::UpdateKarma
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void GhostHierarchies::UpdateKarma( const kernel::Entity_ABC& superior )
{
    const kernel::Diplomacies_ABC& diplomacy = superior.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >();
    symbol_ = baseSymbol_;
    kernel::App6Symbol::SetKarma( symbol_, diplomacy.GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::UpdateSymbolUpward
// Created: JSR 2012-04-12
// -----------------------------------------------------------------------------
void GhostHierarchies::UpdateSymbolUpward()
{
    if( GetSuperior() )
        UpdateKarma( *GetSuperior() );
    TacticalHierarchies::UpdateSymbolUpward();
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* GhostHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = TacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}

// -----------------------------------------------------------------------------
// Name: GhostHierarchies::UpdateSymbol
// Created: ABR 2011-11-07
// -----------------------------------------------------------------------------
void GhostHierarchies::UpdateSymbol( const std::string& level, const std::string& symbol )
{
    level_ = level;
    symbol_ = symbol;
    baseSymbol_ = symbol;
}