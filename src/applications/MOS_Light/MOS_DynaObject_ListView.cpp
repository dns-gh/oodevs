//*****************************************************************************
// 
// $Created: AGN 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_ListView.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 16:54 $
// $Revision: 23 $
// $Workfile: MOS_DynaObject_ListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DynaObject_ListView.h"
#include "moc_MOS_DynaObject_ListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject_ListView.inl"
#endif

#include "MOS_DynaObject_ListViewItem.h"
#include "MOS_DynaObject.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_DynaObject_Editor.h"
#include "MOS_DynaObject_Creator.h"
#include "MOS_ASN_Messages.h"

#include <qwidgetstack.h>
#include <qheader.h>

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView constructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListView::MOS_DynaObject_ListView( QWidget* pParentWidget )
: QListView( pParentWidget )
{
    pDynaObjectEditor_  = new MOS_DynaObject_Editor ( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
    pDynaObjectCreator_ = new MOS_DynaObject_Creator( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );

    setSorting( -1 );
    header()->hide();
    addColumn( "Objets" );
    setRootIsDecorated( true );

    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( SlotOnClick( QListViewItem* ) ) );

    // Creates items for each type of objects
    for( uint i = 0; i < eNbrObjectType; ++i )
        itemObjectTypeMap_.insert( std::make_pair( (ASN1T_EnumObjectType)i, new QListViewItem( this, ENT_Tr::ConvertFromObjectType( (E_ObjectType)i ).c_str() ) ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView destructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListView::~MOS_DynaObject_ListView()
{
    delete pDynaObjectCreator_;
    delete pDynaObjectEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::AddObject
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::AddObject( MOS_DynaObject& object )
{
    IT_ObjectId_Item_Map found = itemObjectTypeMap_.find( object.GetType() );
    assert( found != itemObjectTypeMap_.end() && "Unknown object type" );

    new MOS_DynaObject_ListViewItem( found->second, object );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::RemoveObject
// Created: NLD 2003-07-29
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::RemoveObject( MOS_DynaObject& object )
{
    if( object.GetViewItem() )
        delete object.GetViewItem();
}



// =============================================================================
// SLOTS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::SlotOnClick
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::SlotOnClick( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    if( pItem->rtti() != MOS_DynaObject_ListViewItem::e_rtti )
        return;

    MOS_DynaObject_ListViewItem* pMyItem = static_cast< MOS_DynaObject_ListViewItem* >( pItem );
    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pMyItem->GetObject().GetCenter() );    
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::SlotContextMenuRequested
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    QPopupMenu* pMenu = new QPopupMenu( this );

    if( pItem && pItem->rtti() == MOS_DynaObject_ListViewItem::e_rtti )
    {
        pMenu->insertItem( "Edit object"   , 1 );
        pMenu->insertItem( "Destroy object", 2 );
    }
    else
        pMenu->insertItem( "Create object", 0 );

    int nSelected = pMenu->exec( point );
    delete pMenu;
    switch( nSelected )
    {
        case 0:
            {
                pDynaObjectCreator_->Initialize();
                MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pDynaObjectCreator_ );
                break;
            }
        case 1:
            {
                const MOS_DynaObject& object = static_cast< MOS_DynaObject_ListViewItem* >( pItem )->GetObject();
                pDynaObjectEditor_->SetObject( &object );
                MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pDynaObjectEditor_ );
                break;
            }
        case 2:
            {
                const MOS_DynaObject& object = static_cast< MOS_DynaObject_ListViewItem* >( pItem )->GetObject();
                MOS_ASN_MsgObjectMagicAction asnMsg;
                asnMsg.GetAsnMsg().oid_objet = object.GetID();
                asnMsg.GetAsnMsg().action.t  = T_MsgObjectMagicAction_action_destroy_object;
                asnMsg.Send( 546 );
                break;
            }
        default:
            return;
    }
}

