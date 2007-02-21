// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SymbolIcons.h"
#include "GlWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SymbolIcons constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
SymbolIcons::SymbolIcons( QObject* parent, GlWidget*& widget )
    : QObject( parent )
    , widget_( widget )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons destructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
SymbolIcons::~SymbolIcons()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
QPixmap SymbolIcons::GetSymbol( const std::string& name, const QColor& color /*= Qt::white*/ )
{
    return GetSymbol( name, "", color );
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::GetSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
QPixmap SymbolIcons::GetSymbol( const std::string& s, const std::string& level, const QColor& color /*= Qt::white*/ )
{
    std::string symbolName( s );
    std::replace( symbolName.begin(), symbolName.end(), '*', 'f' ); // $$$$ AGE 2006-11-22: 

    const T_Key key( level, symbolName );
    const QPixmap& result = icons_[ key ];
    if( result.isNull() )
    {
        if( pending_.insert( key ).second )
            widget_->CreateIcon( key.second, key.first, color, *this );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::AddIcon
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void SymbolIcons::AddIcon( const std::string& symbol, const std::string& level, const QPixmap& icon )
{
    const T_Key key( level, symbol );
    icons_[ key ] = icon;
    pending_.erase( key );
}
