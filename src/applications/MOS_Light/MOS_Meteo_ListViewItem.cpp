//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_ListViewItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_ListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_Meteo_ListViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Meteo_ListViewItem.inl"
#endif

#include "MOS_Meteo_Local.h"

//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListViewItem constructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_ListViewItem::MOS_Meteo_ListViewItem( QListView* pParent, MOS_Meteo_Local& localWeather )
: QListViewItem( pParent )
, localWeather_( localWeather )
{
    std::string strName( "Zone météo " );
    strName += itostring( localWeather.GetId() );
    setText( 0, strName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListViewItem destructor
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_Meteo_ListViewItem::~MOS_Meteo_ListViewItem()
{
    
}

