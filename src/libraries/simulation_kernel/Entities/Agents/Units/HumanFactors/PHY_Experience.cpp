// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/HumanFactors/PHY_Experience.cpp $
// $Author: Nld $
// $Modtime: 1/12/04 10:46 $
// $Revision: 2 $
// $Workfile: PHY_Experience.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_Experience.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

PHY_Experience::T_ExperienceMap PHY_Experience::experiences_;

const PHY_Experience PHY_Experience::conscrit_   ( "Conscrit"   , eConscrit   , EnumUnitExperience::conscrit   , 1.);
const PHY_Experience PHY_Experience::experimente_( "Experimente", eExperimente, EnumUnitExperience::experimente, 1.);
const PHY_Experience PHY_Experience::veteran_    ( "Veteran"    , eVeteran    , EnumUnitExperience::veteran    , 1. );

struct PHY_Experience::LoadingWrapper
{
    void ReadExperience( xml::xistream& xis )
    {
        PHY_Experience::ReadExperience( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Experience::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing experiences" );
    
    LoadingWrapper loader;
    xis >> start( "humans-factors" )
            >> start( "experience-factor" )
                >> list( "modifier", loader, &LoadingWrapper::ReadExperience )
            >> end()
        >> end();
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::ReadExperience
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Experience::ReadExperience( xml::xistream& xis )
{
    experiences_[ conscrit_    .GetName() ] = &conscrit_;
    experiences_[ experimente_ .GetName() ] = &experimente_;
    experiences_[ veteran_     .GetName() ] = &veteran_;

    std::string type;
    xis >> attribute( "state", type );
    T_ExperienceMap::iterator it = experiences_.find( type );
    if( it == experiences_.end() )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Undefined experience state" );
    const_cast< PHY_Experience* >( it->second )->Read( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void PHY_Experience::Terminate()
{
    experiences_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Experience constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Experience::PHY_Experience( const std::string& strName, E_ExperienceType nType, ASN1T_EnumUnitExperience nAsnID, MT_Float rDIAWeight )
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Experience::~PHY_Experience()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_Experience::Read( xml::xistream& xis )
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
