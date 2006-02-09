//*****************************************************************************
//
// $Created: AGN 03-08-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Meteo_ABC.inl $
// $Author: Ape $
// $Modtime: 20/07/04 17:42 $
// $Revision: 2 $
// $Workfile: Meteo_ABC.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetTemperature
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetTemperature( int nTemperature )
{
    nTemperature_ = nTemperature;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetWindSpeed
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetWindSpeed( int nWindSpeed )
{
    nWindSpeed_ = nWindSpeed;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetWindDirection
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetWindDirection( int nWindDirection )
{
    nWindDirection_ = nWindDirection;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetCloudCoverFloor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetCloudCoverFloor( int nFloor )
{
    nCloudCoverFloor_ = nFloor;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetCloudCoverCeiling
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetCloudCoverCeiling( int nCeiling )
{
    nCloudCoverCeiling_ = nCeiling;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetCloudDensity
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetCloudDensity( int nDensity )
{
    nCloudCoverAverageDensity_ = nDensity;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::SetWeatherType
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
void Meteo_ABC::SetWeatherType( Meteo_ABC::E_WeatherType nType )
{
    nWeatherType_ = nType;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetTemperature
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetTemperature() const
{
    return nTemperature_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetWindSpeed
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetWindSpeed() const
{
    return nWindSpeed_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetWindDirection
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetWindDirection() const
{
    return nWindDirection_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetCloudCoverFloor
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetCloudCoverFloor() const
{
    return nCloudCoverFloor_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetCloudCoverCeiling
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetCloudCoverCeiling() const
{
    return nCloudCoverCeiling_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetCloudDensity
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
int Meteo_ABC::GetCloudDensity() const
{
    return nCloudCoverAverageDensity_;
}


//-----------------------------------------------------------------------------
// Name: Meteo_ABC::GetWeatherType
// Created: AGN 03-08-06
//-----------------------------------------------------------------------------
inline
Meteo_ABC::E_WeatherType Meteo_ABC::GetWeatherType() const
{
    return nWeatherType_;
}

