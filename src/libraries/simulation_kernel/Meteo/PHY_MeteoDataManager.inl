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
// Name: PHY_MeteoDataManager::GetLighting
// Created: HBD 2010-03-10  
// -----------------------------------------------------------------------------
inline
const weather::PHY_Lighting& PHY_MeteoDataManager::GetLighting() const
{
    assert( pEphemeride_ );
    return pEphemeride_->GetLightingBase();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::RegisterMeteo
// Created: NLD 2006-03-13
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::RegisterMeteo( weather::PHY_Meteo& meteo )
{
    meteo.SetListener( this );
    meteo.Update( pEphemeride_->GetLightingBase() );
    if( ! meteos_.insert( &meteo ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterMeteo
// Created: NLD 2006-03-13
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::UnregisterMeteo( weather::PHY_Meteo& meteo )
{
    if( meteos_.erase( &meteo ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );
}