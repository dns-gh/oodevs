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
#include "clients_kernel/Knowledge_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EntitySymbols::EntitySymbols
// Created: SBO 2007-02-21
// -----------------------------------------------------------------------------
EntitySymbols::EntitySymbols( SymbolIcons& icons, ColorStrategy_ABC& strategy )
    : icons_( icons )
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
QPixmap EntitySymbols::GetSymbol( const kernel::Entity_ABC& entity )
{
    QPixmap stub( 1, 1 );
    stub.fill( Qt::white );
    if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
    {
        const std::string symbolName = hierarchies->GetSymbol();
        const std::string levelName  = hierarchies->GetLevel();
        if( symbolName.empty() && levelName.empty() )
            return stub;
        return icons_.GetSymbol( symbolName, levelName, strategy_.FindColor( entity ) );
    }
    return stub;
}

// -----------------------------------------------------------------------------
// Name: EntitySymbols::GetSymbol
// Created: SBO 2007-02-26
// -----------------------------------------------------------------------------
QPixmap EntitySymbols::GetSymbol( const kernel::Knowledge_ABC& entity )
{
    std::string level = "";
    if( const kernel::Entity_ABC* realOne = entity.GetRecognizedEntity() )
        if( const kernel::TacticalHierarchies* hierarchies = entity.GetEntity()->Retrieve< kernel::TacticalHierarchies >() )
            level = hierarchies->GetLevel();
    return icons_.GetSymbol( entity.GetSymbol(), level, strategy_.FindColor( entity ) );   
}
