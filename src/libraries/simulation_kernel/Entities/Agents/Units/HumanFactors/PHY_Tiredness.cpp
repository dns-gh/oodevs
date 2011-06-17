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
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"
#include <xeumeuleu/xml.hpp>

PHY_Tiredness::T_TirednessMap PHY_Tiredness::tirednesses_;

const PHY_Tiredness PHY_Tiredness::normal_ ( "Normal" , 0, sword::UnitAttributes::rested  );
const PHY_Tiredness PHY_Tiredness::fatigue_( "Fatigue", 1, sword::UnitAttributes::tired );
const PHY_Tiredness PHY_Tiredness::epuise_ ( "Epuise" , 2, sword::UnitAttributes::exhausted );

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
    tirednesses_[ normal_  .GetName() ] = &normal_;
    tirednesses_[ fatigue_ .GetName() ] = &fatigue_;
    tirednesses_[ epuise_  .GetName() ] = &epuise_;
    LoadingWrapper loader;
    xis >> xml::start( "humans-factors" )
            >> xml::start( "tiredness-factor" )
                >> xml::list( "modifier", loader, &LoadingWrapper::ReadTiredness )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::ReadTiredness
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void PHY_Tiredness::ReadTiredness( xml::xistream& xis )
{
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
PHY_Tiredness::PHY_Tiredness( const std::string& strName, unsigned int nType, sword::UnitAttributes::EnumUnitTiredness nAsnID )
    : PHY_HumanFactor( strName, nType )
    , nAsnID_( nAsnID  )
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
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( sword::UnitAttributes::EnumUnitTiredness nAsnID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< sword::UnitAttributes::EnumUnitTiredness >(), nAsnID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetAsnID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

    return it == tirednesses_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( unsigned int nID )
{
    CIT_TirednessMap it = std::find_if( tirednesses_.begin(), tirednesses_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &PHY_Tiredness::GetID ), std::select2nd< T_TirednessMap::value_type >() ) ) );

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
// Name: PHY_Tiredness::GetAsnID
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
sword::UnitAttributes::EnumUnitTiredness PHY_Tiredness::GetAsnID() const
{
    return nAsnID_;
}
