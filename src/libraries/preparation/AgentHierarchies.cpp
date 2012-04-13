// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const std::string& level, const std::string& symbol, kernel::Entity_ABC* superior )
    : TacticalHierarchies( controller, holder, 0 )
    , level_ ( level )
    , baseSymbol_( symbol )
    , symbol_( symbol )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::~AgentHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AgentHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
std::string AgentHierarchies::GetSymbol() const
{
    return symbol_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    if( superior_ )
        UpdateKarma( *superior_ );
    SetSuperior( superior_ );
    TacticalHierarchies::DoUpdate( ic );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateKarma
// Created: AGE 2006-11-24
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateKarma( const kernel::Entity_ABC& superior )
{
    const kernel::Diplomacies_ABC& diplomacy = superior.Get< kernel::TacticalHierarchies >().GetTop().Get< kernel::Diplomacies_ABC >();
    symbol_ = baseSymbol_;
    kernel::App6Symbol::SetKarma( symbol_, diplomacy.GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateSymbolUpward
// Created: JSR 2012-04-12
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSymbolUpward()
{
    if( GetSuperior() )
        UpdateKarma( *GetSuperior() );
    TacticalHierarchies::UpdateSymbolUpward();
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AgentHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = TacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateSymbol
// Created: ABR 2011-11-07
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSymbol( const std::string& level, const std::string& symbol )
{
    level_ = level;
    symbol_ = symbol;
    baseSymbol_ = symbol;
}
