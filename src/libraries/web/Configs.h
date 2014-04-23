// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CONFIGS_H
#define CONFIGS_H

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <map>
#include <set>
#include <string>

namespace web
{
    class  Plugins;
    struct Request_ABC;
    typedef boost::filesystem::path Path;
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
// Name: session::PluginConfig
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
struct PluginConfig
{
    PluginConfig( const Plugins& plugins, const Path& path );
    typedef std::map< std::string, std::string > T_Parameters;
    bool         enabled;
    Path         library;
    T_Parameters parameters;
};

struct Side
{
    Side( const std::string& name, bool created );
    Side();
    std::string name;
    bool created;
};

struct Profile
{
    Profile( const std::string& username, const std::string& password );
    bool operator<( const Profile& other ) const;
    bool operator==( const Profile& other ) const;
    std::string username;
    std::string password;
};

enum LogLevel
{
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_ALL
};

// -----------------------------------------------------------------------------
// Name: session::Config
// Created: BAX 2012-08-02
// -----------------------------------------------------------------------------
struct Config
{
    Config();
    typedef std::map< std::string, PluginConfig > T_Plugins;
    typedef std::map< std::string, Side > T_Sides;
    typedef std::set< Profile > T_Profiles;
    typedef std::map< int, std::string > T_AuthorizedUsers;
    std::string       name;
    T_Plugins         plugins;
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
        LogLevel    level;
        bool        rotate;
        int         max_files;
        int         max_size;
        std::string size_unit;
    }               logs;
    struct
    {
        int         threads;
    }               pathfind;
    struct
    {
        int         frequency;
    }               recorder;
    struct
    {
        int         clean_frequency;
    }               reports;
    struct
    {
        bool        no_side_objects;
        T_Sides     list;
    }               sides;
    struct
    {
        T_AuthorizedUsers users;
        bool              enabled;
    }                     restricted;
    struct
    {
        bool        enabled;
    }               mapnik;
    T_Profiles      profiles;
};

bool ReadConfig ( Config& dst, const Plugins& plugins, const Tree& src );
void WriteConfig( Tree& dst, const Config& cfg );
void WriteProfileConfig( Tree& dst, const std::string& prefix, const session::Config::T_Profiles& src );
}

namespace node
{
// -----------------------------------------------------------------------------
// Name: node::Config
// Created: BAX 2012-08-09
// -----------------------------------------------------------------------------
struct Config
{
    Config();
    typedef std::set< std::string > T_Plugins;
    std::string name;
    struct
    {
        size_t  max_play;
        size_t  max_parallel;
        bool    reset;
    }           sessions;
    T_Plugins   plugins;
};

bool   ReadConfig ( Config& dst, const Tree& src );
void   WriteConfig( Tree& dst, const Config& cfg );
}
}

#endif // CONFIGS_H
