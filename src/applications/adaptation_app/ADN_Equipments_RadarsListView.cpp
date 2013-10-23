// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_RadarsListView.h"
#include "moc_ADN_Equipments_RadarsListView.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Equipments_Data::RadarInfos RadarInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_RadarsListView constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_RadarsListView::ADN_Equipments_RadarsListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Equipments_RadarsListView", tr( "Special sensors" ) )
{
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    pConnector_.reset( new ADN_Connector_ListView< RadarInfos >( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_RadarsListView destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_RadarsListView::~ADN_Equipments_RadarsListView()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_RadarsListView::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_Equipments_RadarsListView::ConnectItem( bool /*bConnect*/ )
{
    // Nothing
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_RadarsListView::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
void ADN_Equipments_RadarsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available radars to the 'add radar' submenu.
    ADN_Radars_Data::T_RadarInfos_Vector& vRadars = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_->vRadars_;
    for( auto it = vRadars.begin(); it != vRadars.end(); ++it )
    {
        ADN_Radars_Data::RadarInfos* pRadar = *it;
        // Don't add a radar to the menu if it already is present in the list.
        if( Contains( pRadar ) )
            continue;
        addMenu.insertItem( pRadar->strName_.GetData().c_str(), 2 + static_cast< int >( std::distance( vRadars.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add sensor" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove sensor" ), 1 );

    int nMenuResult = popupMenu.exec( pt );
    if( nMenuResult == 1 )
    {
        // Remove the radar from the list.
        RadarInfos* pCurrent = static_cast< RadarInfos* >( pCurData_ );
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem(pCurrent);
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        RadarInfos* pNewInfo = new RadarInfos();
        ADN_Radars_Data::RadarInfos* radarsInfos = vRadars[ nMenuResult - 2 ];
        pNewInfo->SetCrossedElement( radarsInfos );

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_RadarsListView::Contains
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
bool ADN_Equipments_RadarsListView::Contains( const ADN_Radars_Data::RadarInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Equipments_Data::RadarInfos* pData = static_cast< ADN_Equipments_Data::RadarInfos* >( pCurr->GetData() );
        if( pData->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}

