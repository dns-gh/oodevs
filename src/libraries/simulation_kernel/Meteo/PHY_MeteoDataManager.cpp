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
#include "MIL_AgentServer.h"
#include "meteo/PHY_LocalMeteo.h"
#include "meteo/PHY_Lighting.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Tools.h"
#include "tools/Loader_ABC.h"
#include "tools/WorldParameters.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

unsigned int PHY_MeteoDataManager::localCounter_ = 1;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MeteoDataManager )

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager()
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , pRawData_    ( 0 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager( MIL_Config& config )
    : pEphemeride_ ( 0 )
    , pGlobalMeteo_( 0 )
    , pRawData_    ( 0 )
{
    config.GetLoader().LoadFile( config.GetWeatherFile(), boost::bind( &PHY_MeteoDataManager::Load, this, _1, boost::ref( config ) ) );
    config.AddFileToCRC( config.GetWeatherFile() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Initialize
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::Initialize()
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting::Initialize();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::Load
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::Load( xml::xistream& xis, MIL_Config& config )
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
    delete pGlobalMeteo_;
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
    pGlobalMeteo_ = new PHY_GlobalMeteo( xis, pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
    pGlobalMeteo_->Update( pEphemeride_->GetLightingBase() );
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
    AddMeteo( *new PHY_LocalMeteo( localCounter_++, xis, pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgMeteo( const sword::MagicAction& msg, unsigned int context )
{
    if( msg.type() == sword::MagicAction::global_weather )
    {
        assert( pGlobalMeteo_ );
        pGlobalMeteo_->Update( msg.parameters() );
        client::ControlGlobalWeatherAck reply;
        reply.Send( NET_Publisher_ABC::Publisher(), context );
    }
    else if( msg.type() == sword::MagicAction::local_weather )
    {
        unsigned int id = 0;
        if( msg.parameters().elem_size() == 11 )
        {
            const sword::MissionParameter& idParameter = msg.parameters().elem( 10 );
            if( idParameter.value_size() != 1 || !idParameter.value().Get( 0 ).has_identifier() )
                throw std::exception( "Local meteo: bad ID parameter." );

            id = idParameter.value().Get( 0 ).identifier();
        }
        if( id == 0 )
            AddMeteo( *new PHY_LocalMeteo( localCounter_++, msg.parameters(), pEphemeride_->GetLightingBase(), MIL_AgentServer::GetWorkspace().GetTimeStepDuration() ) );
        else
        {
            weather::Meteo* meteo = Find( id );
            if( !meteo )
                throw std::exception( "Local meteo: unknown id." );
            static_cast< PHY_LocalMeteo* >( meteo )->Update( msg.parameters() );
        }
        client::ControlLocalWeatherAck replyMsg;
        replyMsg.Send( NET_Publisher_ABC::Publisher(), context );
    }
    else if( msg.type() == sword::MagicAction::local_weather_destruction )
    {
        if( msg.parameters().elem_size() != 1 || msg.parameters().elem( 0 ).value_size() != 1 || !msg.parameters().elem( 0 ).value().Get( 0 ).has_identifier() )
            throw std::exception( "Local meteo destruction: bad ID parameter." );
        unsigned int id = msg.parameters().elem( 0 ).value().Get( 0 ).identifier();

        weather::Meteo* meteo = Find( id );
        if( !meteo )
            throw std::exception( "Local meteo: unknown id." );
        {
            client::ControlLocalWeatherAck replyMsg;
            replyMsg.Send( NET_Publisher_ABC::Publisher(), context );
        }
        meteo->SendDestruction();
        Remove( id );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::load
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t size = 0;
    file >> boost::serialization::base_object< weather::MeteoManager_ABC >( *this )
         >> localCounter_
         >> pGlobalMeteo_
         >> pEphemeride_
         >> size;
    tools::WorldParameters terrainConfig = tools::WorldParameters( MIL_AgentServer::GetWorkspace().GetConfig() );
    pRawData_ = new PHY_RawVisionData( *pGlobalMeteo_, terrainConfig );
    PHY_LocalMeteo* meteo = 0;
    for( ; size > 0; --size )
    {
        file >> meteo;
        meteos_.insert( boost::shared_ptr< weather::Meteo >( meteo ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::save
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( meteos_.size() > 0 );
    std::size_t size = meteos_.size() - 1;
    file << boost::serialization::base_object< weather::MeteoManager_ABC >( *this )
         << localCounter_
         << pGlobalMeteo_
         << pEphemeride_
         << size;
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
    {
        PHY_LocalMeteo* local = static_cast< PHY_LocalMeteo* >( it->get() );
        file << local;
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
        pGlobalMeteo_->Update( pEphemeride_->GetLightingBase() );
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            ( *it )->Update( pEphemeride_->GetLightingBase() );
    }
    pGlobalMeteo_->UpdateMeteoPatch( date, *pRawData_, boost::shared_ptr< weather::Meteo >() );
    for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
        ( *it )->UpdateMeteoPatch( date, *pRawData_, *it );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::SendStateToNewClient
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::SendStateToNewClient()
{
    pGlobalMeteo_->SendCreation();
    for( IT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
      ( *it )->SendCreation();
}
