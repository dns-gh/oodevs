//*****************************************************************************
// 
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_ListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_Meteo_ListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_Meteo_ListView.h"
#include "moc_MOS_Meteo_ListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_Meteo_ListView.inl"
#endif

#include "MOS_Meteo_Local.h"
#include "MOS_Meteo_ListViewItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_Meteo_Local_GUI.h"

#include <qwidgetstack.h>

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_ListView::MOS_Meteo_ListView( QWidget* pParent )
: QListView( pParent )
, pWeatherEditor_( 0 )
{
    addColumn( "Météo locale" );
    setResizeMode(QListView::AllColumns);

    connect( this, SIGNAL( rightButtonPressed( QListViewItem*, const QPoint&, int ) ), SLOT( OnContextMenu( QListViewItem*, const QPoint&, int ) ) );

    pWeatherEditor_ = new MOS_Meteo_Local_GUI( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
    pWeatherEditor_->Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_ListView::~MOS_Meteo_ListView()
{
    delete pWeatherEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView::OnContextMenu
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_ListView::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu * pMenu=new QPopupMenu(this);
    pMenu->insertItem( "Ajouter météo locale", eMenu_AddWeather );

    if( pItem != 0 )
    {
        pMenu->insertSeparator();
        pMenu->insertItem( "Editer météo locale", eMenu_UpdateWeather );
        pMenu->insertItem( "Effacer météo locale", eMenu_RemoveWeather );
    }

    int nResult = pMenu->exec( pos );

    if( nResult < 0 )
        return;

    switch( nResult )
    {
    case eMenu_AddWeather:
        {
            MOS_Meteo_Local& localWeather = MOS_App::GetApp().GetWeatherManager().CreateLocalWeather();
            new MOS_Meteo_ListViewItem( this, localWeather );
            break;
        }
    case eMenu_UpdateWeather:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_Meteo_ListViewItem::e_rtti );
            MOS_Meteo_ListViewItem* pMItem = static_cast< MOS_Meteo_ListViewItem* >( pItem );

            pWeatherEditor_->SetWeather( & pMItem->GetLocalWeather() );

            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pWeatherEditor_ );

            break;
        }
    case eMenu_RemoveWeather:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_Meteo_ListViewItem::e_rtti );

            MOS_Meteo_ListViewItem* pMItem = static_cast< MOS_Meteo_ListViewItem* >( pItem );
            MOS_App::GetApp().GetWeatherManager().DeleteLocalWeather( pMItem->GetLocalWeather() );
            delete & (pMItem->GetLocalWeather());
            takeItem( pMItem );
            break;
        }
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_Meteo_ListView::FillRandomParameters( )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView::ReloadParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_Meteo_ListView::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListView::ResaveParameters
// Created: FBD 03-09-09
//-----------------------------------------------------------------------------
void MOS_Meteo_ListView::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
    
}



