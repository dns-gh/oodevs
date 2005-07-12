//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_PointListView.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:44 $
// $Revision: 6 $
// $Workfile: MOS_Meteo_PointListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_Meteo_PointListView.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_PointListView.inl"
#endif

#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_PointListView::MOS_Meteo_PointListView( QWidget* pParent )
: MOS_PointListView( pParent, "", eListViewEditor )
, bMovingLine_( false )
, vOldPos_()
{
    disconnect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_PointListView::~MOS_Meteo_PointListView()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::Draw
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::Draw()
{
    GFX_Color color( 255,255,0 );

    if ( pointVector_.size() != 2  )
        return;

    // Draw points
    MOS_PointListViewItem* pFirstPoint = pointVector_[0];
    MOS_PointListViewItem* pSecondPoint = pointVector_[1];

    pFirstPoint->Draw( pSelectedPoint_ );
    pSecondPoint->Draw( pSelectedPoint_ );

    // Draw the rectangle
    MT_Vector2D firstPt = pFirstPoint->GetPoint();
    MT_Vector2D secondPt( firstPt );
    MT_Vector2D thirdPt = pSecondPoint->GetPoint();
    MT_Vector2D fourthPt( thirdPt );
    secondPt.rY_ = thirdPt.rY_;
    fourthPt.rY_ = firstPt.rY_;

    MT_Float rSizeLine = 100.; // $$$$ AGE 2005-02-07: 
    
    GFX_Tools::CreateGLLine( firstPt, secondPt, rSizeLine, color );
    GFX_Tools::CreateGLLine( secondPt, thirdPt, rSizeLine, color );
    GFX_Tools::CreateGLLine( thirdPt, fourthPt, rSizeLine, color );
    GFX_Tools::CreateGLLine( fourthPt, firstPt, rSizeLine, color );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::IsActive
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
bool MOS_Meteo_PointListView::IsActive( bool /*bMove*/ )
{
    return hasFocus();    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::RMBPressEvent
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
bool MOS_Meteo_PointListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    bool bReturn = false;

    if ( !bMovingPoint_ && !bMovingLine_ )
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
        if ( bCtrl_ && pointVector_.size() )
        {
            vPosMovingLine_ = vPos;
            pSelectedPoint_ = *pointVector_.begin();
            setSelected( pSelectedPoint_, true );
            bMovingLine_ = true;
            bReturn = true;
        }
        else
        if ( bShift_  )
        {
            vOldPos_ = vPos;
            MT_Vector2D vNewPos( vPos );
            pSelectedPoint_ = &CreatePoint( vNewPos, pSelectedPoint_ );
            LocOnPoint( pSelectedPoint_ );
            bReturn = true;
        }
        else
        if ( pSelectedPoint_  )
        {
            pSelectedPoint_->SetPos( vPos );
            vOldPos_ = vPos;
            LocOnPoint( pSelectedPoint_ );
            bReturn = true;
        }
    }
    return bReturn;

}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::LMBPressEvent
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
bool MOS_Meteo_PointListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::MouseMoveEvent
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( *pSelectedPoint_, vPos, vPosMovingLine_ );    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::RMBReleaseEvent
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::LMBReleaseEvent
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::GetCenterPoint
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MT_Vector2D MOS_Meteo_PointListView::GetCenterPoint( MOS_PointListViewItem* pPoint )
{
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


    //. Clipping dans le monde
    // $$$$ AGE 2005-02-07: 
//    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
//    if ( !rectWorld.IsInside( vNewPos ) )
//    {
//        if ( pPoint )
//            vNewPos = pPoint->GetPoint();
//                        
//    }
    vOldPos_ = vNewPos;

    return vNewPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::IsInsidePoint
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_Meteo_PointListView::IsInsidePoint( const MT_Vector2D& vPos ) const
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
// Name: MOS_Meteo_PointListView::SetPosLine
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::SetPosLine( MOS_PointListViewItem& /*point*/, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    if ( pointVector_.size() < 2 )
        return;
    
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    CIT_PointPtrVector itPoint = pointVector_.begin();
    ++itPoint;
    for (  ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::FillRandomParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::FillRandomParameters()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::ReloadParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_PointListView::ResaveParameters
// Created: FBD 03-09-09
//-----------------------------------------------------------------------------
void MOS_Meteo_PointListView::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
}


