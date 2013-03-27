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
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( const QString& objectName, QWidget* parent /* = 0 */ )
    : QGroupBox( parent )
    , timer_             ( new QTimer( this ) )
    , originalTextColor_ ( palette().color( QPalette::Text ) )
    , originalLightColor_( palette().color( QPalette::Light ) )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
    connect( timer_, SIGNAL( timeout() ), SLOT( OnBlink() ) );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( const QString& objectName, const QString& title, QWidget* parent /* = 0 */ )
    : QGroupBox( parent )
    , timer_             ( new QTimer( this ) )
    , originalTextColor_ ( palette().color( QPalette::Text ) )
    , originalLightColor_( palette().color( QPalette::Light ) )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
    setTitle( title );
    connect( timer_, SIGNAL( timeout() ), SLOT( OnBlink() ) );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox destructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::~RichGroupBox()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
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
