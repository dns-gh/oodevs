//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Meteo_Global_GUI.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_Global_GUI.inl $
//
//*****************************************************************************



//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Global_GUI::SetWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_Global_GUI::SetWeather( MOS_Meteo_ABC* pWeather )
{
    pWeather_ = pWeather;
    if( pWeather_ != 0 )
        UpdateGUI();
}

