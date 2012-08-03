// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SessionConfig.h"
#include "Request_ABC.h"
#include "runtime/PropertyTree.h"

#include <boost/lexical_cast.hpp>

using namespace web;
using namespace web::session;
using property_tree::TryRead;

// -----------------------------------------------------------------------------
// Name: RngConfig::RngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
RngConfig::RngConfig()
    : distribution( RNG_DISTRIBUTION_LINEAR )
    , deviation   ( 0.5 )
    , mean        ( 0.5 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::Config
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
Config::Config()
    : name()
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
    if( src == "linear"   ) return RNG_DISTRIBUTION_LINEAR;
    if( src == "gaussian" ) return RNG_DISTRIBUTION_GAUSSIAN;
    return RNG_DISTRIBUTION_GAUSSIAN;
}

// -----------------------------------------------------------------------------
// Name: ConvertDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
std::string ConvertRngDistribution( RngDistribution src )
{
    if( src == RNG_DISTRIBUTION_LINEAR   ) return "linear";
    if( src == RNG_DISTRIBUTION_GAUSSIAN ) return "gaussian";
    return "invalid";
}
}

namespace
{
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
    dst = boost::lexical_cast< T >( opt );
    return true;
}

// -----------------------------------------------------------------------------
// Name: GetRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
RngConfig GetRngConfig( const Request_ABC& request, const std::string& prefix )
{
    RngConfig cfg;
    std::string dist;
    bool valid = TryRead( dist, request, prefix + "distribution" );
    if( valid )
        cfg.distribution = ConvertRngDistribution( dist );
    TryRead( cfg.deviation, request, prefix + "deviation" );
    TryRead( cfg.mean, request, prefix + "mean" );
    return cfg;
}
}

// -----------------------------------------------------------------------------
// Name: ReadConfiguration
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
Config web::session::GetConfig( const Request_ABC& request )
{
    Config cfg;
    TryRead( cfg.name, request, "name" );
    TryRead( cfg.checkpoints.enabled, request, "checkpoints_enabled" );
    TryRead( cfg.checkpoints.frequency, request, "checkpoints_frequency" );
    TryRead( cfg.checkpoints.keep, request, "checkpoints_keep" );
    TryRead( cfg.pathfind.threads, request, "pathfind_threads" );
    TryRead( cfg.recorder.frequency, request, "recorder_frequency" );
    TryRead( cfg.rng.seed, request, "rng_seed" );
    cfg.rng.breakdown = GetRngConfig( request, "rng_breakdown_" );
    cfg.rng.fire = GetRngConfig( request, "rng_fire_" );
    cfg.rng.perception = GetRngConfig( request, "rng_perception_" );
    cfg.rng.wound = GetRngConfig( request, "rng_wound_" );
    TryRead( cfg.time.end_tick, request, "time_end_tick" );
    TryRead( cfg.time.factor, request, "time_factory" );
    TryRead( cfg.time.paused, request, "time_paused" );
    TryRead( cfg.time.step, request, "time_step" );
    return cfg;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: TryPut
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void TryPut( Tree& dst, const Request_ABC& request, const std::string& out, const std::string& in )
{
    const boost::optional< std::string > opt = request.GetParameter( in );
    if( opt == boost::none )
        return;
    dst.put( out, opt );
}

// -----------------------------------------------------------------------------
// Name: ConvertRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void ConvertRngConfig( Tree& dst, const Request_ABC& request, const std::string& out, const std::string& in )
{
    TryPut( dst, request, out + "distribution", in + "distribution" );
    TryPut( dst, request, out + "deviation", in + "deviation" );
    TryPut( dst, request, out + "mean", in + "mean" );
}
}

