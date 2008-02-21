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
#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: MenuButton constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::MenuButton( const QString& text, QWidget* parent )
    : QPushButton( text, parent )
{
    QFont font( font() );
    font.setPixelSize( 24 );
    setFont( font );
}

// -----------------------------------------------------------------------------
// Name: MenuButton destructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::~MenuButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MenuButton::drawButton
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
void MenuButton::drawButton( QPainter* painter )
{
    painter->fillRect( 0, 0, width(), height(), colorGroup().brush( QColorGroup::Background ) );
    if( hasMouse() )
        painter->setPen( colorGroup().color( QColorGroup::BrightText ) );
    painter->drawText( rect(), Qt::AlignCenter | Qt::SingleLine, text() );
}
