// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Tiredness.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:48 $
// $Revision: 2 $
// $Workfile: PHY_Tiredness.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Tiredness.h"
#include "xeumeuleu/xml.h"

using namespace xml;

PHY_Tiredness::T_TirednessMap PHY_Tiredness::tirednesses_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

const PHY_Tiredness PHY_Tiredness::normal_ ( "Normal" , eNormal , EnumUnitTiredness::normal , 1. );
const PHY_Tiredness PHY_Tiredness::fatigue_( "Fatigue", eFatigue, EnumUnitTiredness::fatigue, 1. );
const PHY_Tiredness PHY_Tiredness::epuise_ ( "Epuise" , eEpuise , EnumUnitTiredness::epuise , 1. );

struct PHY_Tiredness::LoadingWrapper
{
    void ReadTiredness( xml::xistream& xis )
    {
        PHY_Tiredness::ReadTiredness( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Tiredness::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing tirenesses" );

    LoadingWrapper loader;

    xis >> start( "humans-factors" )
            >> start( "tiredness-factor" )
                >> list( "modifier", loader, LoadingWrapper::ReadTiredness )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::ReadTiredness
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Tiredness::ReadTiredness( xml::xistream& xis )
{
    tirednesses_[ normal_  .GetName() ] = &normal_;
    tirednesses_[ fatigue_ .GetName() ] = &fatigue_;
    tirednesses_[ epuise_  .GetName() ] = &epuise_;

    std::string type;
    xis >> attribute( "state", type );
    T_TirednessMap::iterator it = tirednesses_.find( type );
    if( it == tirednesses_.end() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Undefined tiredness state" );
    const_cast< PHY_Tiredness* >( it->second )->Read( xis );

}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Tiredness::Terminate()
{
    tirednesses_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Tiredness::PHY_Tiredness( const std::string& strName, E_TirednessType nType, ASN1T_EnumUnitTiredness nAsnID, MT_Float rDIAWeight )
    : strName_                       ( strName )
    , nType_                         ( nType   )
    , nAsnID_                        ( nAsnID  )
    , rDIAWeight_                    ( rDIAWeight )
    , rCoefMaxSpeedModificator_      ( 1. )
    , rCoefReloadingTimeModificator_ ( 1. )
    , rCoefPhModificator_            ( 1. )
    , rCoefPostureTimeModificator_   ( 1. )
    , rCoefSensorDistanceModificator_( 1. )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Tiredness::~PHY_Tiredness()
{
}


// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_Tiredness::Read( xml::xistream& xis )
{
    xis >> attribute( "max-speed", rCoefMaxSpeedModificator_ )
        >> attribute( "loading-time", rCoefReloadingTimeModificator_ )
        >> attribute( "ph", rCoefPhModificator_ )
        >> attribute( "posture-setup-time", rCoefPostureTimeModificator_ )
        >> attribute( "sensor-distance", rCoefSensorDistanceModificator_ );

    if( rCoefMaxSpeedModificator_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "max-speed <= 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "loading-time <= 0" );
    if( rCoefPhModificator_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "ph <= 0" );
    if( rCoefPostureTimeModificator_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "posture-setup-time <= 0" );
    if( rCoefSensorDistanceModificator_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "sensor-distance <= 0" );
}
