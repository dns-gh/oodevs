//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_ListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 15:42 $
// $Revision: 8 $
// $Workfile: MOS_MTH_Obstacle_ListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_Obstacle_ListViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_Obstacle_ListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_ListViewItem::MOS_MTH_Obstacle_ListViewItem( QListView* pParent )
: QListViewItem( pParent )
, nTypeObstacle_( EnumObjectType::abattis )
, nTypeLocalisation_( EnumTypeLocalisation::point )
, nUrgence_( EnumMissionGenUrgence::serie_minimum )
, nPreliminaire_( EnumMissionGenSousTypeObstacle::de_manoeuvre )
, nPriorite_( EnumMissionGenPriorite::normal )
, nIDPlanifie_( 0 )
{
    std::string strName( "Obstacle " );
    strName += itostring( (int)(this) );
    nID_ = (uint)(this);

    setText( 0, strName.c_str() );

    // $$$$ AGE 2005-02-07: 
    vLocalisationPoint_.push_back( MT_Vector2D( 0, 0 ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_ListViewItem::~MOS_MTH_Obstacle_ListViewItem()
{
    
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::rtti
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
int MOS_MTH_Obstacle_ListViewItem::rtti() const
{
    return e_rtti;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::FillRandomParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListViewItem::FillRandomParameters( MT_OutputArchive_ABC& /*archive*/ )
{
    // to do
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListViewItem::ReloadParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListViewItem::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
    // to do    
}

