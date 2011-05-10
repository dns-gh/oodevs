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
QPixmap EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    QPixmap stub( 1, 1 );
    stub.fill( Qt::white );
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !symbol )
        return stub;
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return stub;
    SymbolIcon icon( symbolName, levelName );
    icon.SetColor( strategy_.FindColor( entity ) );
    icon.SetSize( size );
    return icons_.GetSymbol( icon );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: SBO 2007-10-17
// -----------------------------------------------------------------------------
QPixmap EntitySymbols::GetSymbol( const kernel::Intelligence_ABC& entity, const QSize& size /*= QSize( 32, 32 )*/ ) const
{
    QPixmap stub( 1, 1 );
    stub.fill( Qt::white );
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::IntelligenceHierarchies >();
    if( !symbol )
        return stub;
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return stub;
    SymbolIcon icon( symbolName, levelName );
    icon.SetColor( strategy_.FindColor( entity ) );
    icon.SetSize( size );
    return icons_.GetSymbol( icon );
}
