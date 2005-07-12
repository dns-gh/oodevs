//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Itineraire1Pos.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:53 $
// $Revision: 8 $
// $Workfile: MOS_MTH_Itineraire1Pos.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_MTH_Itineraire1Pos.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_Itineraire1Pos.inl"
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
// Name: MOS_MTH_Itineraire1Pos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_Itineraire1Pos::MOS_MTH_Itineraire1Pos( ASN1T_Point& asnPoint, QWidget* pParent, const std::string& sName )
    :   MOS_PointListView   ( pParent, sName, eListViewEditor )
    ,   asnPoint_           ( asnPoint )
    ,   vOldPos_            ( 0., 0.)
    ,   pAgent_             ( 0 )
    ,   bFirstWidget_       ( false )
    ,   sName_              ( sName )

{
  
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_Itineraire1Pos::~MOS_MTH_Itineraire1Pos()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::Initialize()
{
    MOS_PointListView::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::Terminate()
{
    MOS_PointListView::Terminate();
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::WriteMsg( std::string& sParam )
{
    assert( pointVector_.size() == 2 );
    sParam += sName_;
    sParam += " : ";

    asnPoint_.type                  = EnumTypeLocalisation::point;
    asnPoint_.vecteur_point.n       = 1;
    asnPoint_.vecteur_point.elem    = &asnPos_;
    MOS_PointListViewItem* pPoint = *pointVector_.rbegin();
    pPoint->WriteMsg( asnPoint_.vecteur_point.elem[0], sParam );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::FillRandomParameters()
{
    DeleteAllPoints();
    assert( pAgent_ );

    MT_Vector2D vPos = pAgent_->GetPos();
    MOS_PointListViewItem& point = CreatePoint( vPos );
    point.setText( 0, pAgent_->GetName().c_str() );
    // $$$$ AGE 2005-02-07: 
    uint nX = 0; 
    uint nY = 0; 
    MT_Vector2D vPos2( nX, nY );
    CreatePoint( vPos2, &point );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::ResaveParameters( MT_OutputArchive_ABC& archive )
{
	assert( pointVector_.size() == 2 );

    const MT_Vector2D& vPos = pointVector_[1]->GetPos();
    vPos.Write( archive );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::ReloadParameters( MT_InputArchive_ABC& archive )
{
    DeleteAllPoints();
    MT_Vector2D vPos1 = pAgent_->GetPos();
    MOS_PointListViewItem& point = CreatePoint( vPos1 );
    point.setText( 0, pAgent_->GetName().c_str() );

    MT_Vector2D vPos2;
    vPos2.Read( archive );
    CreatePoint( vPos2, &point );
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;

    if ( pAgent_ )
    {
        DeleteAllPoints();
        MT_Vector2D vPos = pAgent_->GetPos();
        MOS_PointListViewItem& point = CreatePoint( vPos );
        point.setText( 0, pAgent_->GetName().c_str() );
        pSelectedPoint_ = CreatePointFromInterface( point );
        setSelected( 0, pSelectedPoint_ );
        if ( bFirstWidget_ )
            setFocus();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::SlotContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& /*point*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::Draw()
{
    GFX_Color color( 255,255,0 );

    if ( pointVector_.size() == 0 || !pAgent_ )
        return;
    
    IT_PointPtrVector itPoint = pointVector_.begin();
    (*itPoint)->SetPos( pAgent_->GetPos() );
    (*itPoint)->setText( 0, pAgent_->GetName().c_str() );

    ++itPoint;
    if ( pointVector_.size() >= 2 )
    {
        for (  ; itPoint != pointVector_.end(); ++itPoint )
        {
            MOS_PointListViewItem* pPoint = (MOS_PointListViewItem*)( *itPoint );
            if ( pSelectedPoint_ == pPoint )
                pPoint->Draw( (MOS_PointListViewItem*)pSelectedPoint_ );
        }
    }

    MT_Float rSizearrow = 100.;
    MOS_PointListViewItem* pLastPoint = 0;
    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            GFX_Tools::CreateGLArrow( pLastPoint->GetPoint(), pCurPoint->GetPoint(), color, 4., rSizearrow );
        }
        pLastPoint = pCurPoint;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_Itineraire1Pos::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_MTH_Itineraire1Pos::RMBPressEvent( const MT_Vector2D& vPos )
{
    bool bReturn = false;

    if ( !bMovingPoint_  )
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
        if( pointVector_.size() )
        {
            pSelectedPoint_ = *pointVector_.rbegin();
            pSelectedPoint_->SetPos( vPos );
            vOldPos_ = vPos;
            LocOnPoint( pSelectedPoint_ );
            bReturn = true;
        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_Itineraire1Pos::IsActive( bool bMove )
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
// Name: MOS_MTH_Itineraire1Pos::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
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
        pSelectedPoint_ = CreatePointFromInterface( point );
        setSelected( 0, pSelectedPoint_ );
//        setFocus();
    }
    RIT_PointPtrVector itPoint = pointVector_.rbegin();
    (*itPoint)->SetPos( pAgent_->GetPos() );
    (*itPoint)->setText( 0, pAgent_->GetName().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_MTH_Itineraire1Pos::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_MTH_Itineraire1Pos::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_Itineraire1Pos::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Itineraire1Pos::IsInsidePoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_MTH_Itineraire1Pos::IsInsidePoint( const MT_Vector2D& vPos ) const
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


