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
#include <xeumeuleu/xml.h>

PHY_Tiredness::T_TirednessMap PHY_Tiredness::tirednesses_;

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

    xis >> xml::start( "humans-factors" )
            >> xml::start( "tiredness-factor" )
                >> xml::list( "modifier", loader, &LoadingWrapper::ReadTiredness )
            >> xml::end()
        >> xml::end();
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
    xis >> xml::attribute( "state", type );
    T_TirednessMap::iterator it = tirednesses_.find( type );
    if( it == tirednesses_.end() )
        xis.error( "Undefined tiredness state" );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_Tiredness::~PHY_Tiredness()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Read
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
void PHY_Tiredness::Read( xml::xistream& xis )
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
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( ASN1T_EnumUnitTiredness nAsnID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< ASN1T_EnumUnitTiredness >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetAsnID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

    return it == tirednesses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( uint nID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< uint >(), nID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

    return it == tirednesses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( const std::string& strName )
{
    CIT_TirednessMap it = tirednesses_.find( strName );
    if( it == tirednesses_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetName
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const std::string& PHY_Tiredness::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
ASN1T_EnumUnitTiredness PHY_Tiredness::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefMaxSpeedModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetCoefMaxSpeedModificator() const
{
    return rCoefMaxSpeedModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefReloadingTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetCoefReloadingTimeModificator() const
{
    return rCoefReloadingTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefPhModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetCoefPhModificator() const
{
    return rCoefPhModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefPostureTimeModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetCoefPostureTimeModificator() const
{
    return rCoefPostureTimeModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetCoefSensorDistanceModificator
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetCoefSensorDistanceModificator() const
{
    return rCoefSensorDistanceModificator_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::operator==
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Tiredness::operator==( const PHY_Tiredness& rhs ) const
{
    return nType_ == rhs.nType_;
}
    
// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::operator!=
// Created: NLD 2004-11-29
// -----------------------------------------------------------------------------
bool PHY_Tiredness::operator!=( const PHY_Tiredness& rhs ) const
{
    return nType_ != rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetID
// Created: JVT 2004-11-30
// -----------------------------------------------------------------------------
uint PHY_Tiredness::GetID() const
{
    return (uint)nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::GetWeight
// Created: NLD 2004-12-01
// -----------------------------------------------------------------------------
MT_Float PHY_Tiredness::GetWeight() const
{
    return rDIAWeight_;
}
