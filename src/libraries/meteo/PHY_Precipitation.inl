// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Meteo/PHY_Precipitation.inl $
// $Author: Nld $
// $Modtime: 25/11/04 18:22 $
// $Revision: 2 $
// $Workfile: PHY_Precipitation.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetPrecipitation
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Precipitation::T_PrecipitationMap& PHY_Precipitation::GetPrecipitations()
{
    return precipitations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( const std::string& strName )
{
    CIT_PrecipitationMap it = precipitations_.find( strName );
    if( it == precipitations_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::FindPrecipitation
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
const PHY_Precipitation* PHY_Precipitation::FindPrecipitation( sword::WeatherAttributes::EnumPrecipitationType nAsnID )
{
    for( CIT_PrecipitationMap it = precipitations_.begin(); it != precipitations_.end(); ++it )
    {
        const PHY_Precipitation& precipitation = *it->second;
        if( precipitation.GetAsnID() == nAsnID )
            return &precipitation;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const std::string& PHY_Precipitation::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
E_WeatherType PHY_Precipitation::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Precipitation::GetAsnID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
inline
sword::WeatherAttributes::EnumPrecipitationType PHY_Precipitation::GetAsnID() const
{
    return nAsnID_;
}
