// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::StatusBar */

#include "clients_gui_pch.h"
#include "StatusBar.h"
#include "moc_StatusBar.cpp"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include <qstatusbar.h>
#include <boost/format.hpp>

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
	pButtonBarParameters_ = new QToolButton( parent );
    pButtonBarParameters_->setPopupDelay( 0 );
    pButtonBarParameters_->adjustSize();
   
    pPositionXY_ = new QLabel( NotSet(), parent );
    pPositionXY_->setMinimumWidth( 155 );
    pPositionXY_->setAlignment( Qt::AlignCenter );
    pPositionXY_->hide();

	pElevation_ = new QLabel( NotSet(), parent );
	pElevation_->setMinimumWidth( 50 );
    pElevation_->setAlignment( Qt::AlignCenter );
    pElevation_->hide();

    pPositionMgrs_ = new QLabel( NotSet(), parent );
	pPositionMgrs_->setMinimumWidth( 105 );
    pPositionMgrs_->setAlignment( Qt::AlignCenter );
    pPositionMgrs_->hide();

    pPositionLatLong_ = new QLabel( NotSet(), parent );
	pPositionLatLong_->setMinimumWidth( 155 );
    pPositionLatLong_->setAlignment( Qt::AlignCenter );
    pPositionLatLong_->hide();

	pPositionDms_ = new QLabel( NotSet(), parent );
	pPositionDms_->setMinimumWidth( 215 );
    pPositionDms_->setAlignment( Qt::AlignCenter );
    pPositionDms_->hide();

    parent->addWidget( pPositionXY_		, 0, true );
    parent->addWidget( pElevation_		, 0, true );
	parent->addWidget( pPositionMgrs_   , 0, true );
	parent->addWidget( pPositionLatLong_, 0, true );
	parent->addWidget( pPositionDms_	, 0, true );
	parent->addWidget( pButtonBarParameters_, 0, true );

	pMenu_ = new QPopupMenu( pButtonBarParameters_ );
	pButtonBarParameters_->setPopup( pMenu_ );
	connect( pMenu_, SIGNAL( activated( int ) ), this, SLOT( ParameterSelected( int ) ) );
	
	Init();
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
// Name: StatusBar::NotSet
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
QString StatusBar::NotSet()
{
    static const QString notSet = tr( "---" );
    return notSet;
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnMouseMove
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::OnMouseMove( const geometry::Point2f& position )
{
    if( !converter_.IsInBoundaries( position ) )
    {
        pPositionXY_->setText( NotSet() );
		pPositionMgrs_->setText( NotSet() );
        pPositionLatLong_->setText( NotSet() );
        pElevation_->setText( NotSet() );
        pPositionDms_->setText( NotSet() );
	}
	else
    {
        const QString xypos = tr( "y:%1 x:%2" ).arg( position.Y(), 4 ).arg( position.X(), 4 );

        pPositionXY_->setText( xypos );
		
		const QString elev = tr( "h:%1 " ).arg( detection_.ElevationAt( position ) );
		pElevation_->setText( elev );

        pPositionMgrs_->setText( converter_.ConvertToMgrs( position ).c_str() );

        const geometry::Point2d latLong( converter_.ConvertToGeo( position ) );
        const QString latlongpos = tr( "Lat:%1 Lon:%2" ).arg( latLong.Y(), 0, 'g', 6 )
                                                        .arg( latLong.X(), 0, 'g', 6 );
		pPositionLatLong_->setText( latlongpos );

		std::string pos( converter_.ConvertToGeoDms( position ) );
        std::string::size_type loc = pos.find( ":", 0 );
		if (loc != std::string::npos ) 
		{
			const std::string latlongdmspos( boost::str( boost::format( "Lat:%s, Lon:%s" ) 	% pos.substr( 0, loc )
																						    % pos.substr( loc + 1, pos.size() - loc ) ) );			
			pPositionDms_->setText( latlongdmspos.c_str() );
		}

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
// Name: StatusBar::ParameterSelected
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
void StatusBar::ParameterSelected( int index )
{
	pMenu_->setItemChecked( index, !pMenu_->isItemChecked( index ) );
	switch( index )
	{
        case CoordinateSystems::E_Local:		DisplayParameter( *pPositionXY_ );		break;
        case CoordinateSystems::E_Mgrs:		    DisplayParameter( *pPositionMgrs_ );	break;
        case CoordinateSystems::E_Wgs84Dd:		DisplayParameter( *pPositionLatLong_ );	break;
        case CoordinateSystems::E_Wgs84Dms: 	DisplayParameter( *pPositionDms_ );	    break;
		default:                                DisplayParameter( *pElevation_ );		break;
	}
}
// -----------------------------------------------------------------------------
// Name: StatusBar::Init
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
void StatusBar::Init()
{
	pMenu_->setCheckable( true );

    for( kernel::CoordinateSystems::CIT_spatialReference it = converter_.GetCoordSystem().systems_.begin(); it != converter_.GetCoordSystem().systems_.end(); it++ )
    {
        pMenu_->insertItem( it->second->c_str(), it->first );
        if ( it->first != CoordinateSystems::E_Wgs84Dms )
            ParameterSelected( it->first );
    }

    ElevId_ = pMenu_->insertItem( tr( "Elevation" ) );
    ParameterSelected( ElevId_ );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::DisplayParameter
// Created: AME 2010-03-04
// -----------------------------------------------------------------------------
void StatusBar::DisplayParameter( QLabel& label )
{
	if ( label.isHidden() )
    	label.show();
    else
    	label.hide();
}
