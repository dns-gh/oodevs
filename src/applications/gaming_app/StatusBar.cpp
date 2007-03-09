// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "StatusBar.h"
#include "moc_StatusBar.cpp"
#include "clients_kernel/Controllers.h"
#include "icons.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StatusBar constructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::StatusBar( QStatusBar* parent, const DetectionMap& detection, const CoordinateConverter_ABC& converter, Controllers& controllers )
    : gui::StatusBar( parent, detection, converter )
    , lastSimulationStatus_( false )
    , controllers_( controllers )
{
    checkPoint_ = new QLabel( parent );
    checkPoint_->setMinimumWidth( 20 );
    checkPoint_->setAlignment( Qt::AlignCenter );

    pTime_ = new QLabel( "---", parent );
    pTime_->setMinimumWidth( 50 );
    pTime_->setAlignment( Qt::AlignCenter );

    pTick_ = new QLabel( parent );
    pTick_->setAlignment( Qt::AlignCenter );
    pTick_->setPixmap( MAKE_PIXMAP( tickoff ) );

    pLagTimer_ = new QTimer( this );
    checkPointTimer_ = new QTimer( this );
    parent->addWidget( checkPoint_, 0, true );
    parent->addWidget( pTime_, 0, true );
	parent->addWidget( pTick_, 0, true );

    connect( pLagTimer_, SIGNAL( timeout() ), SLOT( OnLag() ) );
    connect( checkPointTimer_, SIGNAL( timeout() ), SLOT( OnCheckPoint() ) );

    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: StatusBar destructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::~StatusBar()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnLag
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::OnLag()
{
    pTick_->setPixmap( MAKE_PIXMAP( tickred ) );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::NotifyUpdated
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() && lastSimulationStatus_ == true )
    {
        pLagTimer_->stop();
        pTick_->setPixmap( MAKE_PIXMAP( tickoff ) );
        pTime_->setText( "---" );
    }
    else if( lastSimulationStatus_ == false )
        pLagTimer_->start( 10000, true );
    lastSimulationStatus_ = simulation.IsConnected();

    unsigned long seconds = simulation.GetTime();
    pTime_->setText( QString( "%1:%2:%3" ).arg( ( seconds / 3600 ) % 24, 2 )
                                          .arg( ( seconds / 60   ) % 60, 2 )
                                          .arg( ( seconds % 60   )     , 2 ) );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::NotifyUpdated
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::NotifyUpdated( const Simulation::sStartTick& )
{
    pLagTimer_->stop();
    pTick_->setPixmap( MAKE_PIXMAP( tickon ) );
}
    
// -----------------------------------------------------------------------------
// Name: StatusBar::NotifyUpdated
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::NotifyUpdated( const Simulation::sEndTick& )
{
    pTick_->setPixmap( MAKE_PIXMAP( tickoff ) );
    pLagTimer_->start( 10000, true );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::NotifyUpdated
// Created: SBO 2007-03-09
// -----------------------------------------------------------------------------
void StatusBar::NotifyUpdated( const Simulation::sCheckPoint& checkpoint )
{
    if( !checkpoint.start_ )
    {
        checkPoint_->setPixmap( QPixmap() );
        QToolTip::remove( checkPoint_ );
        checkPointTimer_->stop();
        return;
    }
    OnCheckPoint();
    QToolTip::add( checkPoint_, tr( "Saving checkpoint..." ) );
    checkPointTimer_->start( 200 );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnCheckPoint
// Created: SBO 2007-03-09
// -----------------------------------------------------------------------------
void StatusBar::OnCheckPoint()
{
    static unsigned int i = 0;
    static QPixmap pixmaps[3] = { MAKE_PIXMAP( checkpoint1 ), MAKE_PIXMAP( checkpoint2 ), MAKE_PIXMAP( checkpoint3 ) };
    checkPoint_->setPixmap( pixmaps[i] );
    i = (i + 1) % 3;
}
