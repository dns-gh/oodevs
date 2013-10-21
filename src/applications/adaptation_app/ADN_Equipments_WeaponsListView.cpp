// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_WeaponsListView.h"
#include "moc_ADN_Equipments_WeaponsListView.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Workspace.h"
#include "ADN_WorkspaceElement.h"

typedef ADN_Equipments_Data::WeaponInfos WeaponInfos;

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_WeaponsListView constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_WeaponsListView::ADN_Equipments_WeaponsListView( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_Equipments_WeaponsListView", tr( "Weapon systems" ) )
{
    setMinimumHeight( 115 );
    setMaximumHeight( 115 );
    pConnector_.reset( new ADN_Connector_ListView< WeaponInfos >( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_WeaponsListView destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Equipments_WeaponsListView::~ADN_Equipments_WeaponsListView()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_WeaponsListView::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_Equipments_WeaponsListView::ConnectItem( bool /*bConnect*/ )
{
    // Nothing
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_WeaponsListView::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
void ADN_Equipments_WeaponsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available weapons to the 'add weapons' submenu.
    auto& vWeapons = ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos();
    for( auto it = vWeapons.begin(); it != vWeapons.end(); ++it )
    {
        ADN_Weapons_Data_WeaponInfos* pWeapon = *it;
        // Don't add a weapon to the menu if it already is present in the list.
        if( Contains( pWeapon ) )
            continue;
        addMenu.insertItem( pWeapon->strName_.GetData().c_str(), static_cast< int >( 2 + std::distance( vWeapons.begin(), it ) ) );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add weapon" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove weapon" ), 1 );

    int nMenuResult = popupMenu.exec( pt );
    if( nMenuResult == 1 )
    {
        // Remove the weapon from the list.
        WeaponInfos* pCurWeapon = (WeaponInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ ).RemItem(pCurWeapon);
    }
    else if( nMenuResult > 1 )
    {
        int nIndex = nMenuResult - 2;
        assert( nIndex < (int)vWeapons.size() );

        // Add the weapon to the list.
        WeaponInfos* pNewInfo = new WeaponInfos( * vWeapons[nIndex] );

        ADN_Connector_Vector_ABC& pCTable = static_cast< ADN_Connector_Vector_ABC& >( *pConnector_ );
        pCTable.AddItem( pNewInfo );
        if( ADN_ListViewItem* item = FindItem( pNewInfo ) )
            selectionModel()->setCurrentIndex( dataModel_.indexFromItem( item ), QItemSelectionModel::ClearAndSelect );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Equipments_WeaponsListView::Contains
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
bool ADN_Equipments_WeaponsListView::Contains( const ADN_Weapons_Data_WeaponInfos* pInfo )
{
    const int rowCount = dataModel_.rowCount();
    for( int row = 0; row < rowCount; ++row )
    {
        ADN_ListViewItem* pCurr = static_cast< ADN_ListViewItem* >( dataModel_.item( row ) );
        ADN_Equipments_Data::WeaponInfos* pData = static_cast< ADN_Equipments_Data::WeaponInfos* >( pCurr->GetData() );
        if( pData->GetCrossedElement() == pInfo )
            return true;
    }
    return false;
}

