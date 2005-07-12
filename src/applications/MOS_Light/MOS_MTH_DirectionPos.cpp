//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_DirectionPos.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:53 $
// $Revision: 7 $
// $Workfile: MOS_MTH_DirectionPos.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_MTH_DirectionPos.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_DirectionPos.inl"
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
// Name: MOS_MTH_DirectionPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_DirectionPos::MOS_MTH_DirectionPos( ASN1T_Direction& asnDirection, QWidget* pParent, const std::string& sName, bool bFollow )
    :   MOS_PointListView( pParent, sName, eListViewEditor )
    ,   asnDirection_	 ( asnDirection )
	,	vOldPos_         ( 0., 0.)
    ,   pAgent_          ( 0 )
    ,   bFollow_         ( bFollow )
    ,   bMovingLine_     ( false )
    ,   bFirstWidget_    ( false )
    ,   sName_           ( sName )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_DirectionPos::~MOS_MTH_DirectionPos()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::Initialize()
{
    MOS_PointListView::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::Terminate()
{
    MOS_PointListView::Terminate();
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::FillRandomParameters()
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
// Name: MOS_MTH_DirectionPos::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::ResaveParameters( MT_OutputArchive_ABC& archive )
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
// Name: MOS_MTH_DirectionPos::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::ReloadParameters( MT_InputArchive_ABC& archive )
{
    DeleteAllPoints();

    if ( pAgent_ )
    {
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        MOS_PointListViewItem* pPoint = CreatePointFromInterface( point );
        vPos.Read( archive );
        MT_Vector2D vPos2;
        vPos2.Read( archive );
        pPoint->SetPos( vPos2 );

    }
	assert( pointVector_.size() == 2 );
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::WriteMsg( std::string& sParam )
{
	assert( pointVector_.size() == 2 );

    // Compute the angle in degrees
    MT_Vector2D vNorth( 0, 1 );
    MT_Vector2D vDir = pointVector_[1]->GetPoint() - pointVector_[0]->GetPoint();
    vDir.Normalize();

    MT_Float rDotProduct = DotProduct( vNorth, vDir );
    MT_Float rAngle = rDotProduct / ( vNorth.Magnitude() * vDir.Magnitude() );
    rAngle = acos( rAngle ) * 180 / MT_PI;
    if( vDir.rX_ < 0 )
        rAngle = 360 - rAngle;

    asnDirection_ = rAngle;

    sParam += sName_;
    sParam += MT_FormatString( " Dir : %f Deg |", rAngle );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    if ( pAgent_ )
    {
        DeleteAllPoints();
        
        if ( bFollow_ )
        {
            MT_Vector2D vPos = pAgent_->GetPos();
            MOS_PointListViewItem& point = CreatePoint( vPos );
            point.setText( 0, pAgent_->GetName().c_str() );
            pSelectedPoint_ = &point;
            MOS_PointListViewItem* pPoint = CreatePointFromInterface( point );
            setSelected( 0, pPoint );
            if ( bFirstWidget_ )
                setFocus();
        }
        else
        {
            MT_Vector2D vPos = GetCenterPoint( 0 );
            MOS_PointListViewItem& point1 = CreatePoint( vPos );
            vOldPos_ = MT_Vector2D( 0., 0. );
            vPos = GetCenterPoint( &point1 );
            MOS_PointListViewItem& point2 = CreatePoint( vPos );
            pSelectedPoint_ = &point2;
            setSelected( 0, pSelectedPoint_ );
            if ( bFirstWidget_ )
                setFocus();
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::SlotContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& /*point*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::Draw()
{
    assert( pointVector_.size() == 2 );

    if ( !pAgent_ )
        return;
    
    GFX_Color color( 255,255,0 );

    if ( bFollow_ )    
    {
        pointVector_[0]->SetPos( pAgent_->GetPos() );
        pointVector_[0]->setText( 0, pAgent_->GetName().c_str() );
    }
    else
        pointVector_[0]->Draw();

    pointVector_[1]->Draw();

    
    MT_Float rSizearrow = 100.;
    GFX_Tools::CreateGLArrow( pointVector_[0]->GetPos(), pointVector_[1]->GetPos(), color, 4, rSizearrow );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_DirectionPos::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_MTH_DirectionPos::RMBPressEvent( const MT_Vector2D& vPos )
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
        if ( !bFollow_ && bCtrl_ && pointVector_.size() )
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
// Name: MOS_MTH_DirectionPos::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( *pSelectedPoint_, vPos, vPosMovingLine_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_DirectionPos::IsActive( bool bMove )
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
// Name: MOS_MTH_DirectionPos::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
{
    if ( pAgent_ == 0 || !bFollow_ || pAgent_ != &agent )
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
// Name: MOS_MTH_DirectionPos::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_MTH_DirectionPos::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_MTH_DirectionPos::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_MTH_DirectionPos::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    if ( pointVector_.size() < 2 )
        return 0; 

    
    if ( !bFollow_ )
    {
        if ( pointVector_[0]->IsInside( vPos ) )
            return pointVector_[0];
    }

    if ( pointVector_[1]->IsInside( vPos ) )
            return pointVector_[1];

    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_DirectionPos::SetPosLine
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_DirectionPos::SetPosLine( MOS_PointListViewItem& /*point*/, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
}


