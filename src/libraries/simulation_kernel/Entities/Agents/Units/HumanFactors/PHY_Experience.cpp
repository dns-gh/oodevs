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
#include <xeumeuleu/xml.h>

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
    xis >> xml::start( "humans-factors" )
            >> xml::start( "experience-factor" )
                >> xml::list( "modifier", loader, &LoadingWrapper::ReadExperience )
            >> xml::end()
        >> xml::end();
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
    xis >> xml::attribute( "state", type );
    T_ExperienceMap::iterator it = experiences_.find( type );
    if( it == experiences_.end() )
        xis.error( "Undefined experience state" );
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
    xis >> xml::attribute( "max-speed", rCoefMaxSpeedModificator_ )
        >> xml::attribute( "loading-time", rCoefReloadingTimeModificator_ )
        >> xml::attribute( "ph", rCoefPhModificator_ )
        >> xml::attribute( "posture-setup-time", rCoefPostureTimeModificator_ )
        >> xml::attribute( "sensor-distance", rCoefSensorDistanceModificator_ );

    if( rCoefMaxSpeedModificator_ <= 0 )
        xis.error( "max-speed <= 0" );
    if( rCoefReloadingTimeModificator_ <= 0 )
        xis.error( "loading-time <= 0" );
    if( rCoefPhModificator_ <= 0 )
        xis.error( "ph <= 0" );
    if( rCoefPostureTimeModificator_ <= 0 )
        xis.error( "posture-setup-time <= 0" );
    if( rCoefSensorDistanceModificator_ <= 0 )
        xis.error( "sensor-distance <= 0" );
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Experience* PHY_Experience::Find( ASN1T_EnumUnitExperience nAsnID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumUnitExperience >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Experience::GetAsnID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

    return it == experiences_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Experience* PHY_Experience::Find( uint nID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Experience::GetID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

    return it == experiences_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
const PHY_Experience* PHY_Experience::Find( const std::string& strName )
{
    CIT_ExperienceMap it = experiences_.find( strName );
    if( it == experiences_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Experience::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
ASN1T_EnumUnitExperience PHY_Experience::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Experience::operator==( const PHY_Experience& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Experience::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Experience::operator!=( const PHY_Experience& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
uint PHY_Experience::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::GetWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
MT_Float PHY_Experience::GetWeight() const
{
    return rDIAWeight_;
}
