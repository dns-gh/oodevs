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
#include "moc_MenuButton.cpp"

// -----------------------------------------------------------------------------
// Name: MenuButton constructor
// Created: SBO 2008-02-21
// -----------------------------------------------------------------------------
MenuButton::MenuButton( const QString& text, QWidget* parent )
    : QPushButton( parent )
    , mask_( "resources/images/selftraining/menu-mask.png" )
    , baseFont_( "Century Gothic", 16, QFont::Bold )
    , selectedFont_( "Century Gothic", 18, QFont::Bold )
    , disabledFont_( "Century Gothic", 18, QFont::Bold )
    , hasMouse_( false )
{
    disabledFont_.setItalic( true ) ;
    setFont( baseFont_ );
    setText( text );
    setBackgroundOrigin( QWidget::WindowOrigin );
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
    repaint( false );
    emit ( Selected( this ) ) ;
}

// -----------------------------------------------------------------------------
// Name: MenuButton::leaveEvent
// Created: SBO 2008-08-12
// -----------------------------------------------------------------------------
void MenuButton::leaveEvent( QEvent* )
{
    hasMouse_ = false;
    repaint( false );
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

void MenuButton::paintEvent( QPaintEvent* )
{
    QPainter painter;
    if ( painter.begin( this ) )
    {
        const QFont& font = isEnabled() ? ( hasMouse_ ? selectedFont_ : baseFont_ ) : disabledFont_  ;
        const QColorGroup::ColorRole& colorRole = isEnabled() ? ( hasMouse_ ? QColorGroup::BrightText : QColorGroup::ButtonText ) : QColorGroup::Light ;
        painter.drawImage( rect(), mask_ );
        painter.setFont( font );
        painter.setPen( colorGroup().color( colorRole ) );
        painter.drawText( rect(), Qt::AlignCenter | Qt::SingleLine, text() );
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
