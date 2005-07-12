//*****************************************************************************
//
// $Created: AGN 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Direction.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 5 $
// $Workfile: MOS_EnemyInfo_Direction.cpp $
//
//*****************************************************************************
 
#include "MOS_Light_Pch.h"
#include "MOS_EnemyInfo_Direction.h"

#ifndef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Direction.inl"
#endif

#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction constructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Direction::MOS_EnemyInfo_Direction( QWidget* pParent, const std::string& strName )
: MOS_PointListView( pParent, strName ,eListViewEditor )
, vOldPos_        ( 0., 0.)
, bMovingLine_    ( false )
, bFirstWidget_   ( false )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction destructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Direction::~MOS_EnemyInfo_Direction()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::Initialize
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::Initialize()
{
    MOS_PointListView::Initialize();    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::Terminate
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::Terminate()
{
    MOS_PointListView::Terminate();    
}

    

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::Draw
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::Draw()
{
    assert( pointVector_.size() == 2 );

    
    GFX_Color color( 255,255,0 );

    pointVector_[0]->Draw();
    pointVector_[1]->Draw();

    
    MT_Float rSizearrow = 100. ;
    GFX_Tools::CreateGLArrow( pointVector_[0]->GetPos(), pointVector_[1]->GetPos(), color, 4, rSizearrow );    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::IsActive
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Direction::IsActive( bool /*bMove*/ )
{
    return pointVector_.size() > 0 && hasFocus();
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::RMBPressEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Direction::RMBPressEvent( const MT_Vector2D& vPos )
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
// Name: MOS_EnemyInfo_Direction::LMBPressEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Direction::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::MouseMoveEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( vPos, vPosMovingLine_ );    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::RMBReleaseEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::LMBReleaseEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}



//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::GetCenterPoint
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MT_Vector2D MOS_EnemyInfo_Direction::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_EnemyInfo_Direction::IsInsidePoint
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_EnemyInfo_Direction::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    if ( pointVector_.size() < 2 )
        return 0; 

    
    if ( pointVector_[0]->IsInside( vPos ) )
        return pointVector_[0];

    if ( pointVector_[1]->IsInside( vPos ) )
            return pointVector_[1];

    return 0;    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::SlotContextMenuRequested
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::SlotContextMenuRequested( QListViewItem* /*pItem*/, const QPoint& /*point*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Direction::SetPosLine
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Direction::SetPosLine( const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;    
}
