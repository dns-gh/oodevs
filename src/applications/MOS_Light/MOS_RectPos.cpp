//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RectPos.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_RectPos.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_RectPos.h"

#ifndef MOS_USE_INLINE
#	include "MOS_RectPos.inl"
#endif

#include "MOS_Attr_Def.h"
#include "MOS_Net_Def.h"

#include "MOS_PointListViewItem.h"
#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Mission_ABC.h"


using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_RectPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RectPos::MOS_RectPos( QWidget* pParent, const std::string& sName )
    :   MOS_PointListView   ( pParent, sName, eListViewEditor )
    ,   vOldPos_            ( 0., 0.)
    ,   pAgent_             ( 0 )
    ,   bMovingLine_        ( false )
    ,   bFirstWidget_       ( false )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RectPos::~MOS_RectPos()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RectPos::Initialize()
{
    MOS_PointListView::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RectPos::Terminate()
{
    MOS_PointListView::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::WriteOrder
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
/*void MOS_RectPos::WriteOrderMsg( DIN_BufferedMessage& msg )
{
    MT_LOG_INFO_MSG( MT_FormatString( "-------RectPos %s------", sName_.c_str() ).c_str() );

    uint8 nPoint = (uint8)pointVector_.size();
    msg << nPoint;
    MT_LOG_INFO_MSG( MT_FormatString( "nPos:%d(uint8)", nPoint ).c_str() );

    for ( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pPointItem = (MOS_PointListViewItem*)(*itPoint);
        pPointItem->WriteOrderMsg( msg );
    }
}*/

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_RectPos::WriteMsg()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_RectPos::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    if ( pAgent_ )
    {
        DeleteAllPoints();
     
        MT_Vector2D vPos1 = GetCenterPoint( 0 );
        MT_Vector2D vPos3 = GetCenterPoint( 0 );
        MT_Vector2D vPos2 = MT_Vector2D( vPos3.rX_, vPos1.rY_ );
        MT_Vector2D vPos4 = MT_Vector2D( vPos1.rX_, vPos3.rY_ );
        CreatePoint( vPos1 );
        CreatePoint( vPos2 );
        CreatePoint( vPos3 );
        MOS_PointListViewItem* pPoint = &CreatePoint( vPos4 );
        setSelected( 0, pPoint );
        if ( bFirstWidget_ )
            setFocus();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_RectPos::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{

    if ( !pAgent_  )
        return;

    QPopupMenu* pMenu = new QPopupMenu( this );
    if( !pItem ) 
        pMenu->insertItem( QString( "Add Point"), 0 );
    else
    {
        switch( ((MOS_ListViewItem_ABC*)pItem)->GetItemType() )
        {
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

    if ( nSelected == 2 )
    {
        if ( pSelectedPoint_ )
        {
            pSelectedPoint_ = CreatePointFromInterface( *pSelectedPoint_ );
            LocOnPoint( pSelectedPoint_ );
        }
    }
    else
    if ( nSelected == 3 )
    {
        if ( pSelectedPoint_  )
        {
    
            MOS_PointListViewItem* pPoint = (MOS_PointListViewItem*)( pSelectedPoint_ );
            MOS_PointListViewItem* pNewPoint = GetPointBefore( *pSelectedPoint_ );
            DeletePointFromInterface( *pPoint );
            if ( pNewPoint == 0)
                pNewPoint = GetFirstPoint();

            pSelectedPoint_ = pNewPoint;
            LocOnPoint( pSelectedPoint_ );
        }
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RectPos::Draw()
{
    GFX_Color color( 255,255,0 );

    if ( pointVector_.size() != 4 || !pAgent_ )
        return;

    for ( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end(); ++itPoint )
    {
        MOS_PointListViewItem* pPoint = (MOS_PointListViewItem*)( *itPoint );
        pPoint->Draw( (MOS_PointListViewItem*)pSelectedPoint_ );
    }

    GFX_Tools::CreateGLLine( pointVector_[0]->GetPos(), pointVector_[1]->GetPos(), 2., color );
    GFX_Tools::CreateGLLine( pointVector_[1]->GetPos(), pointVector_[2]->GetPos(), 2., color );
    GFX_Tools::CreateGLLine( pointVector_[2]->GetPos(), pointVector_[3]->GetPos(), 2., color );
    GFX_Tools::CreateGLLine( pointVector_[3]->GetPos(), pointVector_[0]->GetPos(), 2., color );
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_RectPos::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_RectPos::RMBPressEvent( const MT_Vector2D& vPos )
{
    assert( pointVector_.size() == 4 );

    bool bReturn = false;

    if ( !bMovingPoint_ && !bMovingLine_ )
    {
        MOS_PointListViewItem* pCurPoint = 0;
        pCurPoint = IsInsidePoint( vPos );
        if ( pCurPoint )
        {
            bMovingPoint_ = true;
            pSelectedPoint_ = pCurPoint;
            setSelected( pSelectedPoint_, true );
            UpdatePosRect( vPos );
            vOldPos_ = vPos;
            bReturn = true;

        }
        else
        if ( bCtrl_ && pointVector_.size() )
        {
            vPosMovingLine_ = vPos;
            pCurPoint = *pointVector_.begin();
            bMovingLine_ = true;
            bReturn = true;
        }
    }
    return bReturn;

}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::UpdatePosRect
// Created: FBD 03-02-06
//-----------------------------------------------------------------------------
void MOS_RectPos::UpdatePosRect( const MT_Vector2D& vPos )
{
    if ( pSelectedPoint_ == pointVector_[0] )
    {
        pSelectedPoint_->SetPos( vPos );
       const MT_Vector2D& vNewPos1 = pointVector_[1]->GetPos();
       pointVector_[1]->SetPos( MT_Vector2D( vNewPos1.rX_, vPos.rY_ ) );

       const MT_Vector2D& vNewPos2 = pointVector_[3]->GetPos();
       pointVector_[3]->SetPos( MT_Vector2D( vPos.rX_, vNewPos2.rY_ ) );
    }
    else
    if ( pSelectedPoint_ == pointVector_[1] )
    {
        pSelectedPoint_->SetPos( vPos );
       const MT_Vector2D& vNewPos1 = pointVector_[0]->GetPos();
       pointVector_[0]->SetPos( MT_Vector2D( vNewPos1.rX_, vPos.rY_ ) );

       const MT_Vector2D& vNewPos2 = pointVector_[2]->GetPos();
       pointVector_[2]->SetPos( MT_Vector2D( vPos.rX_, vNewPos2.rY_ ) );
    }
    else
    if ( pSelectedPoint_ == pointVector_[2] )
    {
        pSelectedPoint_->SetPos( vPos );
       const MT_Vector2D& vNewPos1 = pointVector_[1]->GetPos();
       pointVector_[1]->SetPos( MT_Vector2D( vPos.rX_, vNewPos1.rY_ ) );

       const MT_Vector2D& vNewPos2 = pointVector_[3]->GetPos();
       pointVector_[3]->SetPos( MT_Vector2D( vNewPos2.rX_, vPos.rY_ ) );
    }
    else
    if ( pSelectedPoint_ == pointVector_[3] )
    {
        pSelectedPoint_->SetPos( vPos );
       const MT_Vector2D& vNewPos1 = pointVector_[0]->GetPos();
       pointVector_[0]->SetPos( MT_Vector2D( vPos.rX_, vNewPos1.rY_ ) );

       const MT_Vector2D& vNewPos2 = pointVector_[2]->GetPos();
       pointVector_[2]->SetPos( MT_Vector2D( vNewPos2.rX_, vPos.rY_ ) );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RectPos::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        UpdatePosRect( vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( *pSelectedPoint_, vPos, vPosMovingLine_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_RectPos::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RectPos::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_RectPos::IsActive( bool bMove )
{
    bool bActive = false;
    if ( pMission_ )
    {
       bActive = pMission_->IsActive( bMove );
       if ( bActive && bMove )   
           bActive = pMission_->IsActiveTab( pWidgetTab_ );
           if ( bActive )
               bActive = hasFocus();
    }
    return bActive;
}
//-----------------------------------------------------------------------------
// Name: MOS_RectPos::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_RectPos::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
{
    if ( pAgent_ == 0)
        return;

    if ( pAgent_ != &agent )
        return;

    if ( pointVector_.size() == 0 )
    {
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        point.setText( 0, pAgent_->GetName().c_str() );
        pSelectedPoint_ = &point;
        MOS_PointListViewItem* pPoint = CreatePointFromInterface( point );
        setSelected( 0, pPoint );
//        setFocus();
    }
    IT_PointPtrVector itPoint = pointVector_.begin();
    (*itPoint)->SetPos( pAgent_->GetPos() );
    (*itPoint)->setText( 0, pAgent_->GetName().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_RectPos::GetCenterPoint( MOS_PointListViewItem* pPoint )
{
    if ( !pAgent_ )
        return MT_Vector2D( 0., 0. );

    MT_Vector2D vNewPos;

    uint nPoint = pointVector_.size();

    if ( pPoint == 0 && nPoint )
        pPoint = *pointVector_.rbegin();

    if( pointVector_.size() )
    {
        CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), pPoint );
        assert( itPoint != pointVector_.end() );

        if( nPoint > 1 )
        {
            // Si premier point
            if ( *itPoint == *pointVector_.begin() )
            {
                MOS_PointListViewItem* pPointAfter = GetPointAfter( **itPoint );
                assert( pPointAfter );
                vNewPos = ( ( pPointAfter->GetPos() - (*itPoint)->GetPos() ) / 2.0 + (*itPoint)->GetPos() );

            }
            else
            {
                MOS_PointListViewItem* pPointBefore = GetPointBefore( **itPoint );
                assert( pPointBefore );
                vNewPos = ( pPoint->GetPos() - pPointBefore->GetPos()  + pPoint->GetPos() );
            }
        }
        else
        {
            if ( vOldPos_.rX_ == 0.0 && vOldPos_.rY_ == 0.0 )
            {
                MT_Vector2D vPos = (*itPoint)->GetPos();
                vNewPos = MT_Vector2D( vPos.rX_ + ( (MT_Float)( rand() % 50 ) ), vPos.rY_ + ( (MT_Float)( rand() % 50 ) )  );
            }
            else
            {
                vNewPos = vOldPos_;
            }
        }
    }
    else
        vNewPos = pAgent_->GetPosAround();

    //. Clipping dans le monde
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if ( !rectWorld.IsInside( vNewPos ) )
    {
        if ( pPoint )
            vNewPos = pPoint->GetPoint();
                        
    }
    vOldPos_ = vNewPos;

    return vNewPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_RectPos::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_RectPos::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_RectPos::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_RectPos::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}
//-----------------------------------------------------------------------------
// Name: MOS_RectPos::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_RectPos::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = (MOS_PointListViewItem*)*itPoint;
        if ( pCurPoint->IsInside( vPos ) )
            return (MOS_PointListViewItem*)*itPoint;
    }
    return 0;
}
//-----------------------------------------------------------------------------
// Name: MOS_RectPos::SetPosLine
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_RectPos::SetPosLine( MOS_PointListViewItem& /*point*/, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
}


