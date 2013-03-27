// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ClearButton.h"
#include "moc_ClearButton.cpp"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ClearButton constructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
ClearButton::ClearButton( QWidget *parent /* = 0*/ )
    : QAbstractButton(parent)
{
    ObjectNameManager::getInstance()->SetObjectName( this, "clearButton" );
    setCursor( Qt::ArrowCursor );
    setFocusPolicy( Qt::NoFocus );
    setToolTip( tr( "Clear" ) );
    setMinimumSize( 22, 22 );
    setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: ClearButton destructor
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
ClearButton::~ClearButton()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: ClearButton::TextChanged
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void ClearButton::TextChanged( const QString& text )
{
    setVisible( !text.isEmpty() );
}

// -----------------------------------------------------------------------------
// Name: ClearButton::paintEvent
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void ClearButton::paintEvent( QPaintEvent* event )
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
