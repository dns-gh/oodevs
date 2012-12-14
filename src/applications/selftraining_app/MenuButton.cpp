// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MenuButton.h"
#include "moc_MenuButton.cpp"

// -----------------------------------------------------------------------------
// Name: MenuButton constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::MenuButton()
    : QPushButton()
    , mask_( "resources/images/selftraining/menu-mask.png" )
    , baseFont_( "Century Gothic", 16, QFont::Bold )
    , selectedFont_( "Century Gothic", 18, QFont::Bold )
    , disabledFont_( "Century Gothic", 18, QFont::Bold )
    , hasMouse_( false )
{
    disabledFont_.setItalic( true ) ;
    setFont( baseFont_ );
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
// Name: MenuButton::enterEvent
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
void MenuButton::enterEvent( QEvent* )
{
    hasMouse_ = true;
    repaint();
    emit ( Selected( this ) ) ;
}

// -----------------------------------------------------------------------------
// Name: MenuButton::leaveEvent
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
void MenuButton::leaveEvent( QEvent* )
{
    hasMouse_ = false;
    repaint();
    emit ( UnSelected( this ) ) ;
}

// -----------------------------------------------------------------------------
// Name: MenuButton::drawButton
// Created: SBO 2008-08-08
// -----------------------------------------------------------------------------
void MenuButton::drawButton( QPainter* )
{
    paintEvent();
}

// -----------------------------------------------------------------------------
// Name: MenuButton::paintEvent
// Created: FPT 2011-08-08
// -----------------------------------------------------------------------------
void MenuButton::paintEvent( QPaintEvent* )
{
    QPainter painter;
    if( painter.begin( this ) )
    {
        const QFont& font = isEnabled() ? ( hasMouse_ ? selectedFont_ : baseFont_ ) : disabledFont_ ;
        const QPalette::ColorRole& colorRole = isEnabled() ? ( hasMouse_ ? QPalette::BrightText : QPalette::ButtonText ) : QPalette::Light ;
        painter.setFont( font );
        painter.setPen( palette().color( colorRole ) );
        painter.drawText( rect(), Qt::AlignCenter | Qt::TextSingleLine, text() );
        painter.end();
    }
}

// -----------------------------------------------------------------------------
// Name: MenuButton::sizeHint
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
QSize MenuButton::sizeHint() const
{
    QFontMetrics metrics( selectedFont_ );
    QRect rect = metrics.boundingRect( text() );
    return QSize( rect.width(), rect.height() );
}
