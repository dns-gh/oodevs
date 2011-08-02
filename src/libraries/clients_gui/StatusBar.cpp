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
#include "TerrainPicker.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/CoordinateSystems.h"
#include "clients_kernel/Tools.h"
#include "ENT/Ent_Tr.h"
#include <boost/format.hpp>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: StatusBar constructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::StatusBar( QStatusBar* parent, TerrainPicker& picker, const DetectionMap& detection, const CoordinateConverter_ABC& converter )
    : detection_( detection )
    , converter_( converter )
    , terrainPicker_( picker )
{
    QToolButton* toolButton = new QToolButton( parent );
    toolButton->setPopupDelay( 0 );
    toolButton->adjustSize();
    parent->addWidget( toolButton, 0, true );
    pMenu_ = new QMenu( toolButton );
    toolButton->setPopup( pMenu_ );
    pMenu_->menuAction()->setCheckable( true );

    AddField( parent, 155, CoordinateSystems::E_Local, true );
    AddField( parent, 105, CoordinateSystems::E_Mgrs, true );
    AddField( parent, 155, CoordinateSystems::E_Wgs84Dd, true );
    AddField( parent, 215, CoordinateSystems::E_Wgs84Dms, false );
    pMenu_->insertSeparator();
    pElevation_   = AddField( parent, 50, tr( "Elevation" ), true );
    pTerrainType_ = AddField( parent, 150, tr( "Terrain type" ), true );
    pObjectInfos_ = AddField( parent, 150, tr( "Object infos" ), true );

    connect( pMenu_, SIGNAL( activated( int ) ), this, SLOT( ParameterSelected( int ) ) );
    connect( &terrainPicker_, SIGNAL( TerrainPicked( const QString& ) ), SLOT( TerrainPicked( const QString& ) ) );
    connect( &terrainPicker_, SIGNAL( ObjectPicked( const QStringList& ) ), SLOT( ObjectPicked( const QStringList& ) ) );
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
// Name: StatusBar::AddField
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
QLabel* StatusBar::AddField( QStatusBar* parent, unsigned int size, const QString& title, bool checked )
{
    QLabel* field = new QLabel( tr( "---" ), parent );
    field->setMinimumWidth( size );
    field->setAlignment( Qt::AlignCenter );
    field->hide();
    parent->addWidget( field, 0, true );
    menuFields_.push_back( field );
    const int id = pMenu_->insertItem( title, static_cast< int >( menuFields_.size() ) );
    if( checked )
        ParameterSelected( id );
    return field;
}

// -----------------------------------------------------------------------------
// Name: StatusBar::AddField
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
QLabel* StatusBar::AddField( QStatusBar* parent, unsigned int size, int id, bool checked )
{
    kernel::CoordinateSystems::CIT_spatialReference it = converter_.GetCoordSystem().systems_.find( id );
    if( it != converter_.GetCoordSystem().systems_.end() )
    {   
        QLabel* field = AddField( parent, size, it->second->c_str(), checked );
        coordinateFields_[id] = field;
        return field;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: StatusBar::OnMouseMove
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
void StatusBar::OnMouseMove( const geometry::Point2f& position )
{
    if( !converter_.IsInBoundaries( position ) )
        for( T_MenuFields::iterator it = menuFields_.begin(); it != menuFields_.end(); ++it )
            (*it)->setText( tr( "---" ) );
    else
    {
        const QString xypos = tr( "y:%1 x:%2" ).arg( position.Y(), 4 ).arg( position.X(), 4 );
        coordinateFields_[ CoordinateSystems::E_Local ]->setText( xypos );

        const QString elev = tr( "h:%1 " ).arg( detection_.ElevationAt( position ) );
        pElevation_->setText( elev );

        coordinateFields_[ CoordinateSystems::E_Mgrs ]->setText( converter_.ConvertToMgrs( position ).c_str() );

        const geometry::Point2d latLong( converter_.ConvertToGeo( position ) );
        const QString latlongpos = tr( "Lat:%1 Lon:%2" ).arg( latLong.Y(), 0, 'g', 6 )
                                                        .arg( latLong.X(), 0, 'g', 6 );
        coordinateFields_[ CoordinateSystems::E_Wgs84Dd ]->setText( latlongpos );

        std::string pos( converter_.ConvertToGeoDms( position ) );
        std::string::size_type loc = pos.find( ":", 0 );
        if( loc != std::string::npos )
        {
            const std::string latlongdmspos( boost::str( boost::format( "Lat:%s, Lon:%s" )     % pos.substr( 0, loc )
                                                                                            % pos.substr( loc + 1, pos.size() - loc ) ) );
            coordinateFields_[ CoordinateSystems::E_Wgs84Dms ]->setText( latlongdmspos.c_str() );
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
    QLabel* field = menuFields_[index - 1];
    pMenu_->setItemChecked( index, field->isHidden() );
    if( field )
        field->setShown( field->isHidden() );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::TerrainPicked
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void StatusBar::TerrainPicked( const QString& type )
{
    pTerrainType_->setText( ENT_Tr::ConvertFromLocation( ENT_Tr::ConvertToLocation( type.ascii() ), ENT_Tr::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::BurningCellPicked
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
void StatusBar::ObjectPicked( const QStringList& infos )
{
    pObjectInfos_->setText( infos.join( " " ) );
}
