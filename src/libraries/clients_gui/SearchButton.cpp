// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SearchButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: SearchButton constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchButton::SearchButton( const QString& objectName, QWidget* parent /*= 0*/ )
    : RichPushButton( objectName, "", parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchButton destructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
SearchButton::~SearchButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SearchButton::paintEvent
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void SearchButton::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event );
    if( image_.isNull() )
        image_ = GenerateSearchImage();
    QPainter painter( this );
    painter.drawImage( QPoint( 0, 0 ), image_ );
}

// -----------------------------------------------------------------------------
// Name: SearchButton::sizeHint
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
QSize SearchButton::sizeHint() const
{
    if( !image_.isNull() )
        return image_.size();
    return QSize( 12, 16 );
}

// -----------------------------------------------------------------------------
// Name: SearchButton::GenerateSearchImage
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
QImage SearchButton::GenerateSearchImage()
{
    QImage image( 12, 16, QImage::Format_ARGB32 );
    image.fill( qRgba( 0, 0, 0, 0 ) );
    QPainterPath path;

    // create magnify glass circle
    int radius = image.height() / 2;
    QRect circle( 1, 1, radius, radius );
    path.addEllipse( circle );

    // create handle
    path.arcMoveTo( circle, 300 );
    QPointF currentPosition = path.currentPosition();
    path.moveTo( currentPosition.x() + 1, currentPosition.y() + 1 );
    path.lineTo( image.width() - 2, image.height() - 4 );

    // draw both
    QPainter painter( &image );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setPen( QPen( Qt::darkGray, 2 ) );
    painter.drawPath( path );

    painter.end();
    return image;
}
