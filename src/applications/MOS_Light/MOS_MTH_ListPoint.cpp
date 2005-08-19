//*****************************************************************************
//
// $Created: FBD 03-01-13 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListPoint.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:41 $
// $Revision: 3 $
// $Workfile: MOS_MTH_ListPoint.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_MTH_ListPoint.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_ListPoint.inl"
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
// Name: MOS_MTH_ListPoint constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_ListPoint::MOS_MTH_ListPoint( ASN1T_ListPoint& asnListPoint, QWidget* pParent, const std::string& sName )
    :   MOS_PointListView( pParent, sName, eListViewEditor )
    ,   asnListPoint_    ( asnListPoint )
    ,   vOldPos_         ( 0., 0.)
    ,   pAgent_          ( 0 )
    ,   bMovingLine_     ( false )
    ,   bFirstWidget_    ( false )
    ,   sName_           ( sName )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_MTH_ListPoint::~MOS_MTH_ListPoint()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::Initialize()
{
    MOS_PointListView::Initialize();
    asnListPoint_.n = 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::Terminate()
{
    for( uint i = 0; i < asnListPoint_.n; ++i )
    {
        ASN1T_Point& point = asnListPoint_.elem[i];
        if( point.vecteur_point.n > 0 )
            delete [] point.vecteur_point.elem;
    }

    if( asnListPoint_.n > 0 )
        delete [] asnListPoint_.elem;
    MOS_PointListView::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::WriteMsg( std::string& sParam )
{
    sParam += sName_;
    assert( pointVector_.size() > 0 );
    uint nNbrPoints = pointVector_.size() - 1;
    sParam += MT_FormatString( " : %d Coords |", nNbrPoints );

    asnListPoint_.n = nNbrPoints;
    if( nNbrPoints == 0 )
        return;

    asnListPoint_.elem = new ASN1T_Point[ nNbrPoints ];

    int i = 0;
    IT_PointPtrVector itPoint = pointVector_.begin();
    ++itPoint;
    for( ; itPoint != pointVector_.end() ; ++itPoint )
    {
        ASN1T_Point& point = asnListPoint_.elem[i];
        point.type = EnumTypeLocalisation::point;
        point.vecteur_point.n = 1;
        point.vecteur_point.elem = new ASN1T_CoordUTM[ 1 ];

        MOS_PointListViewItem* pPointItem = (MOS_PointListViewItem*)(*itPoint);
        pPointItem->WriteMsg( point.vecteur_point.elem[0], sParam );
        ++i;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::FillRandomParameters()
{
    DeleteAllPoints();

    assert( pAgent_ );

    MT_Vector2D vPos = pAgent_->GetPos();
    MOS_PointListViewItem& point = CreatePoint( vPos );
    point.setText( 0, pAgent_->GetName().c_str() );

    uint nWantedPoint = ( rand() % 8 ) + 1;

    assert( nWantedPoint  >= 1 );

    MOS_PointListViewItem* pLastPoint = 0;
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        // $$$$ AGE 2005-02-07: 
        uint nX = 0; // rand() % (uint)MOS_App::GetApp().GetWorld().GetExtent().GetRect().GetWidth();
        uint nY = 0; // rand() % (uint)MOS_App::GetApp().GetWorld().GetExtent().GetRect().GetHeight();
        MT_Vector2D vPos2( nX, nY );
        pLastPoint = &CreatePoint( vPos2, pLastPoint );
        
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::ResaveParameters( MT_OutputArchive_ABC& archive )
{

    uint nWantedPoint = pointVector_.size() - 1;
    assert( nWantedPoint  >= 1 );

    archive.WriteField( "sName", std::string( "MOS_MTH_ListPoint|nWantedPoint" ) );
    archive.WriteField( "nWantedPoint", nWantedPoint );
    
    for ( uint i = 1 ; i < pointVector_.size() ; ++i )
    {
        const MT_Vector2D& vPos = pointVector_[i]->GetPos();
        vPos.Write( archive );
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::ReloadParameters( MT_InputArchive_ABC& archive )
{
    DeleteAllPoints();

    assert( pAgent_ );

    MT_Vector2D vPos = pAgent_->GetPos();
    MOS_PointListViewItem& point = CreatePoint( vPos );
    point.setText( 0, pAgent_->GetName().c_str() );

    uint nWantedPoint = 0; //rand() % 8 + 1;
    std::string sName;
    archive.ReadField( "sName", sName );
    assert( sName == "MOS_MTH_ListPoint|nWantedPoint" );
    archive.ReadField( "nWantedPoint", nWantedPoint );

    assert( nWantedPoint >= 1 );

    MOS_PointListViewItem* pLastPoint = 0;
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        MT_Vector2D vPos2;
        vPos2.Read( archive );
        pLastPoint = &CreatePoint( vPos2, pLastPoint );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::SetAgent( MOS_Agent* pAgent )
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
// Name: MOS_MTH_ListPoint::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
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
// Name: MOS_MTH_ListPoint::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::Draw()
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
            GFX_Tools::CreateGLArrow( pLastPoint->GetPoint(), pCurPoint->GetPoint(), color, 4, rSizearrow );
        }
        pLastPoint = pCurPoint;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_ListPoint::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_MTH_ListPoint::RMBPressEvent( const MT_Vector2D& vPos )
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
// Name: MOS_MTH_ListPoint::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( bMovingPoint_ && pSelectedPoint_  )
        SetPosPoint( *pSelectedPoint_, vPos );
    else
    if ( bMovingLine_ && pSelectedPoint_  )
        SetPosLine( *pSelectedPoint_, vPos, vPosMovingLine_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_MTH_ListPoint::IsActive( bool bMove )
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
// Name: MOS_MTH_ListPoint::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& /*vPos*/ )
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
// Name: MOS_MTH_ListPoint::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MT_Vector2D MOS_MTH_ListPoint::GetCenterPoint( MOS_PointListViewItem* pPoint )
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
// Name: MOS_MTH_ListPoint::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::SetMission( MOS_Mission_ABC* pMission )
{
    pMission_ = pMission;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::SetWidgetTab( QWidget* pWidget )
{
    pWidgetTab_ = pWidget;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListPoint::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_MTH_ListPoint::IsInsidePoint( const MT_Vector2D& vPos ) const
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
// Name: MOS_PointListView::SetPosLine
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_ListPoint::SetPosLine( MOS_PointListViewItem& /*point*/, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
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

