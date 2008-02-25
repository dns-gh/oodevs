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
#include "graphics.cpp"
#include <qpainter.h>
#include <qimage.h>

// -----------------------------------------------------------------------------
// Name: MenuButton constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::MenuButton( const QString& text, QWidget* parent )
    : QPushButton( text, parent )
    , pixmap_( qembed_findData( "menu-item.jpg" ) )
    , size_( 200, 52 )
{
    QFont font( font() );
    font.setPixelSize( 20 );
    setFont( font );
    setFixedSize( size_ );
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
    if( size() != size_ )
    {
        QImage img( pixmap_.convertToImage() );
        img = img.scale( size() );
        pixmap_ = img;
        size_ = size();
    }
    painter->drawPixmap( rect(), pixmap_ );
    if( hasMouse() )
        painter->setPen( colorGroup().color( QColorGroup::BrightText ) );
    painter->drawText( rect(), Qt::AlignCenter | Qt::SingleLine, text() );
}
