//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_ListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:42 $
// $Revision: 4 $
// $Workfile: MOS_MTH_PointList_ListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_PointList_ListViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_PointList_ListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_ListViewItem::MOS_MTH_PointList_ListViewItem( QListView* pParent )
: QListViewItem( pParent )
{
    std::string strName( "PointList " );
    strName += itostring( (int)(this) );

    setText( 0, strName.c_str() );

    // $$$$ AGE 2005-02-07: 
    vLocalisationPoint_.push_back( MT_Vector2D( 0, 0 ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_ListViewItem::~MOS_MTH_PointList_ListViewItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::FillRandomParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListViewItem::FillRandomParameters()
{
    uint nWantedPoint = rand() % ( 8 + 1 + 3 ); // 3 car attention plusieurs cas possible
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        uint nX = 0; // rand() % (uint)MOS_App::GetApp().GetWorld().GetExtent().GetRect().GetWidth();
        uint nY = 0; // rand() % (uint)MOS_App::GetApp().GetWorld().GetExtent().GetRect().GetHeight();
        MT_Vector2D vPos2( nX, nY );
        AddPoint( vPos2 );
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListViewItem::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nWantedPoint = vLocalisationPoint_.size();
    archive.WriteField( "sName", std::string( "MOS_MTH_PointList_ListViewItem|nWantedPoint" ) );
    archive.WriteField( "nWantedPoint", nWantedPoint );

    for ( IT_PointVector itPoint = vLocalisationPoint_.begin() ; itPoint != vLocalisationPoint_.end() ; ++itPoint  )  
    {
        (*itPoint).Write( archive );
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListViewItem::ReloadParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListViewItem::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nWantedPoint = 0; //rand() % ( 8 + 1 + 3 ); // 3 car attention plusieurs cas possible
// -------------------------------------------------

    std::string sName;
    archive.ReadField( "sName", sName );
    assert( sName == "MOS_MTH_PointList_ListViewItem|nWantedPoint" );
    
// -------------------------------------------------
    archive.ReadField( "nWantedPoint", nWantedPoint );
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        MT_Vector2D vPos2;
        vPos2.Read( archive );
        AddPoint( vPos2 );
    }
    
}

