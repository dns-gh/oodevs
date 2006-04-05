// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ColorButton.h"
#include "moc_ColorButton.cpp"

#include <qpainter.h>
#include <qcolordialog.h>

// -----------------------------------------------------------------------------
// Name: ColorButton constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
ColorButton::ColorButton( QWidget* parent /*= 0*/, const char* name /*= 0*/ )
    : QPushButton( parent, name )
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
    bool bOk = false;
    QRgb rgb = QColorDialog::getRgba( QRgb(), &bOk, this );
    if( bOk )
        SetColor( rgb );
}

// -----------------------------------------------------------------------------
// Name: ColorButton::SetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::SetColor( const QRgb& rgb )
{
    previous_ = current_;
    current_ = rgb;
    repaint();
}

// -----------------------------------------------------------------------------
// Name: ColorButton::GetColor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
QRgb ColorButton::GetColor() const
{
    return current_;
}
    
// -----------------------------------------------------------------------------
// Name: ColorButton::Revert
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void ColorButton::Revert()
{
    current_  = previous_;
}
