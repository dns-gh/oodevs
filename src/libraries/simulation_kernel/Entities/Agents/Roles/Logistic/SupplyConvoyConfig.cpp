// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SupplyConvoyConfig.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Model_ABC.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

using namespace logistic;

MT_InterpolatedFunction< double > SupplyConvoyConfig::setupTime_;
MT_InterpolatedFunction< double > SupplyConvoyConfig::loadingTime_;
MT_InterpolatedFunction< double > SupplyConvoyConfig::unloadingTime_;
MT_InterpolatedFunction< double > SupplyConvoyConfig::coefSpeedModificator_;
const MIL_AgentTypePion* SupplyConvoyConfig::convoyAgentType_ = 0;
const MIL_MissionType_ABC* SupplyConvoyConfig::convoyMissionType_ = 0;

// =============================================================================
// Ugly static initialization
// =============================================================================

struct SupplyConvoyConfig::LoadingWrapper
{
    void ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound )
    {
        SupplyConvoyConfig::ReadInterpolatedTime( xis, data, upperBound );
    }
    void ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound )
    {
        SupplyConvoyConfig::ReadSpeedModifier( xis, upperBound );
    }
};

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::Initialize
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing convoys" );
    xis >> xml::start( "supply" )
            >> xml::start( "convoys" );
    InitializeConvoyUnitType( xis );
    InitializeConvoyMission ( xis );
    InitializeInterpolatedTime ( xis, "constitution-times", setupTime_   );
    InitializeInterpolatedTime ( xis, "loading-times"     , loadingTime_   );
    InitializeInterpolatedTime ( xis, "unloading-times"   , unloadingTime_ );
    InitializeSpeedModificators( xis );
    xis     >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeConvoyUnitType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeConvoyUnitType( xml::xistream& xis )
{
    std::string strConvoyAgentType;
    xis >> xml::attribute( "unit-type", strConvoyAgentType );
    convoyAgentType_ = MIL_AgentTypePion::Find( strConvoyAgentType );
    if( !convoyAgentType_ )
        xis.error( "Unknown type for convoy" );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeConvoyMission( xml::xistream& xis )
{
    std::string strMission;
    xis >> xml::attribute( "mission", strMission );

    convoyMissionType_ = MIL_PionMissionType::Find( strMission );
    if( !convoyMissionType_ )
        xis.error( "Invalid mission name for convoy" );
    assert( convoyAgentType_ );
    if( !convoyAgentType_->GetModel().IsMissionAvailable( *convoyMissionType_ ) )
        xis.error( "Convoy type pion cannot receive convoy mission" );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeInterpolatedTime
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeInterpolatedTime( xml::xistream& xis, const std::string& strTagName, MT_InterpolatedFunction< double >& data )
{
    data.AddNewPoint( 0., 0. );
    LoadingWrapper loader;

    std::pair< unsigned int, double > upperBound( 0, 0.f );
    xis >> xml::start( strTagName )
            >> xml::list( "unit-time", loader, &LoadingWrapper::ReadInterpolatedTime, data, upperBound )
        >> xml::end;

    data.SetAfterValue( upperBound.second );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::ReadTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound )
{
    unsigned int nNbrCamions;
    double rTime;

    xis >> xml::attribute( "truck-count", nNbrCamions );
    if( nNbrCamions <= 0 )
        xis.error( "time: truck-count <= 0" );

    tools::ReadTimeAttribute( xis, "time", rTime );
    if( rTime <= 0 )
        xis.error( "time: time <= 0" );
    rTime = MIL_Tools::ConvertSecondsToSim( rTime );

    data.AddNewPoint( nNbrCamions, rTime );

    if( nNbrCamions >= upperBound.first )
    {
        upperBound.first = nNbrCamions;
        upperBound.second = rTime;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeSpeedModificators
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeSpeedModificators( xml::xistream& xis )
{
    coefSpeedModificator_.AddNewPoint( 0., 0. );
    LoadingWrapper loader;

    std::pair< unsigned int, double > upperBound( 0, 0.f );

    xis >> xml::start( "speed-modifiers" )
            >> xml::list( "speed-modifier", loader, &LoadingWrapper::ReadSpeedModifier, upperBound )
        >> xml::end;

    coefSpeedModificator_.SetAfterValue( upperBound.second );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::ReadSpeedModifier
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound )
{
    unsigned int     nNbrCamions;
    double rValue;

    xis >> xml::attribute( "truck-count", nNbrCamions )
        >> xml::attribute( "value", rValue );

    if( nNbrCamions <= 0 )
        xis.error( "speed-modifier: truck-count <= 0" );
    if( rValue <= 0 )
        xis.error( "speed-modifier: value <= 0" );

    coefSpeedModificator_.AddNewPoint( nNbrCamions, rValue );

    if( nNbrCamions >= upperBound.first )
    {
        upperBound.first = nNbrCamions;
        upperBound.second = rValue;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::Terminate
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::Terminate()
{
    // NOTHING
}