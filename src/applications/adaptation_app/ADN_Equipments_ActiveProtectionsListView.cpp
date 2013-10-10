// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_ActiveProtectionsListView.h"
#include "moc_ADN_Equipments_ActiveProtectionsListView.cpp"

#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"

#include <Qt3Support/q3popupmenu.h>

typedef ADN_Equipments_Data::ActiveProtectionsInfos ActiveProtectionsInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_ActiveProtectionsListView constructor
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
ADN_Equipments_ActiveProtectionsListView::ADN_Equipments_ActiveProtectionsListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Equipments_ActiveProtectionsListView", tr( "ActiveProtections" ) )
{
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<ActiveProtectionsInfos>(*this);

    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_ActiveProtectionsListView destructor
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
ADN_Equipments_ActiveProtectionsListView::~ADN_Equipments_ActiveProtectionsListView()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_ActiveProtectionsListView::ConnectItem
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
void ADN_Equipments_ActiveProtectionsListView::ConnectItem( bool )
{
    // Nothing
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_ActiveProtectionsListView::OnContextMenu
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
void ADN_Equipments_ActiveProtectionsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available ActiveProtections to the 'add ActiveProtections' submenu.
    ADN_ActiveProtections_Data::T_ActiveProtectionsInfosVector& vActiveProtections = ADN_Workspace::GetWorkspace().GetActiveProtections().GetData().GetActiveProtectionsInfos();
    for( auto it = vActiveProtections.begin(); it != vActiveProtections.end(); ++it )
    {
        ADN_ActiveProtections_Data::ActiveProtectionsInfos* pActiveProtections = *it;
        // Don't add a ActiveProtections to the menu if it already is present in the list.
        if( Contains( pActiveProtections ) )
            continue;
        addMenu.insertItem( pActiveProtections->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vActiveProtections.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add active protections" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove active protections" ), 1 );

    int nMenuResult = popupMenu.exec( pt );
    if( nMenuResult == 1 )
    {
        // Remove the ActiveProtections from the list.
        ActiveProtectionsInfos* pCurActiveProtections = (ActiveProtectionsInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurActiveProtections);
    }
    else if( nMenuResult > 1 )
    {
        int nIndex = nMenuResult - 2;
        assert( nIndex < (int)vActiveProtections.size() );

        // Add the ActiveProtections to the list.
        ActiveProtectionsInfos* pNewInfo = new ActiveProtectionsInfos( * vActiveProtections[nIndex] );

        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_ActiveProtectionsListView::Contains
// Created: FDS 10-02-24
//-----------------------------------------------------------------------------
bool ADN_Equipments_ActiveProtectionsListView::Contains( const ADN_ActiveProtections_Data::ActiveProtectionsInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Equipments_Data::ActiveProtectionsInfos* pData = static_cast< ADN_Equipments_Data::ActiveProtectionsInfos* >( pCurr->GetData() );
        if( pData->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}

