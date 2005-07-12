//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::GetRawVisionData
// Created: JVT 02-11-20
//-----------------------------------------------------------------------------
inline
const PHY_RawVisionData& PHY_MeteoDataManager::GetRawVisionData() const
{
    assert( pRawData_ );
    return *pRawData_;
}


//-----------------------------------------------------------------------------
// Name: PHY_Meteo::sEphemeride
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
const PHY_Ephemeride& PHY_MeteoDataManager::GetEphemeride() const
{
    assert( pEphemeride_ );
    return *pEphemeride_;
}

