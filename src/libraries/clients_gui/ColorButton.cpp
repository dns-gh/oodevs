// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ColorButton.h"
#include "moc_ColorButton.cpp"

#include <qpainter.h>
#include <qcolordialog.h>

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::ColorButton( QWidget* parent /*= 0*/, const char* name /*= 0*/, QColor color /*= black*/ )
    : QPushButton( parent, name )
    , changed_( false )
    , previous_( color )
    , current_( color )
{
    setMaximumWidth( height() * 4 / 3 );
    connect( this, SIGNAL( clicked() ), this, SLOT( OnClick() ) );
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
    QPushButton::drawButton( painter );
    painter->fillRect( 4, 4, width() - 8, height() - 8, QBrush( QColor( current_ ) ) );
    painter->drawRect( 4, 4, width() - 8, height() - 8 );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::OnClick
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::OnClick()
{
    SetColor( QColorDialog::getColor( previous_, this ) );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::SetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::SetColor( const QColor& rgb )
{
    if( ! changed_ )
        previous_ = current_;
    changed_ = true;
    current_ = rgb;
    repaint();
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
    changed_ = false;
    current_  = previous_;
}

// -----------------------------------------------------------------------------
// Name: ColorButton::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void ColorButton::Commit()
{
    previous_ = current_;
    changed_ = false;
}
