//*****************************************************************************
//
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_ListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:53 $
// $Revision: 7 $
// $Workfile: MOS_MTH_ListLocalisation_ListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_ListLocalisation_ListViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_ListLocalisation_ListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem constructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListViewItem::MOS_MTH_ListLocalisation_ListViewItem( QListView* pParent )
: QListViewItem( pParent )
{
    std::string strName( "Localisation " );
    strName += itostring( (int)(this) );

    setText( 0, strName.c_str() );

    vLocalisationPoint_.push_back( MOS_App::GetApp().GetWorld().GetRect().GetCenter() );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem destructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListViewItem::~MOS_MTH_ListLocalisation_ListViewItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::rtti
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
int MOS_MTH_ListLocalisation_ListViewItem::rtti() const
{
    return e_rtti;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::FillRandomParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListViewItem::FillRandomParameters()
{
    uint nRnd = rand() % 3;
    switch( nRnd )
    {
        case 0 : nRnd = 2/*li32*/;  break;
        case 1 : nRnd = 3/*po32*/;  break;
        case 2 : nRnd = 4/*pt*/;    break;
    }
    SetType( (ASN1T_EnumTypeLocalisation)nRnd );

    uint nWantedPoint = 0;
    if ( nRnd == 2 ) // Lignes
        nWantedPoint = 2;
    if ( nRnd == 3 ) // Polygones
        nWantedPoint = 3;
    if ( nRnd == 4 ) // Points
        nWantedPoint = 1;
    assert( nWantedPoint );
    
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        // $$$$ AGE 2005-02-07: 
        uint nX = 0; 
        uint nY = 0; 
        MT_Vector2D vPos2( nX, nY );
        AddPoint( vPos2 );
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::ResaveParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListViewItem::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nRnd = (uint)GetType();
    archive.WriteField( "sName", std::string( "MOS_MTH_Localisation|nRnd" ) );
    archive.WriteField( "nRnd", nRnd );

    uint nWantedPoint = vLocalisationPoint_.size();
    archive.WriteField( "sName", std::string( "MOS_MTH_ListLocalisation_ListViewItem|nWantedPoint" ) );
    archive.WriteField( "nWantedPoint", nWantedPoint );

    for ( IT_PointVector itPoint = vLocalisationPoint_.begin() ; itPoint != vLocalisationPoint_.end() ; ++itPoint  )  
    {
        (*itPoint).Write( archive );
    }
    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListViewItem::ReloadParameters
// Created: FBD 03-08-18
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListViewItem::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nRnd = 0;
    std::string sName;
    archive.ReadField( "sName", sName );
    assert( sName == "MOS_MTH_Localisation|nRnd" );
    archive.ReadField( "nRnd", nRnd );

    SetType( (ASN1T_EnumTypeLocalisation)nRnd );


    uint nWantedPoint = 0; 
// -------------------------------------------------
    archive.ReadField( "sName", sName );
    assert( sName == "MOS_MTH_ListLocalisation_ListViewItem|nWantedPoint" );
// -------------------------------------------------
    archive.ReadField( "nWantedPoint", nWantedPoint );
    for ( uint i = 0 ; i < nWantedPoint ; ++i )  
    {
        MT_Vector2D vPos2;
        vPos2.Read( archive );
        AddPoint( vPos2 );
    }
}
