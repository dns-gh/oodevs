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
#include "IconsRenderPass.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SymbolIcons constructor
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
SymbolIcons::SymbolIcons( QObject* parent )
    : QObject( parent )
    , widget_( 0 )
    , renderPass_( new IconsRenderPass() )
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
    if( !widget_ && widget )
        widget->AddPass( *renderPass_ );
    widget_ = widget;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::GetSymbol
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
QPixmap SymbolIcons::GetSymbol( SymbolIcon symbol )
{
    std::replace( symbol.symbol_.begin(), symbol.symbol_.end(), '*', 'f' ); // $$$$ AGE 2006-11-22: 
    const QPixmap& result = icons_[ symbol ];
    if( result.isNull() )
        if( widget_ && pending_.insert( symbol ).second )
            renderPass_->CreateIcon( symbol, *this );
    return result;
}

// -----------------------------------------------------------------------------
// Name: SymbolIcons::AddIcon
// Created: AGE 2007-10-31
// -----------------------------------------------------------------------------
void SymbolIcons::AddIcon( const SymbolIcon& task, const QPixmap& icon )
{
    icons_[ task ] = icon;
    pending_.erase( task );
}
