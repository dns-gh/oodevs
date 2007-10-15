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
#include "moc_SymbolIcons.cpp"
#include "GlWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SymbolIcons constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
SymbolIcons::SymbolIcons( QObject* parent )
    : QObject( parent )
    , widget_( 0 )
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
// Name: SymbolIcons::OnWidget2dChanged
// Created: AGE 2007-03-09
// -----------------------------------------------------------------------------
void SymbolIcons::OnWidget2dChanged( gui::GlWidget* widget )
{
    widget_ = widget;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
QPixmap SymbolIcons::GetSymbol( const std::string& name, const QColor& color /*= Qt::white*/, const QSize& size /*= QSize( 32, 32 )*/ )
{
    return GetSymbol( name, "", color, size );
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::GetSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
QPixmap SymbolIcons::GetSymbol( const std::string& s, const std::string& level, const QColor& color /*= Qt::white*/, const QSize& size /*= QSize( 32, 32 )*/ )
{
    std::string symbolName( s );
    std::replace( symbolName.begin(), symbolName.end(), '*', 'f' ); // $$$$ AGE 2006-11-22: 

    const Key key( symbolName, level, color, size );
    const QPixmap& result = icons_[ key ];
    if( result.isNull() )
        if( widget_ && pending_.insert( key ).second )
            widget_->CreateIcon( key.symbol_, key.level_, key.color_, *this, key.size_ );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::AddIcon
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void SymbolIcons::AddIcon( const std::string& symbol, const std::string& level, const QColor& color, const QSize& size, const QPixmap& icon )
{
    const Key key( symbol, level, color, size );
    icons_[ key ] = icon;
    pending_.erase( key );
}
