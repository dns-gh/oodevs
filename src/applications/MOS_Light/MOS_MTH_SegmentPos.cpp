//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_SegmentPos.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 7 $
// $Workfile: MOS_MTH_SegmentPos.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_MTH_SegmentPos.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_SegmentPos.inl"
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
// Name: MOS_MTH_SegmentPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_SegmentPos::MOS_MTH_SegmentPos( ASN1T_Segment& asnSegment, QWidget* pParent, const std::string& sName )
    : MOS_PointListView   ( pParent, sName, eListViewEditor )
    , asnSegment_         ( asnSegment )
    , bModifiedAgent_     ( false )
    , vOldPos_            ( 0., 0.)
    , pAgent_             ( 0 )
    , bMovingLine_        ( false )
    , bFirstWidget_       ( false )
    , sName_              ( sName )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_SegmentPos::~MOS_MTH_SegmentPos()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::Initialize()
{
    MOS_PointListView::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::Terminate()
{
    MOS_PointListView::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::WriteMsg
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::WriteMsg( std::string& sParam )
{

    assert( pointVector_.size() == 2 );

    asnSegment_.type                  = EnumTypeLocalisation::line;
    asnSegment_.vecteur_point.n       = 2;
    asnSegment_.vecteur_point.elem    = asnPos_;
    sParam += sName_;
    sParam += std::string( "2 Coords : | " );

    IT_PointPtrVector itPoint = pointVector_.begin();
    ((MOS_PointListViewItem*)*itPoint)->WriteMsg( asnSegment_.vecteur_point.elem[0], sParam );
    ++itPoint;
    ((MOS_PointListViewItem*)*itPoint)->WriteMsg( asnSegment_.vecteur_point.elem[1], sParam );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::FillRandomParameters()
{
    DeleteAllPoints();
    if ( pAgent_ )
    {
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        CreatePointFromInterface( point );
    }
	assert( pointVector_.size() == 2 );
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::ReloadParameters( MT_InputArchive_ABC& archive )
{
    DeleteAllPoints();
    if ( pAgent_ )
    {
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        MOS_PointListViewItem* pItem = CreatePointFromInterface( point );
        vPos.Read( archive );
        MT_Vector2D vPos2;
        vPos2.Read( archive );
        pItem->SetPos( vPos2 );
    }
	assert( pointVector_.size() == 2 );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::ResaveParameters( MT_OutputArchive_ABC& archive )
{
	assert( pointVector_.size() == 2 );

    if ( pAgent_ )
    {
        const MT_Vector2D& vPos1 = pointVector_[0]->GetPos();
        const MT_Vector2D& vPos2 = pointVector_[1]->GetPos();
        vPos1.Write( archive );
        vPos2.Write( archive );
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    if ( pAgent_ )
    {
        DeleteAllPoints();

        MT_Vector2D vPos = GetCenterPoint( 0 );
        MOS_PointListViewItem& point = CreatePoint( vPos );
        vOldPos_ = MT_Vector2D( 0., 0.);
        vPos = GetCenterPoint( &point );
        /*MOS_PointListViewItem& point2 = */CreatePoint( vPos );
        setSelected( 0, &point );
        if ( bFirstWidget_ )
            setFocus();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::SlotContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& /*point*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::Draw()
{
    GFX_Color color( 255,255,0 );

    if ( pointVector_.size() == 0 || !pAgent_ )
        return;

    for ( IT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end(); ++itPoint )
    {
        MOS_PointListViewItem* pPoint = (MOS_PointListViewItem*)( *itPoint );
        pPoint->Draw( (MOS_PointListViewItem*)pSelectedPoint_ );
    }

    MOS_PointListViewItem* pLastPoint = 0;
    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
            GFX_Tools::CreateGLLine( pLastPoint->GetPoint(), pCurPoint->GetPoint(), 2., color );
        pLastPoint = pCurPoint;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_SegmentPos::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_MTH_SegmentPos::RMBPressEvent( const MT_Vector2D& vPos )
{
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
// Name: MOS_MTH_SegmentPos::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( *pSelectedPoint_, vPos, vPosMovingLine_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_SegmentPos::IsActive( bool bMove )
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
// Name: MOS_MTH_SegmentPos::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
{
    if ( pAgent_ == 0)
        return;

    if ( pAgent_ != &agent )
        return;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_MTH_SegmentPos::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_MTH_SegmentPos::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_SegmentPos::IsInsidePoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_MTH_SegmentPos::IsInsidePoint( const MT_Vector2D& vPos ) const
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
// Name: MOS_MTH_SegmentPos::SetPosLine
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_SegmentPos::SetPosLine( MOS_PointListViewItem& /*point*/, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
}


