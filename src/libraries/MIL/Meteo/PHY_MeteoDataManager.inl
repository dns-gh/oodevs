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

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::RegisterMeteo
// Created: NLD 2006-03-13
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::RegisterMeteo( PHY_Meteo& meteo )
{
    bool bOut = meteos_.insert( &meteo ).second;
    assert( bOut );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterMeteo
// Created: NLD 2006-03-13
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::UnregisterMeteo( PHY_Meteo& meteo )
{
    int nOut = meteos_.erase( &meteo );
    assert( nOut == 1 );
}
  