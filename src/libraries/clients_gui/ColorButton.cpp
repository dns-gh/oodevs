// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorButton.h"
#include "moc_ColorButton.cpp"

#include <qpainter.h>
#include <qcolordialog.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::ColorButton( QWidget* parent /*= 0*/, const char* name /*= 0*/, QColor color /*= black*/ )
    : QToolButton( parent, name )
    , previous_( color )
    , current_( color )
{
    setAutoRaise( true );
    setMinimumSize( 25, 25 );
    setMaximumWidth( height() * 4 / 3 );
    connect( this, SIGNAL( clicked() ), SLOT( OnClick() ) );
}

// -----------------------------------------------------------------------------
// Name: ColorButton destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::~ColorButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ColorButton::drawButton
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::drawButton( QPainter* painter )
{
    QToolButton::drawButton( painter );
    painter->fillRect( 4, 4, width() - 8, height() - 8, QBrush( QColor( current_ ) ) );
    painter->drawRect( 4, 4, width() - 8, height() - 8 );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::OnClick
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::OnClick()
{
    QColor color = QColorDialog::getColor( current_, this );
    if( color.isValid() )
        SetColor( color );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::SetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::SetColor( const QColor& rgb )
{
    current_ = rgb;
    repaint();
    emit ColorChanged( rgb );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::GetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
QColor ColorButton::GetColor() const
{
    return current_;
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Revert
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::Revert()
{
    SetColor( previous_ );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ColorButton::Commit()
{
    previous_ = current_;
}
