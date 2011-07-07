// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Lighting.inl $
// $Author: Nld $
// $Modtime: 19/10/04 13:55 $
// $Revision: 1 $
// $Workfile: PHY_Lighting.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetLightings
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Lighting::T_LightingMap& PHY_Lighting::GetLightings()
{
    return lightings_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindLighting
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Lighting* PHY_Lighting::FindLighting( const std::string& strName )
{
    CIT_LightingMap it = lightings_.find( strName );
    if( it == lightings_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::FindPrecipitation
// Created: SLG 2010-05-26
// -----------------------------------------------------------------------------
inline
const PHY_Lighting* PHY_Lighting::FindLighting( sword::WeatherAttributes::EnumLightingType nAsnID )
{
    for( CIT_LightingMap it = lightings_.begin(); it != lightings_.end(); ++it )
    {
        const PHY_Lighting& lighting = *it->second;
        if( lighting.GetAsnID() == nAsnID )
            return &lighting;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Lighting::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
E_LightingType PHY_Lighting::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Lighting::GetAsnID
// Created: SLG 2010-05-26
// -----------------------------------------------------------------------------
inline
sword::WeatherAttributes::EnumLightingType PHY_Lighting::GetAsnID() const
{
    return nAsnID_;
}
