// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GradientItem.h"
#include "Painter_ABC.h"
#include <qpainter.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientItem constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem::GradientItem( QCanvas* canvas, const Painter_ABC& painter,
                            unsigned short percentage, const QColor& color, bool disableState )
    : QCanvasLine( canvas )
    , painter_     ( painter )
    , percentage_  ( percentage )
    , color_       ( color )
    , disableState_( disableState )
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
// Name: GradientItem::GetX
// Created: LGY 2010-09-27
// -----------------------------------------------------------------------------
unsigned short GradientItem::GetX()
{
    return unsigned short( 0.01f * float( percentage_ ) * float( canvas()->rect().width() ) );;
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
    painter.fillRect( startPoint().x() - 3, startPoint().y(), 7, 7, pen().color() );
    if( ! disableState_ )
        painter_.Draw( painter, percentage_, GetX(), canvas()->rect().height() - 20 );
}

// -----------------------------------------------------------------------------
// Name: GradientItem::areaPoints
// Created: SBO 2008-01-16
// -----------------------------------------------------------------------------
QPointArray GradientItem::areaPoints() const
{
    return QPointArray( QRect( startPoint().x(), startPoint().y(), 7, canvas()->rect().height() ) );
}

// -----------------------------------------------------------------------------
// Name: GradientItem::UpdatePosition
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
void GradientItem::UpdatePosition()
{
    const unsigned short x = GetX();
    setPoints( x, 0, x, canvas()->rect().height() - 30 );
}

// -----------------------------------------------------------------------------
// Name: GradientItem::ToggleScale
// Created: LGY 2010-09-29
// -----------------------------------------------------------------------------
void GradientItem::ToggleScale( bool state )
{
    disableState_ = state;
}
