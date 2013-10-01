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
#include "MIL_Time_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Protocol.h"

PHY_Tiredness::T_TirednessMap PHY_Tiredness::tirednesses_;
PHY_Tiredness::Evolution PHY_Tiredness::evolution_;

const PHY_Tiredness PHY_Tiredness::normal_ ( "Normal" , 0, sword::UnitAttributes::rested  );
const PHY_Tiredness PHY_Tiredness::fatigue_( "Fatigue", 1, sword::UnitAttributes::tired );
const PHY_Tiredness PHY_Tiredness::epuise_ ( "Epuise" , 2, sword::UnitAttributes::exhausted );

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
    xis >> xml::start( "humans-factors" )
            >> xml::start( "tiredness-factor" )
                >> xml::list( "modifier", &PHY_Tiredness::ReadTiredness )
            >> xml::end
            >> xml::start( "automatic-evolution" )
                >> xml::start( "tiredness-evolution" )
                    >> xml::attribute( "first-threshold",  evolution_.firstThreshold_ )
                    >> xml::attribute( "second-threshold", evolution_.secondThreshold_ )
                    >> xml::attribute( "engine-running",   evolution_.engineRunning_ )
                    >> xml::attribute( "engine-stopped",   evolution_.engineStopped_ )
                    >> xml::attribute( "moving",           evolution_.moving_ )
                    >> xml::attribute( "working",          evolution_.working_ )
                >> xml::end
            >> xml::end
        >> xml::end;
    double timeFactor = MIL_Time_ABC::GetTime().GetTickDuration() / 3600.;
    evolution_.engineRunning_ *= timeFactor;
    evolution_.engineStopped_ *= timeFactor;
    evolution_.moving_ *= timeFactor;
    evolution_.working_ *= timeFactor;
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
        throw MASA_EXCEPTION( xis.context() + "Undefined tiredness state" );
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
    for( auto it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Tiredness::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const PHY_Tiredness* PHY_Tiredness::Find( unsigned int nID )
{
    for( auto it = tirednesses_.begin(); it != tirednesses_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
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
