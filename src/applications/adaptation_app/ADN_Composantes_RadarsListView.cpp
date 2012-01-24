//*****************************************************************************
//
// $Created: JDY 03-07-22 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Composantes_RadarsListView.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 15:29 $
// $Revision: 2 $
// $Workfile: ADN_Composantes_RadarsListView.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Composantes_RadarsListView.h"
#include "moc_ADN_Composantes_RadarsListView.cpp"

#include <Qt3Support/q3popupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Workspace.h"

typedef ADN_Composantes_Data::RadarInfos RadarInfos;


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_RadarsListView constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_RadarsListView::ADN_Composantes_RadarsListView( QWidget* pParent, const char* szName, Qt::WFlags f )
: ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Special sensors" ) );

    setMinimumHeight( 115 );
    setMaximumHeight( 115 );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<RadarInfos>(*this);

    SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_RadarsListView destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_Composantes_RadarsListView::~ADN_Composantes_RadarsListView()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_RadarsListView::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_Composantes_RadarsListView::ConnectItem( bool /*bConnect*/ )
{
    // Nothing
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_RadarsListView::OnContextMenu
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
void ADN_Composantes_RadarsListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    Q3PopupMenu addMenu( this );

    // Add the available radars to the 'add radar' submenu.
    ADN_Radars_Data::T_RadarInfos_Vector& vRadars = ADN_Workspace::GetWorkspace().GetSensors().GetData().radarData_.vRadars_;
    for( ADN_Radars_Data::IT_RadarInfos_Vector it = vRadars.begin(); it != vRadars.end(); ++it )
    {
        ADN_Radars_Data::RadarInfos* pRadar = *it;
        // Don't add a radar to the menu if it already is present in the list.
        if( Contains( pRadar ) )
            continue;
        addMenu.insertItem( pRadar->strName_.GetData().c_str(), (int)pRadar );
    }
    ADN_Tools::SortMenu( addMenu );

    popupMenu.insertItem( tr( "Add sensor" ), &addMenu, 0 );
    if( pCurData_ != 0 )
        popupMenu.insertItem( tr( "Remove sensor" ), 1 );

    int nMenuResult = popupMenu.exec( pt );
    if( nMenuResult == 1 )
    {
        // Remove the radar from the list.
        RadarInfos* pCurrent = (RadarInfos*)pCurData_;
        static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurrent);
    }
    else if( nMenuResult > 1 )
    {
        // Add the weapon to the list.
        RadarInfos* pNewInfo = new RadarInfos();
        pNewInfo->ptrRadar_ = (ADN_Radars_Data::RadarInfos*)nMenuResult;
        pNewInfo->strName_ = ((ADN_Radars_Data::RadarInfos*)nMenuResult)->strName_.GetData();

        ADN_Connector_Vector_ABC* pCTable = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
        pCTable->AddItem( pNewInfo );
        setCurrentItem( FindItem( pNewInfo ) );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Composantes_RadarsListView::Contains
// Created: AGN 03-08-01
//-----------------------------------------------------------------------------
bool ADN_Composantes_RadarsListView::Contains( const ADN_Radars_Data::RadarInfos* pInfo )
{
    Q3ListViewItemIterator it( this );
    while( it.current() != 0 )
    {
        ADN_ListViewItem* pCurr = (ADN_ListViewItem*)it.current();
        ADN_Composantes_Data::RadarInfos* pData = static_cast< ADN_Composantes_Data::RadarInfos* >( pCurr->GetData() );
        if( pData->ptrRadar_.GetData() == pInfo )
            return true;
        ++it;
    }
    return false;
}

