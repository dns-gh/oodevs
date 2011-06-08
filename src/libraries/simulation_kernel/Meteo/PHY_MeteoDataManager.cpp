// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MeteoDataManager.h"
#include "meteo/PHY_LocalMeteo.h"
#include "meteo/PHY_Lighting.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "tools/Loader_ABC.h"
#include "tools/WorldParameters.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager( MIL_Config& config )
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , pRawData_    ( 0 )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting::Initialize();

    config.GetLoader().LoadFile( config.GetWeatherFile(), boost::bind( &PHY_MeteoDataManager::Initialize, this, _1, boost::ref( config ) ) );
    config.AddFileToCRC( config.GetWeatherFile() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Initialize
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::Initialize( xml::xistream& xis, MIL_Config& config )
{
    xis >> xml::start( "weather" );
    pEphemeride_ = new PHY_Ephemeride( xis );
    InitializeGlobalMeteo( xis );
    tools::WorldParameters terrainConfig = tools::WorldParameters( config );
    pRawData_ = new PHY_RawVisionData( *pGlobalMeteo_, terrainConfig );
    InitializeLocalMeteos( xis );
    xis >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::~PHY_MeteoDataManager()
{
    delete pRawData_;
    weather::PHY_Lighting::Terminate();
    weather::PHY_Precipitation::Terminate();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeGlobalMeteo
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeGlobalMeteo( xml::xistream& xis )
{
    xis >> xml::start( "theater" );
    pGlobalMeteo_ = new PHY_GlobalMeteo( 0, xis, pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
    AddMeteo( *pGlobalMeteo_ );
    xis >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeLocalMeteos
// Created: JVT 03-08-05
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeLocalMeteos( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "local-weather" )
            >> xml::list( "local", *this, &PHY_MeteoDataManager::ReadPatchLocal )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::ReadPatchLocal
// Created: ABL 2007-07-27
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::ReadPatchLocal( xml::xistream& xis )
{
    static unsigned int localCounter = 1;
    AddMeteo( *new PHY_LocalMeteo( localCounter++, xis, pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgMeteo( const sword::MagicAction& msg )
{
    if( msg.type() == sword::MagicAction::global_weather )
    {
        assert( pGlobalMeteo_ );
        pGlobalMeteo_->Update( msg.parameters() );
        client::ControlGlobalWeatherAck reply;
        reply.Send( NET_Publisher_ABC::Publisher() );
    }
    else if( msg.type() == sword::MagicAction::local_weather )
    {
        const sword::MissionParameter& idParameter = msg.parameters().elem( 7 );
        if( idParameter.value_size() != 1 || !idParameter.value().Get( 0 ).has_identifier() )
            throw std::exception( "Local Meteo : bad ID parameter" );
        unsigned int id = idParameter.value().Get( 0 ).identifier();
        bool founded = false;
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            if( ( *it )->GetId() == id )
            {
                static_cast< PHY_LocalMeteo* >( ( *it ).get() )->Update( msg.parameters() );
                founded = true;
                break;
            }
        if( !founded )
            AddMeteo( *new PHY_LocalMeteo( id, msg.parameters(), pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() ) );
        client::ControlLocalWeatherAck replyMsg;
        replyMsg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::RegisterWeatherEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::RegisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
{
    assert( pRawData_ );
    pRawData_->RegisterWeatherEffect( surface, ammoCategory );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UnregisterWeatherEffect
// Created: NLD 2004-10-12
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::UnregisterWeatherEffect( const MT_Ellipse& surface, const PHY_IndirectFireDotationClass& ammoCategory )
{
    assert( pRawData_ );
    pRawData_->UnregisterWeatherEffect( surface, ammoCategory );
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Update
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
void PHY_MeteoDataManager::Update( unsigned int date )
{
    // cycle jour / nuit
    assert( pEphemeride_ );
    if( pEphemeride_->UpdateNight( date ) )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "Ephemeris is now: %s", pEphemeride_->GetLightingBase().GetName().c_str() ) );
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            static_cast< weather::Meteo* >( ( *it ).get() )->Update( pEphemeride_->GetLightingBase() );
    }
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        static_cast< weather::Meteo* >( ( *it ).get() )->UpdateMeteoPatch( date, *pRawData_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::SendStateToNewClient
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::SendStateToNewClient()
{
    for( IT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
      static_cast< weather::Meteo* >( ( *it ).get() )->SendCreation();
}
