//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Ephemeride.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Ephemeride.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
const PHY_Lighting& PHY_Ephemeride::GetLightingBase() const
{
    assert( pNightBase_ );
    return bIsNight_ ? *pNightBase_ : PHY_Lighting::jourSansNuage_;
}

//-----------------------------------------------------------------------------
// Name: PHY_Ephemeride::GetCurrentTimeBase
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
bool PHY_Ephemeride::IsNight() const
{
    return bIsNight_;
}

