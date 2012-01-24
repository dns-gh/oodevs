// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ClearButton.h"
#include "moc_ADN_ClearButton.cpp"

// -----------------------------------------------------------------------------
// Name: ADN_ClearButton constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_ClearButton::ADN_ClearButton( QWidget *parent /*= 0*/ )
    : QAbstractButton(parent)
{
    setCursor( Qt::ArrowCursor );
    setFocusPolicy( Qt::NoFocus );
    setToolTip( tr( "Clear" ) );
    setMinimumSize( 22, 22 );
    setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: ADN_ClearButton destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_ClearButton::~ADN_ClearButton()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ClearButton::TextChanged
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_ClearButton::TextChanged( const QString& text )
{
    setVisible( !text.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ADN_ClearButton::paintEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_ClearButton::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    QPainter painter( this );

    painter.setRenderHint( QPainter::Antialiasing, true );
    QPalette p = palette();
    QColor circleColor = isDown() ? p.color( QPalette::Dark ) : p.color( QPalette::Mid );
    QColor xColor = p.color( QPalette::Window );

    // draw circle
    painter.setBrush( circleColor );
    painter.setPen( circleColor );
    int padding = width() / 5;
    int circleRadius = width() - ( padding * 2 );
    painter.drawEllipse( padding, padding, circleRadius, circleRadius );

    // draw X
    painter.setPen( xColor );
    padding = padding * 2;
    painter.drawLine( padding, padding,            width() - padding, width() - padding );
    painter.drawLine( padding, height() - padding, width() - padding, padding );
}
