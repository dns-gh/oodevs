// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "StatusBar.h"
#include "moc_StatusBar.cpp"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Simulation.h"

#include <qstatusbar.h>

// -----------------------------------------------------------------------------
// Name: StatusBar constructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::StatusBar( QStatusBar* parent, const DetectionMap& detection, const CoordinateConverter_ABC& converter, Controllers& controllers )
    : detection_( detection )
    , converter_( converter )
    , lastSimulationStatus_( false )
{
    pPositionXYZ_ = new QLabel( "---", parent );
    pPositionXYZ_->setMinimumWidth( 195 );
    pPositionXYZ_->setAlignment( Qt::AlignCenter );

    pPositionMgrs_ = new QLabel( "---", parent );
	pPositionMgrs_->setMinimumWidth( 105 );
    pPositionMgrs_->setAlignment( Qt::AlignCenter );

    pPositionLatLong_ = new QLabel( "---", parent );
	pPositionLatLong_->setMinimumWidth( 125 );
    pPositionLatLong_->setAlignment( Qt::AlignCenter );

    pTime_ = new QLabel( "---", parent );
    pTime_->setMinimumWidth( 50 );
    pTime_->setAlignment( Qt::AlignCenter );

    pTick_ = new QLabel( parent );
    pTick_->setAlignment( Qt::AlignCenter );
    pTick_->setPixmap( MAKE_PIXMAP( tickoff ) );

    pLagTimer_ = new QTimer( this );

    parent->addWidget( pPositionXYZ_    , 0, true );
	parent->addWidget( pPositionMgrs_   , 0, true );
	parent->addWidget( pPositionLatLong_, 0, true );
    parent->addWidget( pTime_, 0, true );
	parent->addWidget( pTick_, 0, true );

    connect( pLagTimer_, SIGNAL( timeout() ), this, SLOT( OnLag() ) );

    controllers.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: StatusBar destructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::~StatusBar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnMouseMove
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::OnMouseMove( const geometry::Point2f& position )
{
    if( !converter_.IsInBoundaries( position ) )
    {
        pPositionXYZ_->setText( "---" );
		pPositionMgrs_->setText( "---" );
        pPositionLatLong_->setText( "---" );
	}
	else
    {
        pPositionXYZ_->setText( QString( "x:%1 y:%2 h:%3" ).arg( position.X(), 2 )
                                                           .arg( position.Y(), 2 )
                                                           .arg( detection_.ElevationAt( position ) ) );
        pPositionMgrs_->setText( converter_.ConvertToMgrs( position ).c_str() );

        const geometry::Point2f latLong( converter_.ConvertToGeo( position ) );
		pPositionLatLong_->setText( QString( "Lat:%1 Lon:%2" ).arg( latLong.X(), 0, 'g', 3 ).arg( latLong.Y(), 0, 'g', 3 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnMouseMove
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::OnMouseMove( const geometry::Point3f& position )
{
    const geometry::Point2f point( position.X(), position.Y() );
    OnMouseMove( point );
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
