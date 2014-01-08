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
#include "SupplyConvoyRealFactory.h"
#include "SupplyConvoyVirtualFactory.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "Decision/DEC_Model_ABC.h"
#include "tools/Codec.h"

using namespace logistic;

MT_InterpolatedFunction SupplyConvoyConfig::setupTime_;
MT_InterpolatedFunction SupplyConvoyConfig::loadingTime_;
MT_InterpolatedFunction SupplyConvoyConfig::unloadingTime_;
MT_InterpolatedFunction SupplyConvoyConfig::coefSpeedModificator_;
const MIL_AgentTypePion* SupplyConvoyConfig::convoyAgentType_ = 0;
const MIL_MissionType_ABC* SupplyConvoyConfig::convoyMissionType_ = 0;
const SupplyConvoyFactory_ABC* SupplyConvoyConfig::stockSupplyConvoyFactory_    = &SupplyConvoyRealFactory::Instance();
const SupplyConvoyFactory_ABC* SupplyConvoyConfig::dotationSupplyConvoyFactory_ = &SupplyConvoyVirtualFactory::Instance();

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
    InitializeConvoyType    ( xis );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown type for convoy" );
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
        throw MASA_EXCEPTION( xis.context() + "Invalid mission name for convoy" );
    assert( convoyAgentType_ );
    if( !convoyAgentType_->GetModel().IsMissionAvailable( *convoyMissionType_ ) )
        throw MASA_EXCEPTION( xis.context() + "Convoy type pion cannot receive convoy mission" );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeConvoyType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
const SupplyConvoyFactory_ABC& SupplyConvoyConfig::GetConvoyFactory( xml::xistream& xis, const std::string& type )
{
    if( type == "real" )
        return SupplyConvoyRealFactory::Instance();
    else if( type == "virtual" )
        return SupplyConvoyVirtualFactory::Instance();
    throw MASA_EXCEPTION( xis.context() + "Invalid convoy type" );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeConvoyType
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeConvoyType( xml::xistream& xis )
{
    if( xis.has_child( "type" ) )
    {
        std::string convoyType;
        xis >> xml::start( "type" )
                >> xml::start( "dotation-supply" )
                    >> xml::attribute( "type", convoyType );
        dotationSupplyConvoyFactory_ = &GetConvoyFactory( xis, convoyType );
        xis     >> xml::end
                >> xml::start( "stock-supply" )
                    >> xml::attribute( "type", convoyType );
        stockSupplyConvoyFactory_ = &GetConvoyFactory( xis, convoyType );
        xis     >> xml::end
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::InitializeInterpolatedTime
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::InitializeInterpolatedTime( xml::xistream& xis, const std::string& strTagName, MT_InterpolatedFunction& data )
{
    data.AddNewPoint( 0., 0. );

    std::pair< unsigned int, double > upperBound( 0, 0.f );
    xis >> xml::start( strTagName )
            >> xml::list( "unit-time", boost::bind( &SupplyConvoyConfig::ReadInterpolatedTime, _1, boost::ref( data ), boost::ref( upperBound ) ) )
        >> xml::end;

    data.SetAfterValue( upperBound.second );
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::ReadTime
// Created: ABL 2007-07-24
// -----------------------------------------------------------------------------
void SupplyConvoyConfig::ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction& data, std::pair< unsigned int, double >& upperBound )
{
    unsigned int nNbrCamions;
    double rTime;

    xis >> xml::attribute( "truck-count", nNbrCamions );
    if( nNbrCamions <= 0 )
        throw MASA_EXCEPTION( xis.context() + "time: truck-count <= 0" );

    tools::ReadTimeAttribute( xis, "time", rTime );
    if( rTime < 0 )
        throw MASA_EXCEPTION( xis.context() + "time: time < 0" );
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

    std::pair< unsigned int, double > upperBound( 0, 0.f );

    xis >> xml::start( "speed-modifiers" )
            >> xml::list( "speed-modifier", boost::bind( &SupplyConvoyConfig::ReadSpeedModifier, _1, boost::ref( upperBound ) ) )
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
        throw MASA_EXCEPTION( xis.context() + "speed-modifier: truck-count <= 0" );
    if( rValue <= 0 )
        throw MASA_EXCEPTION( xis.context() + "speed-modifier: value <= 0" );

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

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::GetStockSupplyConvoyFactory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const SupplyConvoyFactory_ABC& SupplyConvoyConfig::GetStockSupplyConvoyFactory()
{
    assert( stockSupplyConvoyFactory_ );
    return *stockSupplyConvoyFactory_;
}

// -----------------------------------------------------------------------------
// Name: SupplyConvoyConfig::GetDotationSupplyConvoyFactory
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
const SupplyConvoyFactory_ABC& SupplyConvoyConfig::GetDotationSupplyConvoyFactory()
{
    assert( dotationSupplyConvoyFactory_ );
    return *dotationSupplyConvoyFactory_;
}