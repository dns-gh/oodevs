//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitListView.cpp $
// $Author: Nld $
// $Modtime: 20/01/05 11:46 $
// $Revision: 28 $
// $Workfile: MOS_LimitListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_LimitListView.h"
#include "moc_MOS_LimitListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimitListView.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"

#include "MOS_PointListViewItem.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_LimitListViewItem.h"

#include "MOS_LimitCheckListItem.h"
#include "MOS_CheckListItem_ABC.h"


#include "MOS_LineManager.h"
#include "MOS_Limit.h"
#include "MOS_LimitSelector.h"
#include "MOS_LimitEditor.h"
#include "MOS_LimitMission.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_Agent.h"

#include <QHeader.h>
#include <QSplitter.h>

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitListView::MOS_LimitListView( QWidget* pParent, E_ListViewType nType )
    :   QListView       ( pParent )
    ,   MOS_GLMouse_ABC ()
    ,   pSelectedPoint_ ( 0 )
    ,   pSelectedLimit_  ( 0 )
    ,   bMovingPoint_   ( false )
    ,   bMovingLine_    ( false )
    ,   bDrawAll_       ( true )
    ,   nType_          ( nType )
    ,   bActive_        ( false )
    ,   bShift_         ( false )
    ,   bCreatedPoint_  ( false )
    ,   bCtrl_          ( false )
    ,   pAgent_         ( 0 )
    ,   vOldPos_        ( 0., 0.)
    ,   sFileName_      ( "MOS_Limit.bin" )
{

    setSorting( -1 );
    header()->hide();
    addColumn( "LimitS" );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem * ) ), this, SLOT( SlotItemDoubleClicked( QListViewItem * ) ) );
    connect( this, SIGNAL( itemRenamed( QListViewItem* , int , const QString& ) ), this, SLOT( SlotItemRenamed( QListViewItem* , int , const QString& ) ) );
    setRootIsDecorated( true );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimitListView::~MOS_LimitListView()
{
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::CreateLimitFromSelector
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimitListView::CreateLimitFromSelector( MOS_LimitListView* pSelector )
{
    assert( nType_ == eListViewMission );

    RIT_LimitPtrVector itLimitItem = limitVector_.rbegin();
    while( itLimitItem != limitVector_.rend() )
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)*itLimitItem;
        pLimit->GetLimit().UnregisterListViewItem( this, pLimit );
        DeleteLimit( *pLimit );
        itLimitItem = limitVector_.rbegin();
    }

    MOS_LineManager::T_LimitSet LimitSet;
    pSelector->GetTaggedLimits( LimitSet );

    for ( MOS_LineManager::IT_LimitSet itLimit = LimitSet.begin() ; itLimit != LimitSet.end() ; ++itLimit )
    {
        
        MOS_Limit& Limit = **itLimit;

        MOS_LimitListViewItem* pLimit = 0;
        if ( limitVector_.size() )
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, **( limitVector_.rbegin()), Limit.GetName() );
        else
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, Limit.GetName() );
        pLimit->setRenameEnabled( 0 , false );
        
        RegisterLimit( *pLimit );

        CT_PointPtrVector& pointVector = Limit.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
