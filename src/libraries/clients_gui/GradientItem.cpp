// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientItem.h"
#include <qpainter.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientItem constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem::GradientItem( QCanvas* canvas, unsigned short percentage, const QColor& color )
    : QCanvasLine( canvas )
    , percentage_( percentage )
    , color_( color )
{
    UpdatePosition();
    show();
}

// -----------------------------------------------------------------------------
// Name: GradientItem destructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem::~GradientItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GradientItem::SetColor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientItem::SetColor( const QColor& color )
{
    color_ = color;
}

// -----------------------------------------------------------------------------
// Name: GradientItem::GetColor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
QColor GradientItem::GetColor() const
{
    return color_;
}

// -----------------------------------------------------------------------------
// Name: GradientItem::GetPercentage
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
unsigned short GradientItem::GetPercentage() const
{
    return percentage_;
}

// -----------------------------------------------------------------------------
// Name: GradientItem::SetPercentage
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientItem::SetPercentage( unsigned short percentage )
{
    percentage_ = percentage;
    UpdatePosition();
}

// -----------------------------------------------------------------------------
// Name: GradientItem::draw
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientItem::draw( QPainter& painter )
{
    UpdatePosition();
    if( isSelected() )
        setPen( QColor( Qt::white ) );
    else
        setPen( QColor( Qt::black ) );
    QCanvasLine::draw( painter );
    painter.fillRect( startPoint().x() - 2, startPoint().y(), 5, 5, pen().color() );
}

// -----------------------------------------------------------------------------
// Name: GradientItem::UpdatePosition
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientItem::UpdatePosition()
{
    const unsigned short x = 0.01 * float( percentage_ ) * float( canvas()->rect().width() );
    setPoints( x, 0, x, canvas()->rect().height() );
}
