// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MenuButton.h"

// -----------------------------------------------------------------------------
// Name: MenuButton constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::MenuButton( const QString& text, QWidget* parent )
    : QPushButton( text, parent )
{
    QFont font( "Verdana", 24, QFont::Bold );
    setFont( font );
    setPaletteBackgroundColor( QColor( 32, 32, 48 ) );
    setPaletteForegroundColor( Qt::white );
    setFlat( true );
    setDefault( false );
}

// -----------------------------------------------------------------------------
// Name: MenuButton destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::~MenuButton()
{
    // NOTHING
}
