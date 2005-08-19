//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 12 $
// $Workfile: MOS_LimitListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_LimitListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_LimitListViewItem.inl"
#endif

#include "MOS_Limit.h"
#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_LimitListView.h"
#include "MOS_Agent.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"


using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitListViewItem::MOS_LimitListViewItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitListViewItem::MOS_LimitListViewItem( E_ItemType nItemType, MOS_Limit& Limit, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitListViewItem::MOS_LimitListViewItem( E_ItemType nItemType,  MOS_Limit& Limit, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LimitListViewItem::~MOS_LimitListViewItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::AddPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem& MOS_LimitListViewItem::CreatePoint( MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter )
{
    MOS_PointListViewItem* pPointLimit = 0;
    if ( pointVector_.size() )
    {
        if ( pPointAfter )
            pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, *pPointAfter );
        else
            pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, **pointVector_.rbegin() );
    }
    else
        pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this );

    RegisterPoint( *pPointLimit, pPointAfter );
    return ( *pPointLimit );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::DeletePoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitListViewItem::DeletePoint( MT_Vector2D& vPoint )
{
    MOS_PointListViewItem* pPoint = GetPoint( vPoint );
    assert( pPoint );
    UnregisterPoint( *pPoint );
    delete pPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitListViewItem::Draw( MOS_LimitListViewItem* /*pSelectedLimit*/, MOS_PointListViewItem* pSelectedPoint ) const
{
    GFX_Color color( 255,255,255 );

    MOS_PointListViewItem* pLastPoint = 0;
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            GFX_Tools::CreateGLLine( pLastPoint->GetPoint(), pCurPoint->GetPoint(), 2.0, color );
        }
        pLastPoint = pCurPoint;
    }

    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        (*itPoint)->Draw( pSelectedPoint );
        
    }

    MT_Float rSizeName = 500. ;
    
    GFX_Color colorName( 0,255,255 );
    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MT_Vector2D vPos( (*itPoint)->GetPoint() );
        vPos += (*itPoint)->GetSize() / 2.;
        GFX_Tools::PrintWithShadows( vPos, rSizeName, 4., 8., colorName, false, GetName().c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitListViewItem::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pCurPoint->IsInside( vPos ) )
            return *itPoint;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::IsInsideLine
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitListViewItem::IsInsideLine( const MT_Vector2D& vPos ) const
{
    MT_Float rWeldValue = 4.0;
    MOS_PointListViewItem* pLastPoint = 0;
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            MT_Line line( pLastPoint->GetPoint(), pCurPoint->GetPoint() );
            MT_Vector2D vPosCol = line.ClosestPointOnLine( vPos );
            MT_Float rDistance = vPosCol.Distance( vPos );
            if(  rDistance < rWeldValue )
            {
                if ( pLastPoint )
                    return pLastPoint;
                return pCurPoint;
            }

        }
        pLastPoint = pCurPoint;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::SetPos
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitListViewItem::SetPos( const MT_Vector2D& vPos,  MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
   
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
const MT_Vector2D MOS_LimitListViewItem::GetCenterPoint( MOS_PointListViewItem* pPoint ) const
{
    MT_Vector2D vNewPos;

    uint nPoint = pointVector_.size();

    if ( pPoint == 0 && nPoint )
        pPoint = *pointVector_.rbegin();

    MOS_LimitListView* pListView = (MOS_LimitListView*)listView();

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
                vNewPos = ( ( pPointAfter->GetPos() - (*itPoint)->GetPos() ) + (*itPoint)->GetPos() );

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
            MT_Vector2D vPos = (*itPoint)->GetPos();
            vNewPos = MT_Vector2D( vPos.rX_ + ( (MT_Float)( rand() % 50 ) ), vPos.rY_ + ( (MT_Float)( rand() % 50 ) )  );
        }
    }
    else
    {
        vNewPos = MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().GetCenter();
        // Truc imbitable pour que ca marche la faute à FBD :p
        vNewPos.rY_ =  - vNewPos.rY_ + MOS_App::GetApp().GetWorld().GetRect().GetHeight();
    }

    //. Clipping dans le monde
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if ( !rectWorld.IsInside( vNewPos ) )
    {
        if ( pPoint )
            vNewPos = pPoint->GetPoint();
                        
    }
    
    (pListView)->SetOldPos( vNewPos );

    return vNewPos;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::GetPointBefore
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitListViewItem::GetPointBefore( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.begin() )
    {
        return 0;
    }
    --itPoint;
    return *itPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::GetPointAfter
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitListViewItem::GetPointAfter( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.rbegin() )
    {
        return 0;
    }
    ++itPoint;
    return *itPoint;
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_LimitListViewItem::SetName( const std::string sName )
{
    setText(0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::UpdateNamePos
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------

void MOS_LimitListViewItem::UpdateNamePos()
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitListViewItem::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_LimitListViewItem::WriteMsg( ASN1T_OID& asnOID, std::string& sParam )
{
    asnOID = Limit_.GetID();
    sParam += "----->";
    sParam += Limit_.GetName();
    sParam += " | ";
}

