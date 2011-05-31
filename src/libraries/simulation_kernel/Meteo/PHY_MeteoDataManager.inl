//*****************************************************************************
// Created: JVT 02-10-21
//*****************************************************************************

#include "MT_Tools/MT_ScipioException.h"

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
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::RegisterMeteo( weather::PHY_Meteo& element )
{
    element.SetListener( this );
    element.Update( pEphemeride_->GetLightingBase() );
    if( ! meteos_.insert( &element ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Insert failed" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
inline
void PHY_MeteoDataManager::UnregisterMeteo( weather::PHY_Meteo& element )
{
    if( meteos_.erase( &element ) != 1 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Erase failed" );

}
