//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 24 $
// $Workfile: MOS_PointListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_PointListView.h"
#include "moc_MOS_PointListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_PointListView.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"

#include "MOS_PointListViewItem.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_PointListViewItem.h"

#include "MOS_LineManager.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_Agent.h"

#include "MOS_Net_Def.h"


using namespace DIN;


#include <QHeader.h>
#include <QSplitter.h>
#include <QListView.h>

//-----------------------------------------------------------------------------
// Name: MOS_PointListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListView::MOS_PointListView( QWidget* pParent, const std::string& strName, E_ListViewType nType )
    :   QListView           ( pParent )
    ,   MOS_GLMouse_ABC     ()
    ,   MOS_MissionTool_ABC ()
    ,   pSelectedPoint_     ( 0 )
    ,   bMovingPoint_       ( false )
    ,   nType_              ( nType )
    ,   bShift_             ( false )
    ,   bCtrl_              ( false )
{  
    setSorting( -1 );
    addColumn( strName.c_str() );
    connect( this, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );
    setRootIsDecorated( true );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListView::~MOS_PointListView()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_PointListView::Initialize()
{
    MOS_GLMouse_ABC::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_PointListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::DeleteAllPoints
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_PointListView::DeleteAllPoints()
{
    RIT_PointPtrVector itPoint = pointVector_.rbegin();
    while ( itPoint != pointVector_.rend()  )
    {
        DeletePoint( **itPoint );
        itPoint = pointVector_.rbegin();
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::CreatePoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem& MOS_PointListView::CreatePoint( const MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter )
{
    MT_Vector2D& vNewPos = * new MT_Vector2D( vPoint );      //$$$ RAM
    MOS_PointListViewItem* pPoint = 0;
    if ( pointVector_.size() )
    {
        if ( pPointAfter )
            pPoint = new MOS_PointListViewItem( eItemTypeEltPoint, vNewPos, *this, *pPointAfter );
        else
            pPoint = new MOS_PointListViewItem( eItemTypeEltPoint, vNewPos, *this, **pointVector_.rbegin() );
    }
    else
        pPoint = new MOS_PointListViewItem( eItemTypeEltPoint, vNewPos, *this );

    RegisterPoint( *pPoint, pPointAfter );
    return ( *pPoint );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::DeletePoint
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_PointListView::DeletePoint( MOS_PointListViewItem& point )
{
    delete &point.GetPoint();
    delete &point;
    UnregisterPoint( point );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::LocOnPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_PointListView::LocOnPoint( MOS_PointListViewItem* pPoint, bool bSelectList )
{
    if ( pPoint )
    {
        if ( bSelectList )
            setSelected( pPoint, true );
        MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pPoint->GetPoint() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_PointListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem ) 
        return;

    E_ItemType nType = eItemTypeNone;
    MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
    nType = pTmpItem->GetItemType();

    // Point
    if( nType == eItemTypeEltPoint )
    {
        pSelectedPoint_ = (MOS_PointListViewItem*)pItem;
        LocOnPoint( pSelectedPoint_, false );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::IsInsideLine
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_PointListView::IsInsideLine( const MT_Vector2D& vPos ) const
{
    MT_Float rWeldValue = 4.0;
    MOS_PointListViewItem* pLastPoint = 0;
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = (MOS_PointListViewItem*)*itPoint;
        if ( pLastPoint )
        {
            MT_Line line( pLastPoint->GetPoint(), pCurPoint->GetPoint() );
            MT_Vector2D vPosCol = line.ClosestPointOnLine( vPos );
            MT_Float rDistance = vPosCol.Distance( vPos );
            if(  rDistance < rWeldValue )
            {
                if ( pLastPoint )
                    return pLastPoint;
                return pCurPoint;
            }

        }
        pLastPoint = pCurPoint;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::SetPosPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_PointListView::SetPosPoint( MOS_PointListViewItem& point, const MT_Vector2D& vPos )
{
    point.SetPos( vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListView::CreatePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_PointListView::CreatePointFromInterface( MOS_PointListViewItem& point )
{
    MT_Vector2D vPos = GetCenterPoint( &point );
    MOS_PointListViewItem* pPoint = &CreatePoint( vPos , &point );
    return pPoint;
}



//-----------------------------------------------------------------------------
// Name: MOS_PointListView::GetPointBefore
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_PointListView::GetPointBefore( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.begin() )
    {
        return 0;
    }
    --itPoint;
    return *itPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::GetPointAfter
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_PointListView::GetPointAfter( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.rbegin() )
    {
        return 0;
    }
    ++itPoint;
    return *itPoint;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::GetFirstPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_PointListView::GetFirstPoint() const
{
    if ( pointVector_.size() )
        return *pointVector_.begin();
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_PointListView::keyPressEvent( QKeyEvent * e )
{
   if ( e->key() == Qt::Key_Shift )
   {
        bShift_ = true;        
   }
   else
   if ( e->key() == Qt::Key_Control )
   {
        bCtrl_ = true;        
   }
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_PointListView::keyReleaseEvent( QKeyEvent * e )
{
   if ( e->key() == Qt::Key_Shift )
   {
        bShift_ = false;        
   }
   else
   if ( e->key() == Qt::Key_Control )
   {
        bCtrl_ = false;        
   }
}