//            MOS_PointListViewItem& point = pLimit->CreatePoint( **itPoint );
//          point.setVisible( false );
        }
        Limit.RegisterListViewItem( this, pLimit );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::GetTaggedLimits
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimitListView::GetTaggedLimits( MOS_LineManager::T_LimitSet& LimitSet )
{
    assert( nType_ == eListViewSelector );
    for ( IT_LimitPtrVector itLimit = limitVector_.begin() ; itLimit != limitVector_.end() ; ++itLimit )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)*itLimit;
        if ( pLimit->isOn() )
            LimitSet.insert( &pLimit->GetLimit() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::Initialize()
{
    MOS_App::GetApp().GetLineManager().RegisterLimitListView( *this );
    MOS_GLMouse_ABC::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
    MOS_App::GetApp().GetLineManager().UnregisterLimitListView( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::CreateLimit
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem* MOS_LimitListView::CreateLimit( MOS_Limit& Limit )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = 0;
        if ( limitVector_.size() )
            pLimit = new MOS_LimitCheckListItem( eItemTypeEltLimit, Limit, *this, **( limitVector_.rbegin()), QCheckListItem::CheckBox, Limit.GetName() );
        else
            pLimit = new MOS_LimitCheckListItem( eItemTypeEltLimit, Limit, *this, QCheckListItem::CheckBox, Limit.GetName() );
        
        pLimit->setRenameEnabled( 0, false );
        RegisterLimit( *pLimit );
        CT_PointPtrVector& pointVector = Limit.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
            MOS_PointListViewItem& point = pLimit->CreatePoint( **itPoint );
            point.setEnabled( false );

        }

        return pLimit;

    }
    else
    if ( nType_ == eListViewEditor )
    {
        MOS_LimitListViewItem* pLimit = 0;
        if ( limitVector_.size() )
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, **( limitVector_.rbegin()), Limit.GetName() );
        else
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, Limit.GetName() );
        
        RegisterLimit( *pLimit );
        CT_PointPtrVector& pointVector = Limit.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
            pLimit->CreatePoint( **itPoint );
        return pLimit;
    }
    else
    if( nType_ == eListViewMission )
    {
        return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::DeleteLimit
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitListView::DeleteLimit( QListViewItem& Limit )
{
    UnregisterLimit( Limit );
    delete &Limit;
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::RegisterLimit
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitListView::RegisterLimit( QListViewItem& Limit )
{
    limitVector_.push_back( &Limit );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::UnregisterLimit
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitListView::UnregisterLimit( QListViewItem& Limit )
{
    IT_LimitPtrVector itLimit = std::find( limitVector_.begin(), limitVector_.end(), &Limit );
    assert( itLimit != limitVector_.end() );
    limitVector_.erase( itLimit );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_LimitListView::WriteMsg( ASN1T_OID& asnLeftLimitID, ASN1T_OID& asnRightLimitID, std::string& sParam )
{
    assert( nType_ == eListViewMission );
    
    if( limitVector_.size() == 2  )
    {
        sParam += std::string( "2 Limits | " );
        IT_LimitPtrVector itLimit = limitVector_.begin();
        MOS_LimitListViewItem* pLimitItem = 0;
        
//        asnMsgOrder.m.left_limit_idPresent  = 1;
//        asnMsgOrder.m.right_limit_idPresent = 1;

        pLimitItem = (MOS_LimitListViewItem*)(*itLimit);
        pLimitItem->WriteMsg( asnLeftLimitID, sParam );
        ++itLimit;
        pLimitItem = (MOS_LimitListViewItem*)(*itLimit);
        pLimitItem->WriteMsg( asnRightLimitID, sParam );
    }
    else
    {
        sParam += std::string( "No Limits | " );

        asnLeftLimitID  = MIL_NULL_LIMIT_ID;
        asnRightLimitID = MIL_NULL_LIMIT_ID;

//        asnMsgOrder.m.left_limit_idPresent  = 0;
//        asnMsgOrder.m.right_limit_idPresent = 0;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::GetListLimit
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
MOS_LimitListView::T_LimitPtrVector& MOS_LimitListView::GetListLimit()
{
    return limitVector_;
}



//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitListView::FillRandomParameters()
{
    
    assert( nType_ == eListViewMission );

    RIT_LimitPtrVector itLimitItem = limitVector_.rbegin();
    while( itLimitItem != limitVector_.rend() )
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)*itLimitItem;
        pLimit->GetLimit().UnregisterListViewItem( this, pLimit );
        DeleteLimit( *pLimit );
        itLimitItem = limitVector_.rbegin();
    }

    T_LimitPtrVector& limitVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimitEditor().GetListLimit();
    if ( limitVectorEditor.size() < 2 )
        return;

    uint nLimit1 = rand() % ( limitVectorEditor.size() );
    uint nLimit2 = nLimit1;
    while ( nLimit1 = nLimit2 )
    {
       nLimit2 = rand() % ( limitVectorEditor.size() );
    }
    MOS_LineManager::T_LimitSet LimitSet;
    LimitSet.insert( &((MOS_LimitListViewItem*)limitVectorEditor[nLimit1])->GetLimit() );
    LimitSet.insert( &((MOS_LimitListViewItem*)limitVectorEditor[nLimit2])->GetLimit() );
    
    for ( MOS_LineManager::IT_LimitSet itLimit = LimitSet.begin() ; itLimit != LimitSet.end() ; ++itLimit )
    {
        
        MOS_Limit& Limit = **itLimit;

        MOS_LimitListViewItem* pLimit = 0;
        if ( limitVector_.size() )
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, **( limitVector_.rbegin()), Limit.GetName() );
        else
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, Limit.GetName() );
        pLimit->setRenameEnabled( 0 , false );
        
        RegisterLimit( *pLimit );

        Limit.RegisterListViewItem( this, pLimit );
    }
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitListView::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    
    assert( nType_ == eListViewMission );

    T_LimitPtrVector& limitVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimitEditor().GetListLimit();
    if ( limitVectorEditor.size() < 2 )
        return;

    uint nNbLimit = limitVector_.size();
    archive.WriteField( "sName", std::string( "MOS_LimitListView|nNbLimit" ) );
    archive.WriteField( "nNbLimit", nNbLimit );

    for ( IT_LimitPtrVector itLimit = limitVector_.begin() ; itLimit != limitVector_.end() ; ++itLimit )
    {
        MOS_LimitListViewItem* pLimitItem = (MOS_LimitListViewItem*)*itLimit;

        uint nLimit = 0;
        T_LimitPtrVector& limitVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimitEditor().GetListLimit();
        for ( IT_LimitPtrVector itLimitEditor = limitVectorEditor.begin() ; itLimitEditor != limitVectorEditor.end() ; ++itLimitEditor  )
        {
            MOS_LimitListViewItem* pLimitItemDest = (MOS_LimitListViewItem*)*itLimitEditor;

            if ( &pLimitItem->GetLimit() == &pLimitItemDest->GetLimit() )
            {
                archive.WriteField( "sName", std::string( "MOS_LimitListView|nLimit" ) );
                archive.WriteField( "nLimit", nLimit );
                break;
            }
            ++nLimit;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimitListView::ReloadParameters( MT_InputArchive_ABC& archive )
{
    
    assert( nType_ == eListViewMission );

    RIT_LimitPtrVector itLimitItem = limitVector_.rbegin();
    while( itLimitItem != limitVector_.rend() )
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)*itLimitItem;
        pLimit->GetLimit().UnregisterListViewItem( this, pLimit );
        DeleteLimit( *pLimit );
        itLimitItem = limitVector_.rbegin();
    }

    T_LimitPtrVector& limitVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimitEditor().GetListLimit();
    if ( limitVectorEditor.size() < 2 )
        return;


    uint nNbLimit = 0;
    std::string sName( "MOS_LimitListView|nNbLimit" );
    archive.ReadField( "sName", sName );
    archive.ReadField( "nNbLimit", nNbLimit );

    MOS_LineManager::T_LimitSet LimitSet;
    for ( uint j = 0 ; j < nNbLimit ; ++j )
    {
        uint nLimit = 0;
        std::string sName( "MOS_LimitListView|nLimit" );
        archive.ReadField( "sName", sName );
        archive.ReadField( "nLimit", nLimit );
        LimitSet.insert( &((MOS_LimitListViewItem*)limitVectorEditor[nLimit])->GetLimit() );
    }
    
    for ( MOS_LineManager::IT_LimitSet itLimit = LimitSet.begin() ; itLimit != LimitSet.end() ; ++itLimit )
    {
        
        MOS_Limit& Limit = **itLimit;

        MOS_LimitListViewItem* pLimit = 0;
        if ( limitVector_.size() )
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, **( limitVector_.rbegin()), Limit.GetName() );
        else
            pLimit = new MOS_LimitListViewItem( eItemTypeEltLimit, Limit, *this, Limit.GetName() );

        pLimit->setRenameEnabled( 0 , false );
        RegisterLimit( *pLimit );
        Limit.RegisterListViewItem( this, pLimit );
    }
    
}
//=============================================================================
// SLOTS
//=============================================================================
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LimitListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    if ( nType_ == eListViewEditor )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
    
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Add Limit"), 0 );
        }
        else
        {
            switch( ((MOS_ListViewItem_ABC*)pItem)->GetItemType() )
            {
                case eItemTypeEltLimit: 
                {
                    pMenu->insertItem( QString( "Add Limit"), 0 );
                    pMenu->insertItem( QString( "Add point" ), 2 );
                    pMenu->insertItem( QString( "Del Limit %1" ).arg( pSelectedLimit_->text(0) ), 1 );

                }
                break;

                case eItemTypeEltPoint:
                {
                    pMenu->insertItem( QString( "Add point" ), 2 );
                    pMenu->insertItem( QString( "Del point" ), 3 );
                }
                break;
            }
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            pSelectedLimit_ =  CreateLimitFromInterface();
            pSelectedPoint_ = CreatePointFromInterface( *pSelectedLimit_ );
            //APE LocOnPoint( pSelectedPoint_ );

        }
        else
        if ( nSelected == 1 )
        {
            if ( pSelectedLimit_ )
                DeleteLimitFromInterface( *pSelectedLimit_ );
        }
        else
        if ( nSelected == 2 )
        {
            if ( pSelectedLimit_ )
            {
                pSelectedPoint_ = CreatePointFromInterface( *pSelectedLimit_, pSelectedPoint_ );
                //APE LocOnPoint( pSelectedPoint_ );
            }
        }
        else
        if ( nSelected == 3 )
        {
            if ( pSelectedLimit_ && pSelectedPoint_ )
            {
            
                if ( nType_ == eListViewSelector )
                {
                    MOS_LimitCheckListItem* pLimit = static_cast< MOS_LimitCheckListItem* >( pSelectedLimit_ );
                    MOS_PointListViewItem* pNewPoint = pLimit->GetPointBefore( *pSelectedPoint_ );
                    DeletePointFromInterface( *pSelectedLimit_, *pSelectedPoint_ );
                    if ( pNewPoint == 0)
                        pNewPoint = pLimit->GetFirstPoint();

                    pSelectedPoint_ = pNewPoint;
                    //APE LocOnPoint( pSelectedPoint_ );
                }
                else
                {
                    MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( pSelectedLimit_ );
                    MOS_PointListViewItem* pNewPoint = pLimit->GetPointBefore( *pSelectedPoint_ );
                    DeletePointFromInterface( *pSelectedLimit_, *pSelectedPoint_ );
                    if ( pNewPoint == 0)
                        pNewPoint = pLimit->GetFirstPoint();

                    pSelectedPoint_ = pNewPoint;
                    //APE LocOnPoint( pSelectedPoint_ );
                }

            }
        }
    }
    else
    if ( nType_ == eListViewMission )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Add Limit"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            EnterLimitSelector();
        }
    }
    else
    if ( nType_ == eListViewSelector )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Edit Limit"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            EnterLimitEditor();
        }
    }
        
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::EnterLimitEditor
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
void MOS_LimitListView::EnterLimitEditor()
{
    MOS_LimitListView* pListView = &MOS_App::GetApp().GetMainWindow().GetLimitEditor();
    MOS_LimitEditor* pEditor = (MOS_LimitEditor*)pListView->parent();
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::EnterLimitSelector
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
void MOS_LimitListView::EnterLimitSelector()
{
    MOS_LimitListView* pListView = &MOS_App::GetApp().GetMainWindow().GetLimitSelector();
    MOS_LimitSelector* pSelector = (MOS_LimitSelector*)pListView->parent();
    pSelector->SetLimitMission( *this );
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pSelector );
    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::LocOnPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListView::LocOnPoint( MOS_PointListViewItem* pPoint, bool bSelectList )
{
    if ( pPoint )
    {
        if ( bSelectList )
            setSelected( pPoint, true );
        MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pPoint->GetPoint() );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_LimitListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem ) 
        return;

    E_ItemType nType = eItemTypeNone;
    if ( nType_ == eListViewSelector )
    {
        MOS_CheckListItem_ABC* pTmpItem = (MOS_CheckListItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }
    else
    {
        MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }

    // Limit
    if( nType == eItemTypeEltLimit )
    {
        pSelectedLimit_ = pItem;
        if ( nType_ == eListViewSelector )
        {
            MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)pSelectedLimit_;
            pSelectedPoint_ = pLimit->GetFirstPoint();
        }
        else
        {
            MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)pSelectedLimit_;
            pSelectedPoint_ = pLimit->GetFirstPoint();
        }
    }
    // Point
    else
    if( nType == eItemTypeEltPoint )
    {
        pSelectedPoint_ = static_cast< MOS_PointListViewItem* >( pItem );
        pSelectedLimit_ = static_cast< MOS_LimitListViewItem* >( pItem->parent() );
        LocOnPoint( pSelectedPoint_, false );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SlotItemDoubleClicked
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::SlotItemDoubleClicked( QListViewItem* pItem )
{
    if( !pItem || !pSelectedPoint_) 
        return;

    LocOnPoint( pSelectedPoint_, false );
}



//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::Draw()
{
    for ( CIT_LimitPtrVector itLimit = limitVector_.begin() ; itLimit != limitVector_.end(); ++itLimit )
    {
        if ( (*itLimit) == pSelectedLimit_ || bDrawAll_ )
        {
            if ( nType_ == eListViewSelector )
            {
                MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)*itLimit;
                pLimit->Draw( (MOS_LimitCheckListItem*)pSelectedLimit_, pSelectedPoint_ );
            }
            else
            {
                MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)( *itLimit );
                pLimit->Draw( (MOS_LimitListViewItem*)pSelectedLimit_, pSelectedPoint_ );
            }
        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimitListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_LimitListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    if ( nType_ != eListViewEditor )
        return false;

    bool bReturn = false;

    if ( !bMovingPoint_ && !bMovingLine_ )
    {
        for ( CIT_LimitPtrVector itLimit = limitVector_.begin() ; itLimit != limitVector_.end(); ++itLimit )
        {
            if ( !bDrawAll_ )
                if ( pSelectedLimit_ != (*itLimit) )
                    continue;

            if ( bShift_ && !bCreatedPoint_ )
            {
                bCreatedPoint_ = true;

                vOldPos_ = vPos;
                MT_Vector2D vNewPos( vPos );
                pSelectedPoint_ = CreatePointFromInterface( *pSelectedLimit_, pSelectedPoint_ ,&vNewPos );
                //APE LocOnPoint( pSelectedPoint_ );
                bReturn = true;
            }
            else
            if ( bCtrl_ )
            {
                MOS_PointListViewItem* pCurPoint = 0;
                if ( nType_ == eListViewEditor )
                {
//                    MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)( *itLimit );
                    pCurPoint = pSelectedPoint_;;
                }

                if ( pCurPoint )
                {
                    vPosMovingLine_ = vPos;
                    pSelectedPoint_ = pCurPoint;
                    pSelectedLimit_ = static_cast< MOS_LimitListViewItem* >(pCurPoint->parent());
                    setSelected( pSelectedPoint_, true );
                    bMovingLine_ = true;
                    pSelectedPoint_->parent()->setOpen(true);
                    bReturn = true;
                }
            }
            else
            {

                MOS_PointListViewItem* pCurPoint = 0;
                if ( nType_ == eListViewSelector )
                {
                    MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)*itLimit;
                    pCurPoint = pLimit->IsInsidePoint( vPos );
                }
                else
                {
                    MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)( *itLimit );
                    pCurPoint = pLimit->IsInsidePoint( vPos );
                }

                if ( pCurPoint )
                {
                    pSelectedPoint_ = pCurPoint;
                    pSelectedLimit_ = static_cast< MOS_LimitListViewItem* >(pCurPoint->parent());

                    setSelected( pSelectedPoint_, true );
                    bMovingPoint_ = true;
                    pSelectedPoint_->parent()->setOpen(true);
                    bReturn = true;
                }
            
            }


        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( nType_ != eListViewEditor )
        return;


    if ( bMovingPoint_ && pSelectedLimit_ && pSelectedPoint_ )
    {
        SetPosPoint( *pSelectedLimit_, *pSelectedPoint_, vPos );
        vOldPos_ = vPos;
    }
    else
    if ( bMovingLine_ && pSelectedLimit_ && pSelectedPoint_ )
        SetPosLimit( *pSelectedLimit_, vPos, vPosMovingLine_ );

}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_LimitListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( nType_ != eListViewEditor )
        return;

    bCreatedPoint_ = false;

    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SetPosPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListView::SetPosPoint( QListViewItem& Limit, MOS_PointListViewItem& point, const MT_Vector2D& vPos )
{
    point.SetPos( vPos );

    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );

    }
    else
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SetPosLimit
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListView::SetPosLimit( QListViewItem& Limit, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        pLimit->SetPos( vPos, vPosMovingLine );
    }
    else
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        pLimit->SetPos( vPos, vPosMovingLine );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::CreateLimitFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
QListViewItem* MOS_LimitListView::CreateLimitFromInterface()
{
    MOS_Limit* pLimit = new MOS_Limit( true );
    pLimit->SetState( eStateLimitCreated );
    pLimit->SetNetworkState( eStateLimitNotRegistered );
    return pLimit->GetItem( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::DeleteLimitFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListView::DeleteLimitFromInterface( QListViewItem& Limit )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitDeleted );
    }
    else
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitDeleted );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::CreatePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitListView::CreatePointFromInterface( QListViewItem& Limit, MOS_PointListViewItem* pPoint, MT_Vector2D* pPos )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        if ( pPos )
        {
            const MT_Vector2D& vPoint = pLimit->GetLimit().CreatePoint( *pPos, pPoint );
            return pLimit->GetPoint( vPoint );
        }
        else
        {
            const MT_Vector2D& vPoint = pLimit->GetLimit().CreatePoint( pLimit->GetCenterPoint( pPoint ), pPoint );
            return pLimit->GetPoint( vPoint );
        }

    }
    else
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        if ( pPos )
        {
            const MT_Vector2D& vPoint = pLimit->GetLimit().CreatePoint( *pPos, pPoint );
            return pLimit->GetPoint( vPoint );
        }
        else
        {
            const MT_Vector2D& vPoint = pLimit->GetLimit().CreatePoint( pLimit->GetCenterPoint( pPoint ), pPoint );
            return pLimit->GetPoint( vPoint );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::DeletePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListView::DeletePointFromInterface( QListViewItem& Limit, MOS_PointListViewItem& point )
{
    MT_Vector2D& vPoint = point.GetPoint();
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitCheckListItem* pLimit = (MOS_LimitCheckListItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        pLimit->GetLimit().DeletePoint( vPoint );
    }
    else
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)&Limit;
        pLimit->GetLimit().SetState( eStateLimitCoordModified );
        pLimit->GetLimit().DeletePoint( vPoint );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SlotSetShowAll
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitListView::SlotSetShowAll( bool bOn )
{
    bDrawAll_ = bOn;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SlotItemRenamed
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_LimitListView::SlotItemRenamed( QListViewItem* pItem, int /*nCol*/, const QString& sText )
{
    MOS_LimitListViewItem* pLimit = static_cast< MOS_LimitListViewItem* >( pItem );
    pLimit->GetLimit().SetState( eStateLimitCoordModified );
    pLimit->GetLimit().SetName( std::string( sText ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::mousePressEvent
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_LimitListView::mousePressEvent( QMouseEvent* /*pMouseEvent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimitListView::IsActive( bool bMove )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimitSelector* pParent = (MOS_LimitSelector*)parent();
        return pParent->IsActive( bMove );
    }
    else
    if ( nType_ == eListViewEditor )
    {
        MOS_LimitEditor* pParent = (MOS_LimitEditor*)parent();
        return pParent->IsActive( bMove );
    }
    else
    if ( nType_ == eListViewMission )
    {
        MOS_LimitMission* pParent = (MOS_LimitMission*)parent();
        return pParent->IsActive( bMove );
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_LimitListView::keyPressEvent( QKeyEvent * e )
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
// Name: MOS_LimitListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_LimitListView::keyReleaseEvent( QKeyEvent * e )
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

//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimitListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListView::GetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
MOS_Agent* MOS_LimitListView::GetAgent()
{
    return pAgent_;    
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_LimitListView::WriteForGEN
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_LimitListView::Write() const
{
    if ( nType_ != eListViewEditor )
        return 0;
    
    MT_FlatBinaryOutputArchive archive;
    uint nLimits = limitVector_.size();
    archive.WriteField( "Limits", nLimits );
    
    for( CIT_LimitPtrVector itLimit = limitVector_.begin(); itLimit != limitVector_.end(); ++itLimit )    
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)(*itLimit);
        if( pLimit->GetLimit().IsLocal() )
            pLimit->GetLimit().Write( archive );
    }     
    archive.WriteToFile( sFileName_ );
    return 0;
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_LimitListView::WriteForGEN
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_LimitListView::Read()
{
    if( !MOS_App::GetApp().GetLineManager().GetLimitList().empty() )
        return 0;

    if ( nType_ != eListViewEditor )
        return 0;
    
    MT_FlatBinaryInputArchive archive;
    archive.Open( sFileName_ );

    uint nLimits = 0;
    archive.ReadField( "Limits", nLimits );
    
    for ( uint i = 0 ; i < nLimits ; ++i )
    {
        MOS_LimitListViewItem* pLimit = (MOS_LimitListViewItem*)CreateLimitFromInterface();
        pLimit->GetLimit().Read( archive );
        pLimit->GetLimit().SetState( eStateLimitCreated );
        pLimit->GetLimit().SetNetworkState( eStateLimitNotRegistered );
    }
    
    MOS_App::GetApp().GetLineManager().UpdateLimitToSim();

    return 0;
}


