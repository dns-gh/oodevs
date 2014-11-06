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

const QPixmap& EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity, ColorMode colorMode ) const
{
    const kernel::Symbol_ABC* symbol = entity.Retrieve< kernel::TacticalHierarchies >();
    if( !symbol )
        return icons_.GetDefaultSymbol();
    const std::string symbolName = symbol->GetSymbol();
    const std::string levelName  = symbol->GetLevel();
    if( symbolName.empty() && levelName.empty() )
        return icons_.GetDefaultSymbol();
    return GetSymbol( entity, symbolName, levelName, colorMode );
}

const QPixmap& EntitySymbols::GetSymbol(
    const kernel::Entity_ABC& entity, const std::string& symbolName,
    const std::string& levelName, ColorMode colorMode ) const
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
    return icons_.GetSymbol( icon );
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::RecGenerateSymbols
// Created: ABR 2013-02-18
// -----------------------------------------------------------------------------
void EntitySymbols::RecGenerateSymbols( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Hierarchies* hierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy->CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& child = it.NextElement();
            RecGenerateSymbols( child );

            const kernel::Symbol_ABC* symbol = child.Retrieve< kernel::TacticalHierarchies >();
            if( !symbol )
                continue;
            const std::string symbolName = symbol->GetSymbol();
            const std::string levelName  = symbol->GetLevel();
            if( symbolName.empty() && levelName.empty() )
                continue;

            GetSymbol( child, symbolName, levelName, gui::EntitySymbols::eColorBase );
            GetSymbol( child, symbolName, levelName, gui::EntitySymbols::eColorSelected );
            GetSymbol( child, symbolName, levelName, gui::EntitySymbols::eColorSuperiorSelected );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GenerateSymbols
// Created: ABR 2013-02-18
// -----------------------------------------------------------------------------
void EntitySymbols::GenerateSymbols( const tools::Resolver< kernel::Team_ABC >& teamResolver ) const
{
    tools::Iterator< const kernel::Team_ABC& > it = teamResolver.CreateIterator();
    while( it.HasMoreElements() )
    {
        RecGenerateSymbols( it.NextElement() );
    }
}
