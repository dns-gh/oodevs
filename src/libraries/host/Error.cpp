// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Error.h"
#include "runtime/FileSystem_ABC.h"
#include <boost/filesystem.hpp>
#include <boost/xpressive/xpressive.hpp>

namespace
{
    std::string ExtractLastError( const std::string& content )
    {
        const std::string tag = "<Simulation> <functERR> ";
        auto pos = content.rfind( tag );
        if( pos == std::string::npos )
            return "";
        const std::string tail = content.substr( pos + tag.size() );
        static const std::string date = "^\\[\\d{4}\\-\\d{2}\\-\\d{2} \\d{2}:\\d{2}:\\d{2}\\]";
        static const auto regex = boost::xpressive::sregex::compile( date + " <Simulation>" );
        boost::xpressive::smatch what;
        boost::xpressive::regex_search( tail, what, regex );
        if( what.empty() )
            return tail;
        return what.prefix();
    }
}

std::string host::GetLastError( const runtime::FileSystem_ABC& fs, const Path& output )
{
    std::set< runtime::Path > logs;
    fs.Walk( output, false, [&]( const runtime::Path& path ) -> bool
    {
        const auto file = path.filename().string();
        static const auto regex = boost::xpressive::sregex::compile( "Sim(\\.\\d{8}T\\d{6})?\\.log(\\.\\d+)*" );
        if( boost::xpressive::regex_match( file, regex ) )
            logs.insert( path );
        return true;
    } );
    for( auto it = logs.rbegin(); it != logs.rend(); ++it )
    {
        const std::string content = fs.ReadFile( *it );
        const std::string error = ExtractLastError( content );
        if( !error.empty() )
            return error;
        if( content.find( "<Simulation> <info> Sword Simulation" ) != std::string::npos )
            return "";
    }
    return "";
}
