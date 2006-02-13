//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_Manager.inl $
// $Author: Ape $
// $Modtime: 20/07/04 17:54 $
// $Revision: 3 $
// $Workfile: Meteo_Manager.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: Meteo_Manager::GetGlobalWeather
// Created: AGN 03-08-07
//-----------------------------------------------------------------------------
inline
Meteo_ABC& Meteo_Manager::GetGlobalWeather()
{
    return globalWeather_;
}


// -----------------------------------------------------------------------------
// Name: Meteo_Manager::GetLocalWeatherList
/** @return 
*/
// Created: APE 2004-05-17
// -----------------------------------------------------------------------------
inline
Meteo_Manager::T_Meteo_Vector& Meteo_Manager::GetLocalWeatherList()
{
    return localWeatherVector_;
}
