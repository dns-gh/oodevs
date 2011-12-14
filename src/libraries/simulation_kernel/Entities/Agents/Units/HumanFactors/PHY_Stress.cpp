// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Stress.h"
#include "MIL_AgentServer.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Stl.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

PHY_Stress::T_StressMap PHY_Stress::stresses_;
PHY_Stress::Evolution PHY_Stress::evolution_;

const PHY_Stress PHY_Stress::calm_ ( "Calm", 0 , sword::UnitAttributes::calm  );
const PHY_Stress PHY_Stress::worried_( "Worried", 1, sword::UnitAttributes::worried );
const PHY_Stress PHY_Stress::stressed_ ( "Stressed", 2 , sword::UnitAttributes::stressed );

struct PHY_Stress::LoadingWrapper
{
    void ReadStress( xml::xistream& xis )
    {
        PHY_Stress::ReadStress( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: PHY_Stress::Initialize
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void PHY_Stress::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing stresses" );
    stresses_[ calm_.GetName() ] = &calm_;
    stresses_[ worried_.GetName() ] = &worried_;
    stresses_[ stressed_.GetName() ] = &stressed_;
    LoadingWrapper loader;
    xis >> xml::start( "humans-factors" )
            >> xml::start( "stress-factor" )
                >> xml::list( "modifier", loader, &LoadingWrapper::ReadStress )
            >> xml::end
            >> xml::start( "automatic-evolution" )
                >> xml::start( "stress-evolution" )
                    >> xml::attribute( "first-threshold", evolution_.firstThreshold_ )
                    >> xml::attribute( "second-threshold", evolution_.secondThreshold_ )
                    >> xml::attribute( "inc-per-shot", evolution_.incPerShot_ )
                    >> xml::attribute( "dec-per-hour", evolution_.decPerHour_ )
                >> xml::end
            >> xml::end
        >> xml::end;
    double timeFactor = MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / 3600.;
    evolution_.decPerHour_ *= timeFactor;
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::ReadStress
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void PHY_Stress::ReadStress( xml::xistream& xis )
{
    std::string type;
    xis >> xml::attribute( "state", type );
    T_StressMap::iterator it = stresses_.find( type );
    if( it == stresses_.end() )
        xis.error( "Undefined Stress state" );
    const_cast< PHY_Stress* >( it->second )->Read( xis );
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::Terminate
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
void PHY_Stress::Terminate()
{
    stresses_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress constructor
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
PHY_Stress::PHY_Stress( const std::string& strName, unsigned int nType, sword::UnitAttributes::EnumUnitStress nAsnID )
    : PHY_HumanFactor( strName, nType )
    , nAsnID_( nAsnID  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress destructor
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
PHY_Stress::~PHY_Stress()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::Find
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
const PHY_Stress* PHY_Stress::Find( sword::UnitAttributes::EnumUnitStress nAsnID )
{
    for( CIT_StressMap it = stresses_.begin(); it != stresses_.end(); ++it )
        if( it->second->GetAsnID() == nAsnID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::Find
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
const PHY_Stress* PHY_Stress::Find( unsigned int nID )
{
    for( CIT_StressMap it = stresses_.begin(); it != stresses_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::Find
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
const PHY_Stress* PHY_Stress::Find( const std::string& strName )
{
    CIT_StressMap it = stresses_.find( strName );
    if( it == stresses_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_Stress::GetAsnID
// Created: LDC 2011-06-17
// -----------------------------------------------------------------------------
sword::UnitAttributes::EnumUnitStress PHY_Stress::GetAsnID() const
{
    return nAsnID_;
}


