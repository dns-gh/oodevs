// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EntitySymbols.h"
#include "SymbolIcons.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/IntelligenceHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntitySymbols::EntitySymbols
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
EntitySymbols::EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy )
    : icons_   ( icons )
    , strategy_( strategy )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::~EntitySymbols
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
EntitySymbols::~EntitySymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !symbol )
        return icons_.GetDefaultSymbol();
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return icons_.GetDefaultSymbol();
    return GetSymbol( entity, symbolName, levelName, size );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
const QPixmap& EntitySymbols::GetSymbol( const kernel::Intelligence_ABC& entity, const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::IntelligenceHierarchies >();
    if( !symbol )
        return icons_.GetDefaultSymbol();
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return icons_.GetDefaultSymbol();
    return GetSymbol( entity, symbolName, levelName, size );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: LGY 2011-07-22
// -----------------------------------------------------------------------------
const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
                                         const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    SymbolIcon icon( symbolName, levelName );
    icon.SetColor( strategy_.FindColor( entity ) );
    icon.SetSize( size );
    return icons_.GetSymbol( icon );
}
