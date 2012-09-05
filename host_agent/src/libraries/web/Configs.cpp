// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Configs.h"
#include "Plugins.h"
#include "Request_ABC.h"
#include "runtime/PropertyTree.h"
#include "runtime/Utf8.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>

using namespace web;
using namespace property_tree;
using runtime::Utf8Convert;
using session::RngDistribution;

// -----------------------------------------------------------------------------
// Name: RngConfig::RngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
session::RngConfig::RngConfig()
    : distribution( RNG_DISTRIBUTION_LINEAR )
    , deviation   ( 0.5 )
    , mean        ( 0.5 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::PluginConfig
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
session::PluginConfig::PluginConfig( const Plugins& plugins, const Path& path )
    : enabled   ( false )
    , library   ( plugins.GetLibrary( path ) )
    , parameters( plugins.GetDefaults( path ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::Config
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
session::Config::Config()
{
    checkpoints.enabled = true;
    checkpoints.frequency = 3600;
    checkpoints.keep = 1;
    pathfind.threads = 1;
    recorder.frequency = 200;
    time.end_tick = 0;
    time.factor = 10;
    time.paused = false;
    time.step = 10;
    rng.seed = 0;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: ConvertDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
RngDistribution ConvertRngDistribution( const std::string& src )
{
    if( src == "linear"   ) return session::RNG_DISTRIBUTION_LINEAR;
    if( src == "gaussian" ) return session::RNG_DISTRIBUTION_GAUSSIAN;
    return session::RNG_DISTRIBUTION_LINEAR;
}

// -----------------------------------------------------------------------------
// Name: ConvertDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
std::string ConvertRngDistribution( RngDistribution src )
{
    if( src == session::RNG_DISTRIBUTION_LINEAR   ) return "linear";
    if( src == session::RNG_DISTRIBUTION_GAUSSIAN ) return "gaussian";
    return "invalid";
}

// -----------------------------------------------------------------------------
// Name: TryRead
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
template< typename T >
bool TryRead( T& dst, const Request_ABC& request, const std::string& key )
{
    const boost::optional< std::string > opt = request.GetParameter( key );
    if( opt == boost::none )
        return false;
    dst = boost::lexical_cast< T >( *opt );
    return true;
}

// -----------------------------------------------------------------------------
// Name: TryRead
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
template< typename T >
bool TryReadSet( T& dst, const Tree& src, const std::string& key )
{
    Tree::const_assoc_iterator sub = src.find( key );
    if( sub == src.not_found() )
        return false;
    T next;
    for( Tree::const_iterator it = sub->second.begin(); it != sub->second.end(); ++it )
        next.insert( it->second.get_value( std::string() ) );
    if( next == dst )
        return false;
    dst = next;
    return true;
}
}

namespace
{
// -----------------------------------------------------------------------------
// Name: ReadRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool ReadRngConfig( session::RngConfig& dst, const Tree& src, const std::string& prefix )
{
    std::string dist = ConvertRngDistribution( dst.distribution );
    bool modified = ::TryRead( dist, src, prefix + "distribution" );
    if( modified )
        dst.distribution = ConvertRngDistribution( dist );
    modified |= ::TryRead( dst.deviation, src, prefix + "deviation" );
    modified |= ::TryRead( dst.mean, src, prefix + "mean" );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void WriteRngConfig( Tree& dst, const std::string& prefix, const session::RngConfig& cfg )
{
    dst.put( prefix + "distribution", ConvertRngDistribution( cfg.distribution ) );
    dst.put( prefix + "deviation", cfg.deviation );
    dst.put( prefix + "mean", cfg.mean );
}

// -----------------------------------------------------------------------------
// Name: XpathToJson
// Created: BAX 2012-08-29
// -----------------------------------------------------------------------------
std::string XpathToJson( std::string value )
{
    boost::algorithm::replace_all( value, "/", "." );
    boost::algorithm::replace_all( value, "@", "" );
    return value;
}

// -----------------------------------------------------------------------------
// Name: TryReadPluginParameters
// Created: BAX 2012-08-29
// -----------------------------------------------------------------------------
template< typename T >
bool TryReadPluginParameters( T& dst, const Tree& src, const std::string& prefix )
{
    bool modified = false;
    BOOST_FOREACH( typename T::value_type& value, dst )
        modified |= property_tree::TryRead( value.second, src, prefix + XpathToJson( value.first ) );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: ReadPluginConfig
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
bool ReadPluginConfig( session::PluginConfig& dst, const Tree& src, const std::string& prefix )
{
    bool modified = ::TryRead( dst.enabled, src, prefix + "enabled" );
    modified |= TryReadPluginParameters( dst.parameters, src, prefix );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: ReadPlugins
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
bool ReadPlugins( session::Config::T_Plugins& dst, const Plugins& plugins, const Tree& src )
{
    bool modified = false;
    BOOST_FOREACH( const Path& path, plugins.GetNames( 0, INT_MAX ) )
    {
        const std::string name = Utf8Convert( path );
        session::Config::T_Plugins::iterator it = dst.find( name );
        if( it == dst.end() )
            it = dst.insert( std::make_pair( name, session::PluginConfig( plugins, path ) ) ).first;
        modified |= ReadPluginConfig( it->second, src, "plugins." + name + "." );
    }
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WritePluginConfig
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
void WritePluginConfig( Tree& dst, const std::string& prefix, const session::PluginConfig cfg )
{
    dst.put( prefix + "enabled", cfg.enabled );
    BOOST_FOREACH( const session::PluginConfig::T_Parameters::value_type& value, cfg.parameters )
        dst.put( prefix + XpathToJson( value.first ), value.second );
}
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool web::session::ReadConfig( session::Config& dst, const Plugins& plugins, const Tree& src )
{
    bool modified = false;
    modified |= TryRead( dst.name, src, "name" );
    modified |= TryRead( dst.checkpoints.enabled, src, "checkpoints.enabled" );
    modified |= TryRead( dst.checkpoints.frequency, src, "checkpoints.frequency" );
    modified |= TryRead( dst.checkpoints.keep, src, "checkpoints.keep" );
    modified |= TryRead( dst.time.end_tick, src, "time.end_tick" );
    modified |= TryRead( dst.time.factor, src, "time.factor" );
    modified |= TryRead( dst.time.paused, src, "time.paused" );
    modified |= TryRead( dst.time.step, src, "time.step" );
    modified |= TryRead( dst.pathfind.threads, src, "pathfind.threads" );
    modified |= TryRead( dst.recorder.frequency, src, "recorder.frequency" );
    modified |= TryRead( dst.rng.seed, src, "rng.seed" );
    modified |= ReadRngConfig( dst.rng.breakdown, src, "rng.breakdown." );
    modified |= ReadRngConfig( dst.rng.fire, src, "rng.fire." );
    modified |= ReadRngConfig( dst.rng.perception, src, "rng.perception." );
    modified |= ReadRngConfig( dst.rng.wound, src, "rng.wound." );
    modified |= ReadPlugins( dst.plugins, plugins, src );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void web::session::WriteConfig( Tree& dst, const session::Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "checkpoints.enabled", cfg.checkpoints.enabled );
    dst.put( "checkpoints.frequency", cfg.checkpoints.frequency );
    dst.put( "checkpoints.keep", cfg.checkpoints.keep );
    dst.put( "time.end_tick", cfg.time.end_tick );
    dst.put( "time.factor", cfg.time.factor );
    dst.put( "time.paused", cfg.time.paused );
    dst.put( "time.step", cfg.time.step );
    dst.put( "pathfind.threads", cfg.pathfind.threads );
    dst.put( "recorder.frequency", cfg.recorder.frequency );
    dst.put( "rng.seed", cfg.rng.seed );
    WriteRngConfig( dst, "rng.breakdown.", cfg.rng.breakdown );
    WriteRngConfig( dst, "rng.fire.", cfg.rng.fire );
    WriteRngConfig( dst, "rng.perception.", cfg.rng.perception );
    WriteRngConfig( dst, "rng.wound.", cfg.rng.wound );
    BOOST_FOREACH( const session::Config::T_Plugins::value_type& value, cfg.plugins )
        WritePluginConfig( dst, "plugins." + value.first + ".", value.second );
}

// -----------------------------------------------------------------------------
// Name: Config::Config
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
node::Config::Config()
{
    sessions.max_play = 0;
    sessions.max_parallel = 0;
    sessions.reset = true;
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
bool web::node::ReadConfig( node::Config& dst, const Tree& src )
{
    bool modified = false;
    modified |= TryRead( dst.name, src, "name" );
    modified |= TryRead( dst.sessions.max_play, src, "sessions.max_play" );
    modified |= TryRead( dst.sessions.max_parallel, src, "sessions.max_parallel" );
    modified |= TryRead( dst.sessions.reset, src, "sessions.reset" );
    modified |= TryReadSet( dst.plugins, src, "plugins" );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteConfig
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
void web::node::WriteConfig( Tree& dst, const node::Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "sessions.max_play", cfg.sessions.max_play );
    dst.put( "sessions.max_parallel", cfg.sessions.max_parallel );
    dst.put( "sessions.reset", cfg.sessions.reset );
    PutList( dst, "plugins", cfg.plugins );
}
