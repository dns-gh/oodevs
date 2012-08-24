// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "PluginDirectory.h"

#include "runtime/FileSystem_ABC.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>

using namespace host;
using runtime::FileSystem_ABC;

struct host::Plugin
{
    Plugin( const Path& library )
        : library_( library )
    {
        // NOTHING
    }

    ~Plugin()
    {
        // NOTHING
    }

    Path library_;
};

namespace
{
// -----------------------------------------------------------------------------
// Name: MatchLibrary
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
bool MatchLibrary( boost::optional< Path >& reply, const Path& name, const Path& candidate )
{
    const std::wstring filename = candidate.filename().wstring();
    if( boost::algorithm::starts_with( filename, name.wstring() + L"_plugin" ) )
        if( boost::algorithm::ends_with( filename, L".dll" ) )
            reply = candidate.filename();
    // iterate until we get a candidate
    return reply == boost::none;
}

// -----------------------------------------------------------------------------
// Name: FindLibrary
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
boost::optional< Path > FindLibrary( const FileSystem_ABC& fs, const Path& root, const Path& name )
{
    boost::optional< Path > reply;
    fs.Walk( root, false, boost::bind( &MatchLibrary, boost::ref( reply ), boost::cref( name ), _1 ) );
    return reply;
}

// -----------------------------------------------------------------------------
// Name: ParsePluginDirectory
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
bool ParsePluginDirectory( PluginDirectory::T_Plugins& data, const FileSystem_ABC& fs, const Path& dir )
{
    if( !fs.IsFile( dir / "plugin.xml" ) )
        return true;

    const Path name = dir.filename();
    const boost::optional< Path > library = FindLibrary( fs, dir, name );
    if( library == boost::none )
        return true;

    data.insert( std::make_pair( name, Plugin( *library ) ) );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ParsePlugins
// Created: BAX 2012-08-23
// -----------------------------------------------------------------------------
PluginDirectory::T_Plugins ParsePlugins( const FileSystem_ABC& fs, const Path& root )
{
    PluginDirectory::T_Plugins reply;
    fs.Walk( root, false, boost::bind( &ParsePluginDirectory, boost::ref( reply ), boost::cref( fs ), _1 ) );
    return reply;
}
}

// -----------------------------------------------------------------------------
// Name: PluginDirectory::PluginDirectory
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
PluginDirectory::PluginDirectory( const FileSystem_ABC& fs, const Path& root )
    : plugins_( ParsePlugins( fs, root ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginDirectory::~PluginDirectory
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
PluginDirectory::~PluginDirectory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginDirectory::Count
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
size_t PluginDirectory::Count() const
{
    return plugins_.size();
}

// -----------------------------------------------------------------------------
// Name: PluginDirectory::GetPluginNames
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
PluginDirectory::T_Names PluginDirectory::GetNames( int offset, int limit ) const
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
// Name: PluginDirectory::Has
// Created: BAX 2012-08-24
// -----------------------------------------------------------------------------
bool PluginDirectory::Has( const std::string& name ) const
{
    return plugins_.find( name ) != plugins_.end();
}
