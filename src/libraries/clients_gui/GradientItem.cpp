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
#include "ElevationResolver_ABC.h"
#include <qpainter.h>
#include <boost/lexical_cast.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GradientItem constructor
// Created: SBO 2007-07-02
// -----------------------------------------------------------------------------
GradientItem::GradientItem( QCanvas* canvas, const ElevationResolver_ABC& resolver,
                            unsigned short percentage, const QColor& color )
    : QCanvasLine( canvas )
    , resolver_  ( resolver )
    , percentage_( percentage )
    , color_     ( color )
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
    QFont font( "Normal", 8, QFont::Light );
    painter.setFont( font );
    const unsigned int elevation = static_cast< unsigned int >( resolver_.Compute( percentage_ ) );
    painter.drawText( GetX(), canvas()->rect().height() - 20, boost::lexical_cast< std::string >( elevation ).c_str() );
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
