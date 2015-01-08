// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Plugins.h"

#include "runtime/FileSystem_ABC.h"
#include "runtime/PropertyTree.h"

#include <tools/Path.h>

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace web;
using runtime::FileSystem_ABC;
typedef property_tree::T_Tree Tree;

namespace
{
typedef std::map< std::string, std::string > T_Defaults;

void AddSetting( T_Defaults& dst, const Tree& src )
{
    const boost::optional< std::string > display = src.get_optional< std::string >( "<xmlattr>.display" );
    if( display != boost::none )
        return;
    const boost::optional< std::string > name = src.get_optional< std::string >( "<xmlattr>.attribute" );
    if( name == boost::none )
        return;
    boost::optional< std::string > value = src.get_optional< std::string >( "<xmlattr>.default" );
    const std::string def = value == boost::none ? std::string() : *value;
    if( src.get< std::string >( "<xmlattr>.type" ) == "enumeration" )
    {
        std::vector< std::string > tokens;
        boost::algorithm::split( tokens, def, boost::is_any_of( ";" ) );
        value = tokens.front();
    }
    dst.insert( std::make_pair( *name, def ) );
}

void AddSettings( T_Defaults& dst, const Tree& src, const std::string& key )
{
    std::vector< std::string > tokens;
    boost::algorithm::split( tokens, key, boost::is_any_of( "." ) );
    property_tree::Walk( src, tokens.begin(), tokens.end(), boost::bind( &AddSetting, boost::ref( dst ), _1 ) );
}

T_Defaults LoadDefaults( const FileSystem_ABC& fs, const tools::Path& cfg )
{
    const Tree meta = property_tree::FromXml( fs.ReadFile( cfg ) );
    T_Defaults dst;
    AddSettings( dst, meta, "plugin.settings.setting" );
    AddSettings( dst, meta, "plugin.settings.group.settings.setting" );
    AddSettings( dst, meta, "plugin.settings.group.settings.group.settings.setting" );
    return dst;
}
}

struct web::Plugin
{
    Plugin( const FileSystem_ABC& fs, const tools::Path& library, const tools::Path& cfg )
        : library_ ( library )
        , defaults_( LoadDefaults( fs, cfg ) )
    {
        // NOTHING
    }

    ~Plugin()
    {
        // NOTHING
    }

    tools::Path library_;
    T_Defaults  defaults_;
};

namespace
{
// -----------------------------------------------------------------------------
// Name: MatchLibrary
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
bool MatchLibrary( boost::optional< tools::Path >& reply, const tools::Path& name, const tools::Path& candidate )
{
    const std::wstring filename = candidate.FileName().ToUnicode();
    if( boost::algorithm::starts_with( filename, name.ToUnicode() + L"_plugin" ) )
        if( boost::algorithm::ends_with( filename, L".dll" ) )
            reply = candidate.FileName();
    // iterate until we get a candidate
    return reply == boost::none;
}

// -----------------------------------------------------------------------------
// Name: FindLibrary
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
boost::optional< tools::Path > FindLibrary( const FileSystem_ABC& fs, const tools::Path& root, const tools::Path& name )
{
    boost::optional< tools::Path > reply;
    fs.Walk( root, false, boost::bind( &MatchLibrary, boost::ref( reply ), boost::cref( name ), _1 ) );
    return reply;
}

// -----------------------------------------------------------------------------
// Name: ParsePlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
bool ParsePlugins( Plugins::T_Plugins& data, const FileSystem_ABC& fs, const tools::Path& dir )
{
    if( !fs.IsFile( dir / "plugin.xml" ) )
        return true;

    const tools::Path name = dir.FileName();
    const boost::optional< tools::Path > library = FindLibrary( fs, dir, name );
    if( library == boost::none )
        return true;

    data.insert( std::make_pair( name, Plugin( fs, *library, dir / "plugin.xml" ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParsePlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
Plugins::T_Plugins ParsePlugins( const FileSystem_ABC& fs, const tools::Path& root )
{
    Plugins::T_Plugins reply;
    fs.Walk( root, false, boost::bind( &ParsePlugins, boost::ref( reply ), boost::cref( fs ), _1 ) );
    return reply;
}
}

// -----------------------------------------------------------------------------
// Name: Plugins::Plugins
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
Plugins::Plugins( const FileSystem_ABC& fs, const tools::Path& root )
    : plugins_( ParsePlugins( fs, root ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Plugins::~Plugins
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
Plugins::~Plugins()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Plugins::Count
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
size_t Plugins::Count() const
{
    return plugins_.size();
}

// -----------------------------------------------------------------------------
// Name: Plugins::GetPluginNames
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
Plugins::T_Names Plugins::GetNames( int offset, int limit ) const
{
    T_Names names;
    T_Plugins::const_iterator it = plugins_.begin();
    const size_t off = offset > 0 ? static_cast< size_t >( offset ) : 0;
    std::advance( it, std::min( off, plugins_.size() ) );
    for( ; it != plugins_.end() && limit > 0; ++it, --limit )
        names.push_back( it->first );
    return names;
}

// -----------------------------------------------------------------------------
// Name: Plugins::Has
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
bool Plugins::Has( const std::string& name ) const
{
    return plugins_.find( name.c_str() ) != plugins_.end();
}

// -----------------------------------------------------------------------------
// Name: Plugins::GetDefaults
// Created: BAX 2012-08-28
// -----------------------------------------------------------------------------
Plugins::T_Defaults Plugins::GetDefaults( const tools::Path& plugin ) const
{
    T_Plugins::const_iterator it = plugins_.find( plugin );
    if( it == plugins_.end() )
        return T_Defaults();
    return it->second.defaults_;
}

// -----------------------------------------------------------------------------
// Name: Plugins::GetLibrary
// Created: BAX 2012-08-31
// -----------------------------------------------------------------------------
tools::Path Plugins::GetLibrary( const tools::Path& plugin ) const
{
    T_Plugins::const_iterator it = plugins_.find( plugin );
    if( it == plugins_.end() )
        return tools::Path();
    return it->second.library_;
}
