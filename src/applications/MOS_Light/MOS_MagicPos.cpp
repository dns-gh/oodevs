//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MagicPos.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 16 $
// $Workfile: MOS_MagicPos.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MagicPos.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MagicPos.inl"
#endif

#include "MOS_Attr_Def.h"
#include "MOS_Net_Def.h"

#include "MOS_PointListViewItem.h"
#include "MOS_Agent.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_AttrEditor.h"

#include "MOS_ASN_Messages.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MagicPos::MOS_MagicPos( QWidget* pParent, const std::string& sName )
    :   MOS_PointListView   ( pParent, sName, eListViewEditor )
    ,   bModifiedAgent_ ( false )
    ,   vOldPos_        ( 0., 0.)
    ,   pAgent_         ( 0 )
    ,   sName_          ( sName )
    ,   pLabel_         ( 0 )
{
    setMaximumHeight( 60 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MagicPos::~MOS_MagicPos()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::Show
// Created: AGN 03-07-23
//-----------------------------------------------------------------------------
void MOS_MagicPos::Show( bool bShow )
{
    if( bShow )
    {
        show();
        if( pLabel_ != 0 )
            pLabel_->show();
    }
    else
    {
        hide();
        if( pLabel_ != 0 )
            pLabel_->hide();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::WriteOrder
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MagicPos::WriteMsg( std::string& sParam )
{
    assert( pAgent_ );

    IT_PointPtrVector itPoint = pointVector_.begin();
    if( bModifiedAgent_ )
    {
        if ( pAgent_ )
            pAgent_->NotifyMagicalMove();

        ++itPoint;
    }
    
    ASN1T_CoordUTM  coordUTM;
    (*itPoint)->WriteMsg( coordUTM, sParam );

    MOS_ASN_MsgUnitMagicAction asnMsg;
    asnMsg.GetAsnMsg().oid              = pAgent_->GetAgentID();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_move_to;
    asnMsg.GetAsnMsg().action.u.move_to = &coordUTM;
    asnMsg.Send( 56 );

    SetAgent( pAgent_ );
}



//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MagicPos::FillRandomParameters()
{
    assert( pAgent_ );
    pointVector_.clear();
    // $$$$ AGE 2005-02-07: 
    uint nX = 0; 
    uint nY = 0; 
    MT_Vector2D vPos2( nX, nY );
    CreatePoint( vPos2 );
    
}



//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::ResaveParameters
// Created: FBD 03-09-09
//-----------------------------------------------------------------------------
void MOS_MagicPos::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    assert( pointVector_.size() == 1 );

    pointVector_[0]->GetPos().Write( archive );
   
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MagicPos::ReloadParameters( MT_InputArchive_ABC& archive )
{
    assert( pAgent_ );
    pointVector_.clear();
    MT_Vector2D vPos2;
    vPos2.Read( archive );
    CreatePoint( vPos2 );
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MagicPos::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
    if ( pAgent_ )
    {
        DeleteAllPoints();
        vOldPos_ = MT_Vector2D( 0, 0 );
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        point.setText( 0, pAgent_->GetName().c_str() );
        pSelectedPoint_ = &point;
        setSelected( 0, &point );
        setFocus();
        bModifiedAgent_ = false;

        Show( true );
    }
    else
        Show( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_MagicPos::SlotContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& point )
{
    if ( !pAgent_  )
        return;

    if ( !bModifiedAgent_)
    {
        QPopupMenu* pMenu = new QPopupMenu( this );
        pMenu->insertItem( QString( "Magic move Agent"), 0 );
        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == 0 )
        {
            bModifiedAgent_ = true;
            CreatePointFromInterface( **pointVector_.rbegin() );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MagicPos::Draw()
{
    if ( pointVector_.size() == 0 || !pAgent_ )
        return;
    
    MT_Float rSizearrow = 100.;
    
    GFX_Color color( 255,255,0 );
    IT_PointPtrVector itPoint = pointVector_.begin();
    (*itPoint)->SetPos( pAgent_->GetPos() );
    (*itPoint)->setText( 0, pAgent_->GetName().c_str() );
    if ( bModifiedAgent_)
    {
        ++itPoint;
        MOS_PointListViewItem* pLastPoint = 0;
        for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
        {
            MOS_PointListViewItem* pCurPoint = *itPoint;
            if ( pLastPoint )
                GFX_Tools::CreateGLArrow( pLastPoint->GetPoint(), pCurPoint->GetPoint(), color, 4, rSizearrow );

            pLastPoint = pCurPoint;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MagicPos::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_MagicPos::RMBPressEvent( const MT_Vector2D& vPos )
{
    bool bReturn = false;

    if ( !bMovingPoint_ )
    {
        MOS_PointListViewItem* pCurPoint = 0;
        pCurPoint = IsInsidePoint( vPos );
        if ( pCurPoint )
        {
            pSelectedPoint_ = pCurPoint;
            setSelected( pSelectedPoint_, true );
            bMovingPoint_ = true;
            bReturn = true;
        }
        else
        if ( bModifiedAgent_ )
        {
            pCurPoint = *pointVector_.rbegin();
            pSelectedPoint_ = pCurPoint;
            pSelectedPoint_->SetPos( vPos );
            vOldPos_ = vPos;
            LocOnPoint( pSelectedPoint_ );
            bReturn = true;
        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MagicPos::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MagicPos::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MagicPos::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MagicPos::IsActive( bool bMove )
{
    bool bActive = false;
    if ( pAttrEditor_  )
    {
       bActive = pAttrEditor_->IsActive( bMove );
       if ( bActive && bMove )   
           bActive = pAttrEditor_->IsActiveTab( pWidgetTab_ );
           if ( bActive )
               bActive = hasFocus();
    }
    return bActive;
}

//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MagicPos::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
{
    if ( pAgent_ == 0)
        return;

    if ( pAgent_ != &agent )
        return;

    if ( pointVector_.size() == 0 )
    {
        vOldPos_ = MT_Vector2D( 0, 0 );
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        pSelectedPoint_ = &point;
        setSelected( 0, &point );
        bModifiedAgent_ = false;
    }
    IT_PointPtrVector itPoint = pointVector_.begin();
    (*itPoint)->SetPos( pAgent_->GetPos() );
    (*itPoint)->setText( 0, pAgent_->GetName().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_MagicPos::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_MagicPos::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_MagicPos::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_MagicPos::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    if ( pointVector_.size() < 2 )
        return 0; 

    CIT_PointPtrVector itPoint = pointVector_.begin();
    ++itPoint;
    
    for ( ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = (MOS_PointListViewItem*)*itPoint;
        if ( pCurPoint->IsInside( vPos ) )
            return (MOS_PointListViewItem*)*itPoint;
    }
    return 0;
}


