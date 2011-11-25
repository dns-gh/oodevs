// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichGroupBox.h"
#include "moc_RichGroupBox.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( QWidget* parent )
    : QGroupBox( parent )
    , timer_             ( new QTimer( this ) )
    , originalTextColor_ ( palette().color( QPalette::Text ) )
    , originalLightColor_( palette().color( QPalette::Light ) )
{
    connect( timer_, SIGNAL( timeout() ), SLOT( OnBlink() ) );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( const QString& title, QWidget* parent )
    : QGroupBox( title, parent )
    , timer_             ( new QTimer( this ) )
    , originalTextColor_ ( palette().color( QPalette::Text ) )
    , originalLightColor_( palette().color( QPalette::Light ) )
{
    connect( timer_, SIGNAL( timeout() ), SLOT( OnBlink() ) );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox destructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::~RichGroupBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox::Warn
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
void RichGroupBox::Warn()
{
    blink_ = false;
    timer_->start( 500 );
    QTimer::singleShot( 3000, this, SLOT( OnDone() ) );
    Light( Qt::red, Qt::red );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox::OnBlink
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
void RichGroupBox::OnBlink()
{
    if( blink_ )
        Light( Qt::red, Qt::red );
    else
        Light( originalTextColor_, originalLightColor_ );
    blink_ = !blink_;
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox::OnDone
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
void RichGroupBox::OnDone()
{
    timer_->stop();
    Light( originalTextColor_, originalLightColor_ );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox::Light
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
void RichGroupBox::Light( const QColor& textColor, const QColor& lightColor )
{
    QPalette p = palette();
    p.setColor( QPalette::Text, textColor );
    p.setColor( QPalette::Light, lightColor );
    p.setColor( QPalette::Midlight, lightColor );
    setPalette( p );
}
