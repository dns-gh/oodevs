// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EventWidget.h"


// -----------------------------------------------------------------------------
// Name: EventWidget constructor
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
EventWidget::EventWidget( QWidget* parent, const QPixmap& pixmap, const QString& text )
    : QLabel( parent )
    , label_( "" )
{
    QImage img; // $$$$ SBO 2007-02-09: TODO: make new icons instead of resizing
    img = pixmap;
    img = img.smoothScale( 26, 26, Qt::KeepAspectRatioByExpanding );
    QPixmap pix( img );
    setPixmap( pix );
    QFont font;
    font.setPointSize( 9 );
    setFont( font );
    setFixedSize( 27, 27 );
    setToolTip( text );
    hide();
}

// -----------------------------------------------------------------------------
// Name: EventWidget destructor
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
EventWidget::~EventWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventWidget::setText
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
void EventWidget::setText( const QString& text )
{
    label_ = text;
    update();
}

// -----------------------------------------------------------------------------
// Name: EventWidget::paintEvent
// Created: LGY 2012-11-26
// -----------------------------------------------------------------------------
void EventWidget::paintEvent( QPaintEvent* event )
{
    QLabel::paintEvent( event );
    if( label_.isEmpty() )
        return;
    QPainter painter( this );
    QPen pen(  Qt::yellow );
    painter.setPen( pen );
    const QRect rect = painter.boundingRect( QRect( 0, 0, width() - 2, height() - 1 ), Qt::AlignCenter, label_ );
    painter.setPen( pen );
    painter.drawText( rect, Qt::AlignCenter, label_ );
}
