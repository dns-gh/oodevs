//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_Manager.inl $
// $Author: Ape $
// $Modtime: 20/07/04 17:54 $
// $Revision: 3 $
// $Workfile: MOS_Meteo_Manager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::GetGlobalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
MOS_INLINE
MOS_Meteo_ABC& MOS_Meteo_Manager::GetGlobalWeather()
{
    return globalWeather_;
}


// -----------------------------------------------------------------------------
// Name: MOS_Meteo_Manager::GetLocalWeatherList
/** @return 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
MOS_INLINE
MOS_Meteo_Manager::T_Meteo_Vector& MOS_Meteo_Manager::GetLocalWeatherList()
{
    return localWeatherVector_;
}
