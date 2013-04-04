// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "EnvHelpers.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace tools
{

std::vector< tools::Path > ExpandEnvPath( const std::wstring& path, bool checkDir )
{
    std::vector< std::wstring > parts;
    boost::split( parts, path, boost::algorithm::is_any_of( ";" ) );

    std::vector< tools::Path > expandedParts;
    const unsigned int size = 1024;
    WCHAR expanded[size];
    for( auto it = parts.begin(); it != parts.end(); ++it )
    {
        std::wstring& part = *it;
        if( part.find( L"%" ) != std::wstring::npos )
        {
            int done = ExpandEnvironmentStringsW( part.c_str(), expanded, size );
            if( done < size )
                part = expanded;
        }
        if( part.empty() )
            continue;
        auto p = tools::Path::FromUnicode( part );
        if( !checkDir || p.IsDirectory() )
            expandedParts.push_back( p );
    }
    return expandedParts;
}

std::vector< tools::Path > ExpandEnvPath( bool checkDir )
{
    return ExpandEnvPath( _wgetenv( L"PATH" ), checkDir );
}

}  // namespace tools
