// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SESSION_CONFIG_H
#define SESSION_CONFIG_H

#include <boost/property_tree/ptree_fwd.hpp>
#include <string>

namespace web
{
    struct Request_ABC;
    typedef boost::property_tree::ptree Tree;
}

namespace web
{
namespace session
{
// -----------------------------------------------------------------------------
// Name: session::RngDistribution
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
enum RngDistribution
{
    RNG_DISTRIBUTION_LINEAR = 0,
    RNG_DISTRIBUTION_GAUSSIAN,
    RNG_DISTRIBUTION_COUNT
};

// -----------------------------------------------------------------------------
// Name: session::RngConfig
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
struct RngConfig
{
    RngConfig();
    RngDistribution distribution;
    double          deviation;
    double          mean;
};

// -----------------------------------------------------------------------------
// Name: session::Config
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
struct Config
{
    Config();
    std::string     name;
    struct
    {
        bool        paused;
        int         step;
        int         factor;
        int         end_tick;
    }               time;
    struct
    {
        bool        enabled;
        int         frequency;
        int         keep;
    }               checkpoints;
    struct
    {
        int         seed;
        RngConfig   fire;
        RngConfig   wound;
        RngConfig   perception;
        RngConfig   breakdown;
    }               rng;
    struct
    {
        int         threads;
    }               pathfind;
    struct
    {
        int         frequency;
    }               recorder;
};

RngDistribution ConvertRngDistribution( const std::string& src );
std::string     ConvertRngDistribution( RngDistribution src );
Config          GetConfig( const Request_ABC& request );
Tree            ConvertConfig( const Request_ABC& request );
bool            ReadConfig( Config& dst, const Tree& src );
void            WriteConfig( Tree& dst, const Config& cfg );
}
}

#endif // SESSION_CONFIG_H