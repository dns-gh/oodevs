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
#include <qpainter.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DirectionWidget constructor
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
DirectionWidget::DirectionWidget( QWidget* parent )
    : QFrame( parent )
    , old_  ( 0, 0 )
    , point_( 0, 0 )
{
    setFixedSize( 48, 48 );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    old_ = point_ = rect().center();
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
// Name: DirectionWidget::Revert
// Created: AGE 2007-02-22
// -----------------------------------------------------------------------------
void DirectionWidget::Revert()
{
    Move( old_ );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::Commit
// Created: AGE 2007-02-22
// -----------------------------------------------------------------------------
void DirectionWidget::Commit()
{
    old_ = point_;
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::Move
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::Move( const QPoint& position )
{
    const QPoint center( rect().center() );
    const QPoint relative = position - center;
    geometry::Vector2f point( 2.f * relative.x() / size().width(), - 2.f * relative.y() / size().height() );
    if( point.Length() > 1.f )
        point /= point.Length();

    const float radius = point.Length();
    float angle  = radius > 1e-6 ? std::acos( point.X() / radius ) : 0;
    if( point.Y() < 0 )
        angle = -angle;

    point_ = center + QPoint( int( 0.5f * point.X() * width() ), int( - 0.5f * point.Y() * height() ) );
    Move( ( 1.f - radius ) * std::acos( -1.f ) * 0.5f, angle );
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::Move
// Created: AGE 2007-01-03
// -----------------------------------------------------------------------------
void DirectionWidget::Move( float theta, float phy )
{
    geometry::Vector3f direction( 1, 0, 0 );
    direction.Rotate( geometry::Vector3f( 0, 1, 0 ), -theta );
    direction.Rotate( geometry::Vector3f( 0, 0, 1 ), phy );
    emit DirectionChanged( direction );
    update();
}

// -----------------------------------------------------------------------------
// Name: DirectionWidget::paintEvent
// Created: AGE 2007-01-02
// -----------------------------------------------------------------------------
void DirectionWidget::paintEvent( QPaintEvent* )
{
    QPainter paint( this );
    paint.drawEllipse( rect() );
    paint.drawLine( rect().center() - QPoint( 2, 0 ), rect().center() + QPoint( 2, 0 ) );
    paint.drawLine( rect().center() - QPoint( 0, 2 ), rect().center() + QPoint( 0, 2 ) );
    paint.drawEllipse( point_.x() - 2, point_.y() - 2, 5, 5 );
}
