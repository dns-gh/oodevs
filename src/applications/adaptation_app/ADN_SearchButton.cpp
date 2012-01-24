// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_SearchButton.h"

// -----------------------------------------------------------------------------
// Name: ADN_SearchButton constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchButton::ADN_SearchButton( QWidget* parent /*= 0*/ )
    : QPushButton( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchButton destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SearchButton::~ADN_SearchButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchButton::sizeHint
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
QSize ADN_SearchButton::sizeHint() const
{
    if( !image_.isNull() )
        return image_.size();
    return QSize( 12, 16 );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchButton::paintEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_SearchButton::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event );
    if( image_.isNull() )
        image_ = GenerateSearchImage();
    QPainter painter( this );
    painter.drawImage( QPoint( 0, 0 ), image_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_SearchButton::GenerateSearchImage
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
QImage ADN_SearchButton::GenerateSearchImage()
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

