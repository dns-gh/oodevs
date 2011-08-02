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

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Sensors_Data.h"
#include "ADN_Connector_ListView.h"

typedef ADN_Sensors_Data::LimitedToSensorsInfos LimitedToSensorsInfos;


// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView constructor
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
ADN_Sensors_LimitedToSensorsListView::ADN_Sensors_LimitedToSensorsListView( QWidget* pParent /* = 0*/, const char* szName /* = 0*/, Qt::WFlags f /* = 0*/ )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Sensors" ) );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<LimitedToSensorsInfos>(*this);

    this->SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView destructor
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
ADN_Sensors_LimitedToSensorsListView::~ADN_Sensors_LimitedToSensorsListView()
{
    delete pConnector_;
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

    // Add the available sensors to the 'add sensor' submenu.
    ADN_Sensors_Data::T_SensorsInfos_Vector& vSensors = ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsInfos();

    for( ADN_Sensors_Data::IT_SensorsInfos_Vector itSensor = vSensors.begin(); itSensor != vSensors.end(); ++itSensor )
    {
        ADN_Sensors_Data::SensorInfos* pSensor = *itSensor;
        // Don't add a sensor to the menu if it already is present in the list.
        if( Contains( pSensor ) )
            continue;
        addSensorMenu.insertItem( pSensor->strName_.GetData().c_str(), (int)pSensor );
    }

    ADN_Tools::SortMenu( addSensorMenu );

    // Add the available radars to the 'add special sensor' submenu.
    ADN_Radars_Data::T_RadarInfos_Vector& vRadars = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.vRadars_;
    for( ADN_Radars_Data::IT_RadarInfos_Vector itRadar = vRadars.begin(); itRadar != vRadars.end(); ++itRadar )
    {
        ADN_Radars_Data::RadarInfos* pRadar = *itRadar;
        // Don't add a radar to the menu if it already is present in the list.
        if( Contains( pRadar ) )
            continue;
        addRadarMenu.insertItem( pRadar->strName_.GetData().c_str(), (int)pRadar );
    }

    ADN_Tools::SortMenu( addRadarMenu );

    popupMenu.insertItem( tr( "Add sensor" ), &addSensorMenu, 0 );
    popupMenu.insertItem( tr( "Add special sensor" ), &addRadarMenu, 1 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove item" ), 2 );

    int nMenuResult = popupMenu.exec( pt );

    if( nMenuResult == 2 )
    {
        // Remove the sensor from the list.
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurData_ );
    }
    else if( nMenuResult > 1 )
    {
        // Add the sensor to the list.
        ADN_Sensors_Data::LimitedToSensorsInfos* pNewInfo = new ADN_Sensors_Data::LimitedToSensorsInfos();
        pNewInfo->strName_ = reinterpret_cast< ADN_Sensors_Data::LimitedToSensorsInfos* >( nMenuResult )->GetItemName();
        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        setCurrentItem( FindItem( pNewInfo ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView::Contains
// Created: JSR 2010-03-17
// -----------------------------------------------------------------------------
bool ADN_Sensors_LimitedToSensorsListView::Contains( ADN_Sensors_Data::SensorInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        ADN_Sensors_Data::LimitedToSensorsInfos* pData = static_cast< ADN_Sensors_Data::LimitedToSensorsInfos* >( pCurr->GetData() );

        if( pData->strName_.GetData() == pInfo->strName_.GetData() )
            return true;
        ++it;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_LimitedToSensorsListView::Contains
// Created: JSR 2010-03-18
// -----------------------------------------------------------------------------
bool ADN_Sensors_LimitedToSensorsListView::Contains( ADN_Radars_Data::RadarInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        ADN_Sensors_Data::LimitedToSensorsInfos* pData = static_cast< ADN_Sensors_Data::LimitedToSensorsInfos* >( pCurr->GetData() );

        if( pData->strName_.GetData() == pInfo->strName_.GetData() )
            return true;
        ++it;
    }
    return false;
}
