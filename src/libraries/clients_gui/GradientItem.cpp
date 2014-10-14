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

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientItem constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem::GradientItem( Q3Canvas* canvas,
                            const T_Drawer& drawer,
                            unsigned short percentage,
                            const QColor& color,
                            bool disableState )
    : Q3CanvasLine( canvas )
    , drawer_      ( drawer )
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
    Q3CanvasLine::draw( painter );
    painter.fillRect( startPoint().x() - 3, startPoint().y(), 7, 7, pen().color() );
    if( !disableState_ && drawer_ )
        drawer_( painter, percentage_, GetX(), canvas()->rect().height() - 20 );
}

// -----------------------------------------------------------------------------
// Name: GradientItem::areaPoints
// Created: SBO 2008-01-16
// -----------------------------------------------------------------------------
Q3PointArray GradientItem::areaPoints() const
{
    return Q3PointArray( QRect( startPoint().x(), startPoint().y(), 7, canvas()->rect().height() ) );
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
