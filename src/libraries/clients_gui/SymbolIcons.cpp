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
QPixmap SymbolIcons::GetSymbol( const std::string& name )
{
    const QPixmap& result = icons_[ name ];
    if( result.isNull() )
    {
        pending_.push_back( name );
        CreatePendingIcons();
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::CreatePendingIcons
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void SymbolIcons::CreatePendingIcons()
{
    const QColor white( "white" );
    for( CIT_PendingIcons it = pending_.begin(); it != pending_.end(); ++it )
        widget_->CreateIcon( *it, white, *this );
    pending_.clear();
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::AddIcon
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void SymbolIcons::AddIcon( const std::string& name, const QPixmap& icon )
{
    icons_[ name ] = icon;
}
