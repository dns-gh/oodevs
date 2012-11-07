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
// Name: Meteo::sEphemeride
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
inline
const PHY_Ephemeride& PHY_MeteoDataManager::GetEphemeride() const
{
    assert( pEphemeride_ );
    return *pEphemeride_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::AddMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::AddMeteo( weather::Meteo& meteo )
{
    meteo.Update( pEphemeride_->GetLightingBase() );
    meteos_.push_back( boost::shared_ptr< weather::Meteo >( &meteo ) );
}
