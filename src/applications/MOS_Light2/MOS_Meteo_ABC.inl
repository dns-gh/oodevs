//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Meteo_ABC.inl $
// $Author: Ape $
// $Modtime: 20/07/04 17:42 $
// $Revision: 2 $
// $Workfile: MOS_Meteo_ABC.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetTemperature
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetTemperature( int nTemperature )
{
    nTemperature_ = nTemperature;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetWindSpeed
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetWindSpeed( int nWindSpeed )
{
    nWindSpeed_ = nWindSpeed;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetWindDirection
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetWindDirection( int nWindDirection )
{
    nWindDirection_ = nWindDirection;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetCloudCoverFloor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetCloudCoverFloor( int nFloor )
{
    nCloudCoverFloor_ = nFloor;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetCloudCoverCeiling
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetCloudCoverCeiling( int nCeiling )
{
    nCloudCoverCeiling_ = nCeiling;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetCloudDensity
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetCloudDensity( int nDensity )
{
    nCloudCoverAverageDensity_ = nDensity;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::SetWeatherType
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void MOS_Meteo_ABC::SetWeatherType( MOS_Meteo_ABC::E_WeatherType nType )
{
    nWeatherType_ = nType;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetTemperature
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetTemperature() const
{
    return nTemperature_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetWindSpeed
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetWindSpeed() const
{
    return nWindSpeed_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetWindDirection
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetWindDirection() const
{
    return nWindDirection_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetCloudCoverFloor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetCloudCoverFloor() const
{
    return nCloudCoverFloor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetCloudCoverCeiling
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetCloudCoverCeiling() const
{
    return nCloudCoverCeiling_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetCloudDensity
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int MOS_Meteo_ABC::GetCloudDensity() const
{
    return nCloudCoverAverageDensity_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Meteo_ABC::GetWeatherType
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
MOS_Meteo_ABC::E_WeatherType MOS_Meteo_ABC::GetWeatherType() const
{
    return nWeatherType_;
}

