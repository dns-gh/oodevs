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
#include <qstatusbar.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: StatusBar constructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::StatusBar( QStatusBar* parent, const DetectionMap& detection, const CoordinateConverter_ABC& converter )
    : detection_( detection )
    , converter_( converter )
{
    pPositionXYZ_ = new QLabel( "---", parent );// $$$$ AGE 2006-08-22: $$tr$$
    pPositionXYZ_->setMinimumWidth( 195 );
    pPositionXYZ_->setAlignment( Qt::AlignCenter );

    pPositionMgrs_ = new QLabel( "---", parent );// $$$$ AGE 2006-08-22: $$tr$$
	pPositionMgrs_->setMinimumWidth( 105 );
    pPositionMgrs_->setAlignment( Qt::AlignCenter );

    pPositionLatLong_ = new QLabel( "---", parent );// $$$$ AGE 2006-08-22: $$tr$$
	pPositionLatLong_->setMinimumWidth( 125 );
    pPositionLatLong_->setAlignment( Qt::AlignCenter );

    parent->addWidget( pPositionXYZ_    , 0, true );
	parent->addWidget( pPositionMgrs_   , 0, true );
	parent->addWidget( pPositionLatLong_, 0, true );
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
        pPositionXYZ_->setText( "---" );// $$$$ AGE 2006-08-22: $$tr$$
		pPositionMgrs_->setText( "---" );// $$$$ AGE 2006-08-22: $$tr$$
        pPositionLatLong_->setText( "---" );// $$$$ AGE 2006-08-22: $$tr$$
	}
	else
    {
        // $$$$ AGE 2006-08-22: $$tr$$
        pPositionXYZ_->setText( QString( "x:%1 y:%2 h:%3" ).arg( position.X(), 2 )
                                                           .arg( position.Y(), 2 )
                                                           .arg( detection_.ElevationAt( position ) ) );
        pPositionMgrs_->setText( converter_.ConvertToMgrs( position ).c_str() );

        const geometry::Point2f latLong( converter_.ConvertToGeo( position ) );
        // $$$$ AGE 2006-08-22: $$tr$$
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
