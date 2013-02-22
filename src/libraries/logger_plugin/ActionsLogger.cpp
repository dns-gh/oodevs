// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ActionsLogger.h"

#include "clients_kernel/CoordinateConverter.h"
#include "clients_kernel/Time_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/XmlAdapter.h"
#include "dispatcher/Agent.h"
#include "dispatcher/ModelAdapter.h"
#include "dispatcher/Population.h"
#include "MT_Tools/MT_Logger.h"
#include "protocol/Simulation.h"
#include "protocol/XmlWriters.h"
#include "tools/Exception.h"
#include "tools/Loader_ABC.h"
#include "tools/SessionConfig.h"

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;
using namespace plugins::logger;

typedef sword::ClientToSim_Content Content;

// -----------------------------------------------------------------------------
// Name: ActionsLogger constructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::ActionsLogger( const tools::SessionConfig& config,
                              const dispatcher::Model_ABC& model,
                              const kernel::Time_ABC& timer )
    : config_   ( config )
    , timer_    ( timer )
    , entities_ ( new dispatcher::ModelAdapter( model ) )
    , converter_( new kernel::CoordinateConverter( config ) )
    , loaded_   ( !config_.HasCheckpoint() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger destructor
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
ActionsLogger::~ActionsLogger()
{
    // NOTHING
}

namespace
{
    struct Adapter : public protocol::Writer_ABC
    {
        Adapter( const kernel::CoordinateConverter_ABC& converter )
            : converter_( converter )
        {
            // NOTHING
        }

        std::string Convert( double x, double y ) const
        {
            const auto p2d = geometry::Point2d( x, y );
            const auto p2f = converter_.ConvertFromGeo( p2d );
            return converter_.ConvertToMgrs( p2f );
        }

        const kernel::CoordinateConverter_ABC& converter_;
    };
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::SaveTo
// Created: BAX 2013-02-11
// -----------------------------------------------------------------------------
void ActionsLogger::SaveTo( const std::string& filename, const T_Filter& filter ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "actions" );
    const Adapter adapter( *converter_ );
    for( auto it = actions_.begin(); it != actions_.end(); ++it )
    {
        if( filter && !filter( it->second ) )
            continue;
        xml::xosubstream sub( xos );
        sub << xml::start( "action" )
            << xml::attribute( "time", boost::posix_time::to_iso_string( it->first ) );
        protocol::Write( sub, adapter, it->second.message() );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Commit
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Commit() const
{
    SaveTo( config_.BuildSessionChildFile( "actions.ord" ), T_Filter() );
}

namespace
{
    boost::posix_time::ptime MakeTime( std::string value )
    {
        boost::algorithm::erase_all( value, "-" );
        boost::algorithm::erase_all( value, ":" );
        return boost::posix_time::from_iso_string( value );
    }

    void ReadAction( ActionsLogger::T_Actions& dst, const kernel::XmlAdapter& adapter, xml::xistream& xis )
    {
        try
        {
            std::string time;
            xis >> xml::attribute( "time", time );
            sword::ClientToSim msg;
            msg.set_context( 0 );
            protocol::Read( adapter, *msg.mutable_message(), xis );
            dst.insert( std::make_pair( MakeTime( time ), msg ) );
        }
        catch( const std::exception& err )
        {
            MT_LOG_ERROR_MSG( "Unable to process action: " << tools::GetExceptionMsg( err ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::LoadOrdersIfCheckpoint
// Created: JSR 2010-11-05
// -----------------------------------------------------------------------------
void ActionsLogger::LoadOrdersIfCheckpoint()
{
    if( loaded_ || !config_.HasCheckpoint() )
        return;

    loaded_ = true;
    std::string filename( config_.BuildSessionChildFile( "actions.ord" ) );
    if( !bfs::exists( filename ) )
        return;

    const kernel::XmlAdapter adapter( *converter_, *entities_ );
    const auto xis = config_.GetLoader().LoadFile( filename );
    *xis >> xml::start( "actions" )
         >> xml::list( "action", boost::bind( &ReadAction,
                       boost::ref( actions_ ), boost::cref( adapter ), _1 ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::LogAction
// Created: BAX 2013-02-11
// -----------------------------------------------------------------------------
template< typename T, typename U >
void ActionsLogger::LogAction( const T& message, const U& mutator )
{
    if( !message.has_type() || !message.type().id() )
        return;
    LoadOrdersIfCheckpoint();
    sword::ClientToSim msg;
    *( msg.mutable_message()->*mutator )() = message;
    actions_.insert( std::make_pair( tools::QTimeToBoostTime( timer_.GetDateTime() ), msg ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::UnitOrder& message )
{
    LogAction( message, &Content::mutable_unit_order );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::AutomatOrder& message )
{
    LogAction( message, &Content::mutable_automat_order );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::CrowdOrder& message )
{
    LogAction( message, &Content::mutable_crowd_order );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Log
// Created: SBO 2010-05-11
// -----------------------------------------------------------------------------
void ActionsLogger::Log( const sword::FragOrder& message )
{
    LogAction( message, &Content::mutable_frag_order );
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::Close
// Created: LDC 2013-02-21
// -----------------------------------------------------------------------------
void ActionsLogger::Close()
{
    try
    {
        Commit();
    }
    catch( ... )
    {
    }
}

namespace
{
    bool ActiveFilter( const kernel::EntityResolver_ABC& resolver,
                       const sword::ClientToSim& pkt )
    {
        if( !pkt.has_message() )
            return false;
        const auto& content = pkt.message();
        if( content.has_unit_order() )
            if( auto ptr = resolver.FindAgent( content.unit_order().tasker().id() ) )
                return !!static_cast< dispatcher::Agent* >( ptr )->GetOrder();
        if( content.has_crowd_order() )
            if( auto ptr = resolver.FindPopulation( content.crowd_order().tasker().id() ) )
                return !!static_cast< dispatcher::Population* >( ptr )->GetOrder();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsLogger::SaveCheckpointActiveMissions
// Created: NPT 2012-09-12
// -----------------------------------------------------------------------------
void ActionsLogger::SaveCheckpointActiveMissions( std::string name )
{
    const auto filename = config_.BuildOnLocalCheckpointChildFile( name, "current.ord" );
    SaveTo( filename, boost::bind( &ActiveFilter, boost::cref( *entities_ ), _1 ) );
    Commit();
}
