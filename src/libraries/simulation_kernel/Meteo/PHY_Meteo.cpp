//*****************************************************************************
//
// $Created: JVT 03-08-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/PHY_Meteo.cpp $
// $Author: Jvt $
// $Modtime: 29/10/04 10:35 $
// $Revision: 2 $
// $Workfile: PHY_Meteo.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Meteo.h"

#include "Meteo/PHY_MeteoDataManager.h"
#include "Meteo/PHY_Lighting.h"
#include "Meteo/PHY_Precipitation.h"
#include "PHY_Ephemeride.h"
#include "MIL_AgentServer.h"

#include "Tools/MIL_Tools.h"
#include "Network/NET_ASN_Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( xml::xistream& xis, const PHY_Ephemeride& ephemeride )
    : pLighting_     ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_( &PHY_Precipitation::none_ )
    , nRefCount_     ( 0 )
{
    uint nVal;
    xis >> start( "cloud-cover" )
            >> attribute( "floor", nPlancherCouvertureNuageuse_ )
            >> attribute( "ceiling", nPlafondCouvertureNuageuse_ )
            >> attribute( "density", nVal )
        >> end();
    rDensiteCouvertureNuageuse_ = std::min( nVal, (uint)100 ) / 100.;

    uint nAngle;
    xis >> start( "wind" )
            >> attribute( "speed", wind_.rWindSpeed_ )
            >> attribute( "speed", nAngle )
        >> end();
    if( wind_.rWindSpeed_ < 0 )
        xis.error( "meteo: VitesseVent < 0" );
    wind_.rWindSpeed_ = MIL_Tools::ConvertSpeedMosToSim( wind_.rWindSpeed_ );
    if( nAngle < 0 || nAngle > 360 )
        xis.error( "meteo: DirectionVent not in [0..360]" );
    NET_ASN_Tools::ReadDirection( nAngle, wind_.vWindDirection_ );

    std::string strVal;
    xis >> start( "precipitation" )
            >> attribute( "value", strVal )
        >> end();
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( strVal );
    if( !pPrecipitation_ )
        xis.error( "Unknown Precipitation type '" + strVal + "'" );
    Update( ephemeride );
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo constructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::PHY_Meteo( const ASN1T_MeteoAttributes& asnMsg )
    : pLighting_     ( &PHY_Lighting::jourSansNuage_ )
    , pPrecipitation_( &PHY_Precipitation::none_ )
    , nRefCount_     ( 0 )
{
    Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo destructor
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
PHY_Meteo::~PHY_Meteo()
{
    MIL_AgentServer::GetWorkspace().GetMeteoDataManager().UnregisterMeteo( *this );
    assert( nRefCount_ == 0 );
}

// -----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_Meteo::Update( const ASN1T_MeteoAttributes& asnMsg )
{
    // Plancher de couverture nuageuse
    nPlancherCouvertureNuageuse_ = asnMsg.cloud_floor;

    // Plafond de couverture nuageuse
    nPlafondCouvertureNuageuse_ = asnMsg.cloud_ceiling;

    // Densite moyenne de couverture nuageuse
    rDensiteCouvertureNuageuse_ = std::min( std::max( asnMsg.cloud_density, 0 ), 100 ) / 100.;

    // Vitesse du vent
    wind_.rWindSpeed_ = MIL_Tools::ConvertSpeedMosToSim( asnMsg.wind_speed );

    // Direction du vent
    NET_ASN_Tools::ReadDirection( asnMsg.wind_direction, wind_.vWindDirection_ );

    // Pr�cipitation
    pPrecipitation_ = PHY_Precipitation::FindPrecipitation( asnMsg.precipitation );
    if( !pPrecipitation_ )
    {
        assert( false );
        pPrecipitation_ = &PHY_Precipitation::none_;
    }

    Update( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetEphemeride() );
}

//-----------------------------------------------------------------------------
// Name: PHY_Meteo::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_Meteo::Update( const PHY_Ephemeride& ephemeride )
{
    pLighting_ = &ephemeride.GetLightingBase().GetDegradedLighting( (uint)( rDensiteCouvertureNuageuse_ * ( nPlafondCouvertureNuageuse_ - nPlancherCouvertureNuageuse_ ) / 2000 ) );
}
