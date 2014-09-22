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
#include "Meteo/PHY_Ephemeride.h"
#include "Meteo/PHY_GlobalMeteo.h"
#include "Meteo/PHY_LocalMeteo.h"
#include "Meteo/PHY_Precipitation.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_Config.h"
#include "Tools/MIL_MessageParameters.h"

#include "MT_Tools/MT_Logger.h"

#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter.h"
#include "meteo/PHY_Lighting.h"
#include "protocol/ClientSenders.h"
#include "protocol/EnumMaps.h"
#include "protocol/MessageParameters.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace bpt = boost::posix_time;

unsigned int PHY_MeteoDataManager::localCounter_ = 1;

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MeteoDataManager )

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager()
    : pRawData_    ( 0 )
    , tickDuration_( 1 )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager constructor
// Created: JVT 02-10-21
//-----------------------------------------------------------------------------
PHY_MeteoDataManager::PHY_MeteoDataManager(
    const boost::shared_ptr< TER_World >& world, xml::xistream& xis,
    const tools::Path& detectionFile, uint32_t now, uint32_t tickDuration )
    : world_( world )
    , pRawData_    ( 0 )
    , tickDuration_( tickDuration )
{
    xis >> xml::start( "weather" );
    pEphemeride_ = ReadEphemeride( xis, now );
    InitializeGlobalMeteo( xis );
    pRawData_ = new PHY_RawVisionData( *pGlobalMeteo_, detectionFile );
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
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::InitializeGlobalMeteo
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::InitializeGlobalMeteo( xml::xistream& xis )
{
    xis >> xml::start( "theater" );
    pGlobalMeteo_ = boost::make_shared< PHY_GlobalMeteo >(
            xis, pEphemeride_->GetLightingBase(), tickDuration_ );
    pGlobalMeteo_->SetLighting( pEphemeride_->GetLightingBase() );
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
            >> xml::list( "local", [&]( xml::xistream& xis )
            {
                InternalAddLocalWeather( xis );
            })
        >> xml::end;
}

boost::shared_ptr< const weather::Meteo > PHY_MeteoDataManager::InternalAddLocalWeather(
        xml::xistream& xis )
{
    const auto w = boost::make_shared< PHY_LocalMeteo >( localCounter_++, xis,
        pEphemeride_->GetLightingBase(), tickDuration_, world_ );
    AddMeteo( w );
    return w;
}

boost::shared_ptr< const weather::Meteo > PHY_MeteoDataManager::AddLocalWeather(
        xml::xistream& xis )
{
    xis >> xml::start( "local" );
    auto w = InternalAddLocalWeather( xis );
    xis >> xml::end;
    return w;
}

bool PHY_MeteoDataManager::RemoveLocalWeather( uint32_t id )
{
    const auto it = meteos_.find( id );
    if( it == meteos_.end() )
        return false;
    removed_.insert( id );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::UpdateGlobalWeather
// Created: LGY 2013-06-16
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::UpdateGlobalWeather( const sword::MagicAction& msg )
{
    pGlobalMeteo_->Update( msg.parameters() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::ManageLocalWeather
// Created: LGY 2013-06-16
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::ManageLocalWeather( const sword::MagicAction& msg, sword::MagicActionAck& ack )
{
    const auto& params = msg.parameters();
    uint32_t id = 0;
    protocol::CheckCount( params, 10, 11 );
    if( !protocol::IsNull( params, 10 ) )
        id = protocol::GetIdentifier( params, 10 );
    if( id == 0 )
    {
        auto meteo = boost::make_shared< PHY_LocalMeteo >( localCounter_++,
                params, pEphemeride_->GetLightingBase(), tickDuration_, world_ );
        id = meteo->GetId();
        AddMeteo( meteo );
    }
    else
    {
        const auto it = meteos_.find( id );
        if( it == meteos_.end() )
            throw MASA_BADPARAM_MAGICACTION( "unknown local weather id" );
        it->second->Update( params );
    }
    if( id )
    {
        ack.mutable_result()->add_elem()->add_value()->set_identifier( id );
        // Published in 5.2, deprecated
        ack.mutable_weather()->set_id( id );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::RemoveLocalWeather
// Created: LGY 2013-06-16
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::RemoveLocalWeather( const sword::MagicAction& msg )
{
    const auto& params = msg.parameters();
    protocol::CheckCount( params, 1 );
    const uint32_t id = protocol::GetIdentifier( params, 0 );
    if( !RemoveLocalWeather( id ) )
        throw MASA_BADPARAM_MAGICACTION( "parameters[0] must be a local weather identifier" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::OnReceiveMsgGlobalMeteo
// Created: NLD 2003-08-04
// Last modified: JVT 03-08-05
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::OnReceiveMsgMeteo( const sword::MagicAction& msg, sword::MagicActionAck& ack, unsigned int context )
{
    auto& pub = NET_Publisher_ABC::Publisher();
    if( msg.type() == sword::MagicAction::global_weather )
    {
        UpdateGlobalWeather( msg );
        client::ControlGlobalWeatherAck().Send( pub, context ); ///< deprecated
    }
    else if( msg.type() == sword::MagicAction::local_weather )
    {
        ManageLocalWeather( msg, ack );
        client::ControlLocalWeatherAck().Send( pub, context ); ///< deprecated
    }
    else if( msg.type() == sword::MagicAction::local_weather_destruction )
    {
        RemoveLocalWeather( msg );
        client::ControlLocalWeatherAck().Send( pub, context ); ///< deprecated
    }
    else
        throw MASA_EXCEPTION( "unknown magic action" );
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::load
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    std::size_t size = 0;
    file >> localCounter_;
    file >> pGlobalMeteo_;
    file >> pEphemeride_;
    file >> const_cast< uint32_t& >( tickDuration_ );
    file >> size;
    MIL_Config& config = MIL_AgentServer::GetWorkspace().GetConfig();
    pRawData_ = new PHY_RawVisionData( *pGlobalMeteo_, config.GetDetectionFile() );
    PHY_LocalMeteo* meteo = 0;
    for( ; size > 0; --size )
    {
        file >> meteo;
        meteos_[ meteo->GetId() ] = boost::shared_ptr< PHY_LocalMeteo >( meteo );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::save
// Created: JSR 2011-11-22
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    std::size_t size = meteos_.size();
    file << localCounter_;
    file << pGlobalMeteo_;
    file << pEphemeride_;
    file << tickDuration_;
    file << size;
    for( auto it = meteos_.begin(); it != meteos_.end(); ++it )
    {
        auto local = it->second.get();
        file << local;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::WriteWeather
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::WriteWeather( xml::xostream& xos, uint32_t now ) const
{
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "weather" );
    schemaWriter.WriteSchema( xos, "exercise", "weather" );
    xos << xml::start( "exercise-date" )
            << xml::attribute( "value", bpt::to_iso_string( bpt::from_time_t( now ) ) )
        << xml::end
        << xml::start( "ephemerides" );
            pEphemeride_->WriteUrban( xos );
    xos << xml::end
            << xml::start( "theater" );
                pGlobalMeteo_->Serialize( xos );
    xos     << xml::end
            << xml::start( "local-weather" );
    for( auto it = meteos_.begin(); it != meteos_.end() ; it++ )
    {
        xos     << xml::start( "local" );
                    it->second->Serialize( xos );
        xos     << xml::end;
    }
    xos     << xml::end
        << xml::end;
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
        MT_LOG_INFO_MSG( "Ephemeris is now: " << pEphemeride_->GetLightingBase().GetName() );
        pGlobalMeteo_->SetLighting( pEphemeride_->GetLightingBase() );
        for( auto it = meteos_.begin(); it != meteos_.end(); ++it )
            it->second->SetLighting( pEphemeride_->GetLightingBase() );
    }

    // Note weather can be disabled without being removed
    auto disabled = removed_;

    const int now = boost::numeric_cast< int >( date );
    pGlobalMeteo_->SendCreationIfModified();
    for( auto it = meteos_.begin(); it != meteos_.end(); ++it )
    {
        const auto& w = it->second;
        const auto patched = pRawData_->IsWeatherPatched( w );
        const bool active = w->GetStartTime() < now && now < w->GetEndTime()
            && removed_.count( w->GetId() ) == 0;
        if( active != patched )
        {
            if( active )
            {
                pRawData_->RegisterMeteoPatch( w );
            }
            else
            {
                pRawData_->UnregisterMeteoPatch( w );
                disabled.insert( w->GetId() );
            }
        }
        if( active )
            w->SendCreationIfModified();
    }

    for( auto it = disabled.begin(); it != disabled.end(); ++it )
        meteos_.at( *it )->SendDestruction();
    for( auto it = removed_.begin(); it != removed_.end(); ++it )
        meteos_.erase( *it );
    removed_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::SendStateToNewClient
// Created: HBD 2010-03-26
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::SendStateToNewClient()
{
    pGlobalMeteo_->SendCreation();
    for( auto it = meteos_.begin(); it != meteos_.end(); ++it )
        it->second->SendCreation();
}

boost::shared_ptr< const weather::Meteo > PHY_MeteoDataManager::GetGlobalWeather() const
{
    return pGlobalMeteo_;
}

//-----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::GetRawVisionData
// Created: JVT 02-11-20
//-----------------------------------------------------------------------------
const PHY_RawVisionData& PHY_MeteoDataManager::GetRawVisionData() const
{
    assert( pRawData_ );
    return *pRawData_;
}

//-----------------------------------------------------------------------------
// Name: Meteo::sEphemeride
// Created: JVT 03-08-07
//-----------------------------------------------------------------------------
const PHY_Ephemeride& PHY_MeteoDataManager::GetEphemeride() const
{
    assert( pEphemeride_ );
    return *pEphemeride_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MeteoDataManager::AddMeteo
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
void PHY_MeteoDataManager::AddMeteo( const boost::shared_ptr< PHY_LocalMeteo >& meteo )
{
    meteo->SetLighting( pEphemeride_->GetLightingBase() );
    meteos_[ meteo->GetId() ] = meteo;
}