// -----------------------------------------------------------------------------
// Name: ConvertConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
Tree web::session::ConvertConfig( const Request_ABC& request )
{
    Tree dst;
    TryPut( dst, request, "name", "name" );
    TryPut( dst, request, "checkpoints.enabled", "checkpoints_enabled" );
    TryPut( dst, request, "checkpoints.frequency", "checkpoints_frequency" );
    TryPut( dst, request, "checkpoints.keep", "checkpoints_keep" );
    TryPut( dst, request, "time.end_tick", "time_end_tick" );
    TryPut( dst, request, "time.factor", "time_factor" );
    TryPut( dst, request, "time.paused", "time_paused" );
    TryPut( dst, request, "time.step", "time_step" );
    TryPut( dst, request, "rng.seed", "rng_seed" );
    ConvertRngConfig( dst, request, "rng.breakdown.", "rng_breakdown_" );
    ConvertRngConfig( dst, request, "rng.fire.", "rng_fire_" );
    ConvertRngConfig( dst, request, "rng.perception.", "rng_perception_" );
    ConvertRngConfig( dst, request, "rng.wound.", "rng_wound_" );
    TryPut( dst, request, "pathfind.threads", "pathfind_threads" );
    TryPut( dst, request, "recorder.frequency", "recorder_frequency" );
    return dst;
}

namespace
{
// -----------------------------------------------------------------------------
// Name: ReadRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool ReadRngConfig( RngConfig& dst, const Tree& src, const std::string& prefix )
{
    std::string dist;
    bool modified = ::TryRead( dist, src, prefix + ".distribution" );
    if( modified )
        dst.distribution = ConvertRngDistribution( dist );
    modified |= ::TryRead( dst.deviation, src, prefix + ".deviation" );
    modified |= ::TryRead( dst.mean, src, prefix + ".mean" );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteRngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void WriteRngConfig( Tree& dst, const std::string& prefix, const RngConfig& cfg )
{
    dst.put( prefix + ".distribution", ConvertRngDistribution( cfg.distribution ) );
    dst.put( prefix + ".deviation", cfg.deviation );
    dst.put( prefix + ".mean", cfg.mean );
}
}

// -----------------------------------------------------------------------------
// Name: ReadConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
bool web::session::ReadConfig( Config& dst, const Tree& src )
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
    modified |= TryRead( dst.rng.seed, src, "rng.seed" );
    modified |= ReadRngConfig( dst.rng.breakdown, src, "rng.breakdown" );
    modified |= ReadRngConfig( dst.rng.fire, src, "rng.fire" );
    modified |= ReadRngConfig( dst.rng.perception, src, "rng.perception" );
    modified |= ReadRngConfig( dst.rng.wound, src, "rng.wound" );
    modified |= TryRead( dst.pathfind.threads, src, "pathfind.threads" );
    modified |= TryRead( dst.recorder.frequency, src, "recorder.frequency" );
    return modified;
}

// -----------------------------------------------------------------------------
// Name: WriteConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
void web::session::WriteConfig( Tree& dst, const Config& cfg )
{
    dst.put( "name", cfg.name );
    dst.put( "checkpoints.enabled", cfg.checkpoints.enabled );
    dst.put( "checkpoints.frequency", cfg.checkpoints.frequency );
    dst.put( "checkpoints.keep", cfg.checkpoints.keep );
    dst.put( "time.end_tick", cfg.time.end_tick );
    dst.put( "time.factor", cfg.time.factor );
    dst.put( "time.paused", cfg.time.paused );
    dst.put( "time.step", cfg.time.step );
    dst.put( "rng.seed", cfg.rng.seed );
    WriteRngConfig( dst, "rng.breakdown", cfg.rng.breakdown );
    WriteRngConfig( dst, "rng.fire", cfg.rng.fire );
    WriteRngConfig( dst, "rng.perception", cfg.rng.perception );
    WriteRngConfig( dst, "rng.wound", cfg.rng.wound );
    dst.put( "pathfind.threads", cfg.pathfind.threads );
    dst.put( "recorder.frequency", cfg.recorder.frequency );
}