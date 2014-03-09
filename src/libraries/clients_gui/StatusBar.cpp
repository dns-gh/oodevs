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
#include "RichWidget.h"
#include "SubObjectName.h"
#include "TerrainPicker.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "ENT/Ent_Tr.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace kernel;
using namespace gui;

namespace
{
    const QString statusBarCoordinates = "/Common/StatusBarCoordinates";
    QString convertFromCoordSystem( E_CoordinateSystem coordSystem )
    {
        return QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( coordSystem, ENT_Tr::eToSim ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusBar constructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::StatusBar( kernel::Controllers& controllers, QStatusBar* parent, TerrainPicker& picker, const DetectionMap& detection, const CoordinateConverter_ABC& converter, QObject& selector )
    : controllers_  ( controllers )
    , detection_    ( detection )
    , converter_    ( converter )
    , terrainPicker_( picker )
    , parent_       ( parent )
{
    SubObjectName subObject( "statusBar" );
    RichWidget< QToolButton >* toolButton = new RichWidget< QToolButton >( "toolButton", parent );
    toolButton->setPopupDelay( 0 );
    toolButton->adjustSize();
    parent->addWidget( toolButton, 0, true );
    pMenu_ = new QMenu( toolButton );
    toolButton->setPopup( pMenu_ );
    pMenu_->menuAction()->setCheckable( true );

    QSettings settings( "MASA Group", "SWORD" );
    const QStringList fields = settings.value( statusBarCoordinates, QStringList()
        << convertFromCoordSystem( eCoordinateSystem_Local )
        << convertFromCoordSystem( eCoordinateSystem_Mgrs )
        << convertFromCoordSystem( eCoordinateSystem_Wgs84Dd )
        ).toStringList();
    const auto addField = [&]( unsigned size, E_CoordinateSystem coordSystem ){
        const bool checked = !!fields.contains( convertFromCoordSystem( coordSystem ) );
        AddField( parent, size, coordSystem, checked );
    };
    addField( 155, eCoordinateSystem_Local );
    addField( 105, eCoordinateSystem_Mgrs );
    addField( 105, eCoordinateSystem_SanC );
    addField( 155, eCoordinateSystem_Wgs84Dd );
    addField( 215, eCoordinateSystem_Wgs84Dms );
    pMenu_->insertSeparator();
    pElevation_   = AddField( parent, 50, tr( "Elevation" ), true );
    pTerrainType_ = AddField( parent, 150, tr( "Terrain type" ), true );
    pObjectInfos_ = AddField( parent, 150, tr( "Object infos" ), true );

    connect( pMenu_, SIGNAL( activated( int ) ), this, SLOT( ParameterSelected( int ) ) );
    connect( &terrainPicker_, SIGNAL( TerrainPicked( const QString& ) ), SLOT( TerrainPicked( const QString& ) ) );
    connect( &terrainPicker_, SIGNAL( ObjectPicked( const QStringList& ) ), SLOT( ObjectPicked( const QStringList& ) ) );
    connect( &selector, SIGNAL( MouseMove( const geometry::Point2f& ) ), this, SLOT( OnMouseMove( const geometry::Point2f& ) ) );
    connect( &selector, SIGNAL( MouseMove( const geometry::Point3f& ) ), this, SLOT( OnMouseMove( const geometry::Point3f& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: StatusBar destructor
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
StatusBar::~StatusBar()
{
    controllers_.Unregister( *this );
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
        ParameterSelected( id, false );
    return field;
}

// -----------------------------------------------------------------------------
// Name: StatusBar::AddField
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
QLabel* StatusBar::AddField( QStatusBar* parent, unsigned int size, E_CoordinateSystem coordSystem, bool checked )
{
    auto field = AddField( parent, size, QString::fromStdString( ENT_Tr::ConvertFromCoordinateSystem( coordSystem ) ), checked );
    coordinateFields_[ coordSystem ] = field;
    return field;
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
        const QString xypos = tr( "y:%L1 x:%L2" ).arg( position.Y(), 4 ).arg( position.X(), 4 );
        coordinateFields_[ eCoordinateSystem_Local ]->setText( xypos );

        const QString elev = tr( "h:%L1 " ).arg( detection_.ElevationAt( position ) );
        pElevation_->setText( elev );

        coordinateFields_[ eCoordinateSystem_Mgrs ]->setText( converter_.ConvertToMgrs( position ).c_str() );
        auto sad69 = converter_.ConvertTo( position, "SAN-C" );
        try
        {
            converter_.ConvertFrom( sad69, "SAN-C" );
        }
        catch( ... )
        {
            sad69 = tr( "invalid" );
        }
        coordinateFields_[ eCoordinateSystem_SanC ]->setText( QString::fromStdString( sad69 ) );

        const geometry::Point2d latLong( converter_.ConvertToGeo( position ) );
        const QString latlongpos = tr( "Lat:%L1 Lon:%L2" ).arg( latLong.Y(), 0, 'g', 6 )
                                                        .arg( latLong.X(), 0, 'g', 6 );
        coordinateFields_[ eCoordinateSystem_Wgs84Dd ]->setText( latlongpos );

        std::string pos( converter_.ConvertToGeoDms( position ) );
        std::string::size_type loc = pos.find( ":", 0 );
        if( loc != std::string::npos )
        {
            const std::string latlongdmspos( boost::str( boost::format( "Lat:%s, Lon:%s" )  % pos.substr( 0, loc )
                                                                                            % pos.substr( loc + 1, pos.size() - loc ) ) );
            coordinateFields_[ eCoordinateSystem_Wgs84Dms ]->setText( latlongdmspos.c_str() );
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
void StatusBar::ParameterSelected( int index, bool save )
{
    QLabel* field = menuFields_[index - 1];
    if( !field )
        return;
    pMenu_->setItemChecked( index, field->isHidden() );
    field->setVisible( field->isHidden() );
    if( save )
        SaveSettings();
}

// -----------------------------------------------------------------------------
// Name: StatusBar::ParameterSelected
// Created: BAX 2014-02-17
// -----------------------------------------------------------------------------
void StatusBar::ParameterSelected( int index )
{
    ParameterSelected( index, true );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::TerrainPicked
// Created: SBO 2010-03-26
// -----------------------------------------------------------------------------
void StatusBar::TerrainPicked( const QString& type )
{
    pTerrainType_->setText( ENT_Tr::ConvertFromLocation( ENT_Tr::ConvertToLocation( type.toStdString() ), ENT_Tr::eToTr ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::BurningCellPicked
// Created: BCI 2011-01-05
// -----------------------------------------------------------------------------
void StatusBar::ObjectPicked( const QStringList& infos )
{
    pObjectInfos_->setText( infos.join( " " ) );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::SetVisible
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void StatusBar::SetVisible( bool visible )
{
    parent_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::ForceEnabled
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void StatusBar::ForceEnabled( bool enabled )
{
    parent_->setEnabled( enabled );
    parent_->setVisible( enabled );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::EnsureIsEnabled
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
void StatusBar::EnsureIsEnabled()
{
    parent_->setEnabled( true );
    parent_->setVisible( true );
}

// -----------------------------------------------------------------------------
// Name: StatusBar::IsVisible
// Created: ABR 2012-05-29
// -----------------------------------------------------------------------------
bool StatusBar::IsVisible() const
{
    return parent_->isVisible();
}

// -----------------------------------------------------------------------------
// Name: StatusBar::SaveSettings
// Created: BAX 2014-02-13
// -----------------------------------------------------------------------------
void StatusBar::SaveSettings()
{
    QSettings settings( "MASA Group", "SWORD" );
    QStringList list;
    for( auto it = coordinateFields_.begin(); it != coordinateFields_.end(); ++it )
        if( it->second->isVisible() )
        {
            const auto proj = convertFromCoordSystem( static_cast< E_CoordinateSystem >( it->first ) );
            if( !proj.isEmpty() )
                list.push_back( proj );
        }
    settings.setValue( statusBarCoordinates, list );
}
