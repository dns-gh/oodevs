//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Composantes_WeaponsListView.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:04 $
// $Revision: 6 $
// $Workfile: ADN_Composantes_WeaponsListView.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Composantes_WeaponsListView.h"
#include "moc_ADN_Composantes_WeaponsListView.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::WeaponInfos WeaponInfos;


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_WeaponsListView constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_WeaponsListView::ADN_Composantes_WeaponsListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Weapon systems" ) );
//    setResizeMode( QListView::AllColumns );

    setMinimumHeight( 115 );
    setMaximumHeight( 115 );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<WeaponInfos>(*this);

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_WeaponsListView destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_WeaponsListView::~ADN_Composantes_WeaponsListView()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_WeaponsListView::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_Composantes_WeaponsListView::ConnectItem( bool /*bConnect*/ )
{
    // Nothing
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_WeaponsListView::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
void ADN_Composantes_WeaponsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available weapons to the 'add weapons' submenu.
    ADN_Weapons_Data::T_WeaponInfosVector& vWeapons = ADN_Workspace::GetWorkspace().GetWeapons().GetData().GetWeaponInfos();
    for( ADN_Weapons_Data::IT_WeaponInfosVector it = vWeapons.begin(); it != vWeapons.end(); ++it )
    {
        ADN_Weapons_Data::WeaponInfos* pWeapon = *it;
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
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurWeapon);
    }
    else if( nMenuResult > 1 )
    {
        int nIndex = nMenuResult - 2;
        assert( nIndex < (int)vWeapons.size() );

        // Add the weapon to the list.
        WeaponInfos* pNewInfo = new WeaponInfos( * vWeapons[nIndex] );

        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        setCurrentItem( FindItem( pNewInfo ) );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_WeaponsListView::Contains
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
bool ADN_Composantes_WeaponsListView::Contains( const ADN_Weapons_Data::WeaponInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        ADN_Composantes_Data::WeaponInfos* pData = static_cast< ADN_Composantes_Data::WeaponInfos* >( pCurr->GetData() );
        if( pData->ptrWeapon_.GetData() == pInfo )
            return true;
        ++it;
    }
    return false;
}


