// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
// LTO
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_LimitedToSensorsListView.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Sensors_Data::LimitedToSensorsInfos LimitedToSensorsInfos;

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView constructor
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
ADN_Sensors_LimitedToSensorsListView::ADN_Sensors_LimitedToSensorsListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADNSensorsLimitedToSensorsListView", tools::translate( "ADN_Sensors_LimitedToSensorsListView", "Sensors" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< LimitedToSensorsInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView destructor
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
ADN_Sensors_LimitedToSensorsListView::~ADN_Sensors_LimitedToSensorsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView::OnContextMenu
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
void ADN_Sensors_LimitedToSensorsListView::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addSensorMenu( &popupMenu );
    Q3PopupMenu addRadarMenu( &popupMenu );

    std::vector< const ADN_RefWithLocalizedName* > menuItems;

    // Add the available sensors to the 'add sensor' submenu.
    ADN_Sensors_Data::T_SensorsInfos_Vector& vSensors = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();

    for( auto itSensor = vSensors.begin(); itSensor != vSensors.end(); ++itSensor )
    {
        ADN_Sensors_Data::SensorInfos* pSensor = *itSensor;
        // Don't add a sensor to the menu if it already is present in the list.
        if( Contains( pSensor ) )
            continue;
        menuItems.push_back( pSensor );
        addSensorMenu.insertItem( pSensor->strName_.GetData().c_str(), static_cast< int >( 2 + menuItems.size() ) );
    }

    ADN_Tools::SortMenu( addSensorMenu );

    // Add the available radars to the 'add special sensor' submenu.
    ADN_Radars_Data::T_RadarInfos_Vector& vRadars = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->vRadars_;
    for( auto itRadar = vRadars.begin(); itRadar != vRadars.end(); ++itRadar )
    {
        ADN_Radars_Data::RadarInfos* pRadar = *itRadar;
        // Don't add a radar to the menu if it already is present in the list.
        if( Contains( pRadar ) )
            continue;
        menuItems.push_back( pRadar );
        addRadarMenu.insertItem( pRadar->strName_.GetData().c_str(), static_cast< int >( 2 + menuItems.size() ) );
    }

    ADN_Tools::SortMenu( addRadarMenu );

    popupMenu.insertItem( tools::translate( "ADN_Sensors_LimitedToSensorsListView", "Add sensor" ), &addSensorMenu, 0 );
    popupMenu.insertItem( tools::translate( "ADN_Sensors_LimitedToSensorsListView", "Add special sensor" ), &addRadarMenu, 1 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tools::translate( "ADN_Sensors_LimitedToSensorsListView", "Remove item" ), 2 );

    int nMenuResult = popupMenu.exec( pt );

    if( nMenuResult == 2 )
    {
        // Remove the sensor from the list.
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem( pCurData_ );
    }
    else if( nMenuResult > 2 )
    {
        // Add the sensor to the list.
        ADN_Sensors_Data::LimitedToSensorsInfos* pNewInfo = new ADN_Sensors_Data::LimitedToSensorsInfos();
        pNewInfo->strName_ = menuItems[ nMenuResult - 3 ]->strName_.GetData();
        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView::Contains
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
bool ADN_Sensors_LimitedToSensorsListView::Contains( ADN_Sensors_Data::SensorInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Sensors_Data::LimitedToSensorsInfos* pData = static_cast< ADN_Sensors_Data::LimitedToSensorsInfos* >( pCurr->GetData() );

        if( pData->strName_.GetData() == pInfo->strName_.GetData() )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView::Contains
// Created: JSR 2010-03-18
// -----------------------------------------------------------------------------
bool ADN_Sensors_LimitedToSensorsListView::Contains( ADN_Radars_Data::RadarInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Sensors_Data::LimitedToSensorsInfos* pData = static_cast< ADN_Sensors_Data::LimitedToSensorsInfos* >( pCurr->GetData() );

        if( pData->strName_.GetData() == pInfo->strName_.GetData() )
            return true;
    }
    return false;
}
