//*****************************************************************************
// 
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_List.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 8 $
// $Workfile: MOS_EnemyInfo_List.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_EnemyInfo_List.h"
#include "moc_MOS_EnemyInfo_List.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_EnemyInfo_List.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_EnemyInfo_Editor.h"
#include "MOS_EnemyInfo.h"
#include "MOS_EnemyInfo_ListItem.h"
#include "MOS_EnemyInfo_Sending.h"

#include <qwidgetstack.h>
#include <qheader.h>

MT_IdentifierManager MOS_EnemyInfo_List::idManager_;

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List constructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_List::MOS_EnemyInfo_List( QWidget* pParent )
: QListView( pParent )
, pEditor_( new MOS_EnemyInfo_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() ) )
, pSender_( new MOS_EnemyInfo_Sending( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() ) )
, bDrawRensEni_( false )
{
    this->addColumn( "Rens Eni" );
    header()->hide();
    setMargin( 5 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );    

    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List destructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_List::~MOS_EnemyInfo_List()
{
    MT_DELETEOWNED( enemyInfos_ );
    delete pEditor_;
    delete pSender_;
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List::SlotContextMenuRequested
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_List::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    QPopupMenu* pMenu = new QPopupMenu( this );
    if( pItem == 0 )
    {
        pMenu->insertItem( "Ajoute rens", 0 );
    }
    else
    {
        pMenu->insertItem( "Edite rens", 1 );
        pMenu->insertItem( "Efface rens", 2 );
        pMenu->insertSeparator( 3 );
        pMenu->insertItem( "Envoi", 4 );
    }

    int nSelected = pMenu->exec( point );
    delete pMenu;
    switch( nSelected )
    {
    case 0:
        {
            // Create a new enemy info
            MOS_EnemyInfo* pNewInfo = new MOS_EnemyInfo( idManager_.GetFreeIdentifier() );
            enemyInfos_.push_back( pNewInfo );

            new MOS_EnemyInfo_ListItem( this, std::string( "Unité " ) + itostring( pNewInfo->nId_ ), *pNewInfo );
            break;
        }
    case 1:
        {
            // Edit an existing information
            MOS_EnemyInfo_ListItem* pMosItem = static_cast< MOS_EnemyInfo_ListItem* >( pItem );
            assert( pMosItem != 0 );

            MOS_EnemyInfo& info = pMosItem->GetInfo();

            pEditor_->SetEnemyInfo( & info );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case 2:
        {
            // delete an info
            MOS_EnemyInfo_ListItem* pMosItem = static_cast< MOS_EnemyInfo_ListItem* >( pItem );
            assert( pMosItem != 0 );

            MOS_EnemyInfo& info = pMosItem->GetInfo();

            enemyInfos_.erase( std::find( enemyInfos_.begin(), enemyInfos_.end(), & info ) );
            idManager_.ReleaseIdentifier( info.nId_ );
            delete & info;
            takeItem( pMosItem );
            delete pMosItem;

            break;
        }
    case 4:
        {
            // Send info to automata
            // Edit an existing information
            MOS_EnemyInfo_ListItem* pMosItem = static_cast< MOS_EnemyInfo_ListItem* >( pItem );
            assert( pMosItem != 0 );

            MOS_EnemyInfo& info = pMosItem->GetInfo();
            pSender_->SetEnemyInfo( & info );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pSender_ );
            break;
        }
    default:
        {
            return;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List::SlotDrawRensEni
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_List::SlotDrawRensEni( bool bNewVal )
{
    bDrawRensEni_ = bNewVal;
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List::Draw
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_List::Draw()
{
    if( ! bDrawRensEni_ || pEditor_->IsEditing() )
        return;

    for( IT_EnemyInfo_Vector it = enemyInfos_.begin(); it != enemyInfos_.end(); ++it )
    {
        (*it)->Draw();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_List::NotifyDeleteAgent
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_List::NotifyDeleteAgent( MOS_Agent& agent )
{
    for( IT_EnemyInfo_Vector it = enemyInfos_.begin(); it != enemyInfos_.end(); ++it )
        (*it)->RemoveReferecingAutomata( agent );
}

