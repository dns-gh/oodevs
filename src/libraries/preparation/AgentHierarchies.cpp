// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentHierarchies.h"
#include "Agent.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/SymbolHierarchy_ABC.h"
#include "clients_kernel/SymbolFactory.h"

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( kernel::Controller& controller, Agent& holder, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory )
    : TacticalHierarchies( controller, holder, 0 )
    , level_        ( factory.CreateLevelSymbol( ENT_Tr::ConvertFromNatureLevel( holder.GetLevel() ) ) )
    , baseSymbol_   ( holder.GetSymbol() )
    , symbol_       ( holder.GetSymbol() )
    , superior_     ( superior )
    , symbolFactory_( factory )
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
// Name: AgentHierarchies::InternalNotifySuperiorChanged
// Created: NPT 2012-12-13
// -----------------------------------------------------------------------------
void AgentHierarchies::NotifySuperiorChanged( const kernel::Entity_ABC* newSuperior )
{
    if( newSuperior && newSuperior->Retrieve< kernel::TacticalHierarchies >() )
        UpdateKarma( *newSuperior );
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
// Name: AgentHierarchies::UpdateSymbolDownward
// Created: ABR 2012-10-22
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSymbolDownward()
{
    const std::string oldSymbol = GetSymbol();
    if( const kernel::Entity_ABC* superior = GetSuperior() )
        UpdateKarma( *superior );
    if( GetSymbol() != oldSymbol )
        controller_.Update( *static_cast< kernel::Symbol_ABC* >( this ) );
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

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::SetLevel
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void AgentHierarchies::SetLevel( E_NatureLevel level )
{
    level_ = symbolFactory_.CreateLevelSymbol( ENT_Tr::ConvertFromNatureLevel( level ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::SetSymbol
// Created: ABR 2012-08-08
// -----------------------------------------------------------------------------
void AgentHierarchies::SetSymbol( const std::string& symbol )
{
    symbol_ = symbol;
}
