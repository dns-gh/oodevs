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
#include "SelectionColorModifier.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace gui;

EntitySymbols::EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy )
    : icons_   ( icons )
    , strategy_( strategy )
{
    // NOTHING
}

EntitySymbols::~EntitySymbols()
{
    // NOTHING
}

const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const QSize& size /*= QSize( 64, 64 )*/, ColorMode colorMode /* = eColorBase */ ) const
{
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !symbol )
        return icons_.GetDefaultSymbol();
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return icons_.GetDefaultSymbol();
    return GetSymbol( entity, symbolName, levelName, size, colorMode );
}

const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, const std::string& symbolName, const std::string& levelName,
    const QSize& size /*= QSize( 64, 64 )*/, ColorMode colorMode /* = eColorBase */ ) const
{
    SymbolIcon icon( symbolName, levelName );
    QColor color;
    switch( colorMode )
    {
    case eColorBase:
        color = strategy_.FindColor( entity );
        break;
    case eColorWithModifier:
        color = strategy_.FindColorWithModifiers( entity );
        break;
    case eColorSelected:
        color = SelectionColorModifier::SelectedColor( strategy_.FindColor( entity ) );
        break;
    case eColorSuperiorSelected:
        color = SelectionColorModifier::SuperiorSelectedColor( strategy_.FindColor( entity ) );
        break;
    default:
        break;
    }
    icon.SetColor( color );
    icon.SetSize( size );
    return icons_.GetSymbol( icon );
}
