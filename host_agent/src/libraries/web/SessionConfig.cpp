// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "SessionConfig.h"

using namespace web::session;

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

// -----------------------------------------------------------------------------
// Name: ConvertDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
RngDistribution web::session::ConvertRngDistribution( const std::string& src )
{
    if( src == "linear"   ) return RNG_DISTRIBUTION_LINEAR;
    if( src == "gaussian" ) return RNG_DISTRIBUTION_GAUSSIAN;
    return RNG_DISTRIBUTION_GAUSSIAN;
}

// -----------------------------------------------------------------------------
// Name: ConvertDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
std::string web::session::ConvertRngDistribution( RngDistribution src )
{
    if( src == RNG_DISTRIBUTION_LINEAR   ) return "linear";
    if( src == RNG_DISTRIBUTION_GAUSSIAN ) return "gaussian";
    return "invalid";
}