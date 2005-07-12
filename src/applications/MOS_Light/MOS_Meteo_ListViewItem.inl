//*****************************************************************************
//
// $Created: AGN 03-08-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_ListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_ListViewItem.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListViewItem::rtti
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ListViewItem::rtti() const
{
    return e_rtti;
}



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ListViewItem::GetLocalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
MOS_Meteo_Local& MOS_Meteo_ListViewItem::GetLocalWeather()
{
    return localWeather_;
}

