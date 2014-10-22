// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DirectionWidget.h"
#include "moc_DirectionWidget.cpp"
#include "LightingHelpers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DirectionWidget constructor
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
DirectionWidget::DirectionWidget( const QString& objectName, QWidget* parent )
    : QFrame( parent )
    , point_( 0, 0 )
{
    setObjectName( objectName );
    setFixedSize( lighting::GetEditorRect().size() + QSize( 1, 1 ) );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    point_ = rect().center();
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget destructor
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
DirectionWidget::~DirectionWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::mousePressEvent
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::mousePressEvent( QMouseEvent* e )
{
    if( e->button() & Qt::LeftButton )
        Move( e->pos() );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::mouseMoveEvent
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::mouseMoveEvent( QMouseEvent* e )
{
    Move( e->pos() );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::Move
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::Move( const QPoint& position )
{
    point_ = position;
    emit DirectionChanged( lighting::PointToDirectionVector( point_ ) );
    update();
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::paintEvent
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::paintEvent( QPaintEvent* )
{
    QPainter paint( this );
    const auto rect = lighting::GetEditorRect();
    paint.drawEllipse( rect );
    const auto center = rect.center();
    paint.drawLine( center - QPoint( 2, 0 ), center + QPoint( 2, 0 ) );
    paint.drawLine( center - QPoint( 0, 2 ), center + QPoint( 0, 2 ) );
    paint.drawEllipse( point_, 3, 3 );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::GetValue
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
QString DirectionWidget::GetValue() const
{
    return locale().toString( point_.x() ) + ":" + locale().toString( point_.y() );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::SetValue
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void DirectionWidget::SetValue( const QString& value )
{
    if( value == GetValue() )
        return;
    QStringList list = QStringList::split( ":", value );
    if( list.size() != 2 )
        return;
    Move( QPoint( list[0].toInt(), list[1].toInt() ) );
}
