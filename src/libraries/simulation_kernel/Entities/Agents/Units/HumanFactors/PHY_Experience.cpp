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
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"
#include <xeumeuleu/xml.hpp>

PHY_Experience::T_ExperienceMap PHY_Experience::experiences_;

const PHY_Experience PHY_Experience::veteran_    ( "Veteran"    , 0, sword::UnitAttributes::veteran );
const PHY_Experience PHY_Experience::experimente_( "Experimente", 1, sword::UnitAttributes::expert );
const PHY_Experience PHY_Experience::conscrit_   ( "Conscrit"   , 2, sword::UnitAttributes::novice );

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
    experiences_[ veteran_    .GetName() ] = &veteran_;
    experiences_[ experimente_.GetName() ] = &experimente_;
    experiences_[ conscrit_   .GetName() ] = &conscrit_;
    LoadingWrapper loader;
    xis >> xml::start( "humans-factors" )
            >> xml::start( "experience-factor" )
                >> xml::list( "modifier", loader, &LoadingWrapper::ReadExperience )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::ReadExperience
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Experience::ReadExperience( xml::xistream& xis )
{
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
PHY_Experience::PHY_Experience( const std::string& strName, unsigned int nType, sword::UnitAttributes::EnumUnitExperience nAsnID )
    : PHY_HumanFactor( strName, nType )
    , nAsnID_                        ( nAsnID  )
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
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Experience* PHY_Experience::Find( sword::UnitAttributes::EnumUnitExperience nAsnID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< sword::UnitAttributes::EnumUnitExperience >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Experience::GetAsnID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

    return it == experiences_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Experience::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Experience* PHY_Experience::Find( unsigned int nID )
{
    CIT_ExperienceMap it = std::find_if( experiences_.begin(), experiences_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_Experience::GetID ), std::select2nd< T_ExperienceMap::value_type >() ) ) );

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
// Name: PHY_Experience::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
sword::UnitAttributes::EnumUnitExperience PHY_Experience::GetAsnID() const
{
    return nAsnID_;
}
