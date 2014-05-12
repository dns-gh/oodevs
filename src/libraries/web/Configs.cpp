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

using namespace web;
using namespace property_tree;
using runtime::Utf8;
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
// Name: Sides::Sides
// Created: NPT 2013-04-10
// -----------------------------------------------------------------------------
session::Side::Side( const std::string& name, bool created )
    : name( name )
    , created( created )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sides::Sides
// Created: NPT 2013-04-10
// -----------------------------------------------------------------------------
session::Side::Side()
    : created( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiles::Profiles
// Created: BAX 2013-04-22
// -----------------------------------------------------------------------------
session::Profile::Profile( const std::string& username, const std::string& password )
    : username( username )
    , password( password )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Profiles::operator<
// Created: BAX 2013-04-22
// -----------------------------------------------------------------------------
bool session::Profile::operator<( const Profile& other ) const
{
    return username < other.username;
}

// -----------------------------------------------------------------------------
// Name: Profiles::operator==
// Created: BAX 2013-04-22
// -----------------------------------------------------------------------------
bool session::Profile::operator==( const Profile& other ) const
{
    return username == other.username
        && password == other.password;
}

session::User::User( int id )
    : id( id )
{
    // NOTHING
}

session::User::User( int id, const std::string& name )
    : id  ( id )
    , name( name )
{
    // NOTHING
}

bool session::User::operator<( const User& other ) const
{
    return id < other.id;
}

bool session::User::operator==( const User& other ) const
{
    return id == other.id &&
        name == other.name &&
        profiles == other.profiles;
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
    logs.level = LOG_LEVEL_ALL;
    logs.rotate = true;
    logs.max_files = 5;
    logs.max_size = 10;
    logs.size_unit = "mbytes";
    pathfind.threads = 1;
    recorder.frequency = 200;
    time.end_tick = 0;
    time.factor = 10;
    time.paused = false;
    time.step = 10;
    rng.seed = 0;
    reports.clean_frequency = 100;
    sides.no_side_objects = true;
    mapnik.enabled = false;
    restricted.enabled = false;
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

template< typename T, typename U >
void Iterate( const T& src, const std::string& key, const U& read )
{
    if( const auto tree = src.get_child_optional( key ) )
        for( auto it = tree->begin(); it != tree->end(); ++it )
            read( it->first, it->second );
}

// -----------------------------------------------------------------------------
// Name: TryRead
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
template< typename T >
bool TryReadSet( T& dst, const Tree& src, const std::string& key )
{
    auto sub = src.find( key );
    if( sub == src.not_found() )
        return false;
    T next;
    for( auto it = sub->second.begin(); it != sub->second.end(); ++it )
        next.insert( it->second.get_value( std::string() ) );
    if( next == dst )
        return false;
    dst = next;
    return true;
}

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

session::LogLevel ConvertLogLevel( const std::string& src )
{
    if( src == "error" ) return session::LOG_LEVEL_ERROR;
    if( src == "info" ) return session::LOG_LEVEL_INFO;
    return session::LOG_LEVEL_ALL;
}

std::string ConvertLogLevel( session::LogLevel src )
{
    if( src == session::LOG_LEVEL_ERROR ) return "error";
    if( src == session::LOG_LEVEL_INFO ) return "info";
    return "all";
}

bool ReadLogConfig( session::Config& dst, const Tree& src, const std::string& prefix )
{
    std::string level = ConvertLogLevel( dst.logs.level );
    bool modified = ::TryRead( level, src, prefix + "level" );
    dst.logs.level = ConvertLogLevel( level );
    modified |= ::TryRead( dst.logs.rotate, src, prefix + "rotate" );
    modified |= ::TryRead( dst.logs.max_files, src, prefix + "max_files" );
    modified |= ::TryRead( dst.logs.max_size, src, prefix + "max_size" );
    modified |= ::TryRead( dst.logs.size_unit, src, prefix + "size_unit" );
    return modified;
}

void WriteLogConfig( Tree& dst, const std::string& prefix, const session::Config& cfg )
{
    dst.put( prefix + "level", ConvertLogLevel( cfg.logs.level ) );
    dst.put( prefix + "rotate", cfg.logs.rotate );
    dst.put( prefix + "max_files", cfg.logs.max_files );
    dst.put( prefix + "max_size", cfg.logs.max_size );
    dst.put( prefix + "size_unit", cfg.logs.size_unit );
}

// -----------------------------------------------------------------------------
// Name: ReadSideConfig
// Created: NPT 2013-04-08
// -----------------------------------------------------------------------------
bool ReadSideConfig( session::Config::T_Sides& dst, const Tree& src, const std::string& prefix )
{
    bool modified = false;
    const auto tree = src.get_child_optional( prefix );
    if( !tree )
        return false;
    for( auto it = tree->begin(); it != tree->end(); ++it )
    {
        auto side = dst.find( it->first );
        if( side == dst.end() )
            continue;
        const std::string val = prefix + "." + it->first;
        modified |= ::TryRead( side->second.created, src, val + ".created" );
    }
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteSideConfig
// Created: NPT 2013-04-08
// -----------------------------------------------------------------------------
void WriteSideConfig( Tree& dst, const std::string& prefix, const session::Config::T_Sides& src )
{
    auto& tree = dst.put_child( prefix, Tree() );
    for( auto it = src.begin(); it != src.end(); ++it )
    {
        tree.put( it->first + "." + "name", it->second.name );
        tree.put( it->first + "." + "created", it->second.created );
    }
}

// -----------------------------------------------------------------------------
// Name: ReadProfileConfig
// Created: BAX 2013-04-19
// -----------------------------------------------------------------------------
bool ReadProfileConfig( session::Config::T_Profiles& dst, const Tree& src, const std::string& prefix )
{
    const auto tree = src.get_child_optional( prefix );
    if( !tree )
        return false;
    session::Config::T_Profiles next;
    for( auto it = tree->begin(); it != tree->end(); ++it )
        next.insert( session::Profile( it->first, Get< std::string>( it->second, "password" ) ) );
    std::swap( dst, next );
    return dst != next;
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
        const std::string name = Utf8( path );
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
void WritePluginConfig( Tree& dst, const std::string& prefix, const session::PluginConfig& cfg )
{
    dst.put( prefix + "enabled", cfg.enabled );
    BOOST_FOREACH( const session::PluginConfig::T_Parameters::value_type& value, cfg.parameters )
        dst.put( prefix + XpathToJson( value.first ), value.second );
}

bool ReadAuthorizedUsers( session::Config::T_AuthorizedUsers& dst, const Tree& src, const std::string& prefix )
{
    session::Config::T_AuthorizedUsers next;
    Iterate( src, prefix, [&]( const std::string& key, const Tree& data )
    {
        const int id = boost::lexical_cast< int >( key );
        const auto& name = data.get< std::string >( "name" );
        web::session::User user( id, name );
        TryReadSet( user.profiles, data, "profiles" );
        next.insert( user );
    });
    std::swap( dst, next );
    return dst != next;
}

void WriteAuthorizedUsers( Tree& dst, const std::string& prefix, const session::Config::T_AuthorizedUsers& src )
{
    auto& root = dst.put_child( prefix, Tree() );
    for( auto it = src.begin(); it != src.end(); ++it )
    {
        auto child = Tree();
        child.put( "name", it->name );
        if( !it->profiles.empty() )
            PutList( child, "profiles", it->profiles );
        root.push_back( std::make_pair( boost::lexical_cast< std::string >( it->id ), child ) );
    }
}
}

// -----------------------------------------------------------------------------
// Name: WriteProfileConfig
// Created: BAX 2013-04-19
// -----------------------------------------------------------------------------
void session::WriteProfileConfig( Tree& dst, const std::string& prefix, const session::Config::T_Profiles& src )
{
    for( auto it = src.begin(); it != src.end(); ++it )
        dst.put( prefix + it->username + ".password", it->password );
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool session::ReadConfig( session::Config& dst, const Plugins& plugins, const Tree& src )
{
    bool modified = false;
    modified |= TryRead( dst.name, src, "name" );
    modified |= TryRead( dst.checkpoints.enabled, src, "checkpoints.enabled" );
    modified |= TryRead( dst.checkpoints.frequency, src, "checkpoints.frequency" );
    modified |= TryRead( dst.checkpoints.keep, src, "checkpoints.keep" );
    modified |= ReadLogConfig( dst, src, "logs." );
    modified |= TryRead( dst.time.end_tick, src, "time.end_tick" );
    modified |= TryRead( dst.time.factor, src, "time.factor" );
    modified |= TryRead( dst.time.paused, src, "time.paused" );
    modified |= TryRead( dst.time.step, src, "time.step" );
    modified |= TryRead( dst.pathfind.threads, src, "pathfind.threads" );
    modified |= TryRead( dst.recorder.frequency, src, "recorder.frequency" );
    modified |= TryRead( dst.rng.seed, src, "rng.seed" );
    modified |= TryRead( dst.reports.clean_frequency, src, "reports.clean_frequency" );
    modified |= TryRead( dst.sides.no_side_objects, src, "sides.no_side_objects" );
    modified |= TryRead( dst.mapnik.enabled, src, "mapnik.enabled" );
    modified |= TryRead( dst.restricted.enabled, src, "restricted.enabled" );
    modified |= ReadAuthorizedUsers( dst.restricted.users, src, "restricted.list" );
    modified |= ReadSideConfig( dst.sides.list, src, "sides.list" );
    modified |= ReadProfileConfig( dst.profiles, src, "profiles" );
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
void session::WriteConfig( Tree& dst, const session::Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "checkpoints.enabled", cfg.checkpoints.enabled );
    dst.put( "checkpoints.frequency", cfg.checkpoints.frequency );
    dst.put( "checkpoints.keep", cfg.checkpoints.keep );
    WriteLogConfig( dst, "logs.", cfg );
    dst.put( "time.end_tick", cfg.time.end_tick );
    dst.put( "time.factor", cfg.time.factor );
    dst.put( "time.paused", cfg.time.paused );
    dst.put( "time.step", cfg.time.step );
    dst.put( "pathfind.threads", cfg.pathfind.threads );
    dst.put( "recorder.frequency", cfg.recorder.frequency );
    dst.put( "rng.seed", cfg.rng.seed );
    dst.put( "reports.clean_frequency", cfg.reports.clean_frequency );
    dst.put( "sides.no_side_objects", cfg.sides.no_side_objects );
    dst.put( "mapnik.enabled", cfg.mapnik.enabled );
    dst.put( "restricted.enabled", cfg.restricted.enabled );
    WriteAuthorizedUsers( dst, "restricted.list", cfg.restricted.users );
    WriteSideConfig( dst, "sides.list", cfg.sides.list );
    WriteProfileConfig( dst, "profiles.", cfg.profiles );
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
bool node::ReadConfig( node::Config& dst, const Tree& src )
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
void node::WriteConfig( Tree& dst, const node::Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "sessions.max_play", cfg.sessions.max_play );
    dst.put( "sessions.max_parallel", cfg.sessions.max_parallel );
    dst.put( "sessions.reset", cfg.sessions.reset );
    PutList( dst, "plugins", cfg.plugins );
}
