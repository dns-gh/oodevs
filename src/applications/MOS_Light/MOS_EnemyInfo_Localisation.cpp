//*****************************************************************************
//
// $Created: AGN 03-05-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_Localisation.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 4 $
// $Workfile: MOS_EnemyInfo_Localisation.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_EnemyInfo_Localisation.h"

#ifndef MOS_USE_INLINE
#	include "MOS_EnemyInfo_Localisation.inl"
#endif

#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation constructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Localisation::MOS_EnemyInfo_Localisation( QWidget* pParent, const std::string& strName)
: MOS_PointListView( pParent, strName, eListViewEditor )
, vOldPos_        ( 0., 0.)
, bMovingLine_    ( false )
, bFirstWidget_   ( false )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation destructor
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_EnemyInfo_Localisation::~MOS_EnemyInfo_Localisation()
{
    
}

 

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::Initialize
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::Initialize()
{
    MOS_PointListView::Initialize();    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::Terminate
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::Terminate()
{
    MOS_PointListView::Terminate();    
}

    

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::Draw
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::Draw()
{
    if( pointVector_.empty() )
        return;

    IT_PointPtrVector itPoint;
    for( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        (*itPoint)->Draw( pSelectedPoint_ ); 
    }

    if( pointVector_.size() == 1 )
        return;

    GFX_Color color( 255,255,255 );

    MOS_PointListViewItem* pLastPoint = 0;
    MOS_PointListViewItem* pFirstPoint = pointVector_.front();
    for( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            GFX_Tools::CreateGLLine( pLastPoint->GetPoint(), pCurPoint->GetPoint(), 2.0, color );
        }
        pLastPoint = pCurPoint;
    }

    if( pointVector_.size() > 2 )
        GFX_Tools::CreateGLLine( pLastPoint->GetPoint(), pFirstPoint->GetPoint(), 2.0, color );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::IsActive
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Localisation::IsActive( bool /*bMove*/ )
{
    return ! pointVector_.empty() && hasFocus();
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::RMBPressEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Localisation::RMBPressEvent( const MT_Vector2D& vPos )
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
// Name: MOS_EnemyInfo_Localisation::LMBPressEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
bool MOS_EnemyInfo_Localisation::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;        
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::MouseMoveEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( vPos, vPosMovingLine_ );    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::RMBReleaseEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::LMBReleaseEvent
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}



//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_Localisation::GetCenterPoint
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MT_Vector2D MOS_EnemyInfo_Localisation::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_EnemyInfo_Localisation::IsInsidePoint
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_EnemyInfo_Localisation::IsInsidePoint( const MT_Vector2D& vPos ) const
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
// Name: MOS_EnemyInfo_Localisation::SlotContextMenuRequested
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
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
// Name: MOS_EnemyInfo_Localisation::SetPosLine
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_Localisation::SetPosLine( const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;    
}
