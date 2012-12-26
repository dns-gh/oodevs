// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef CLIENT_GUI_TOOLS_H
#define CLIENT_GUI_TOOLS_H

#include "clients_kernel/Tools.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

namespace bfs = boost::filesystem;

namespace gui
{
    template< typename Validator >
    QStringList ListDirectories( const std::string& base, Validator v )
    {
        QStringList result;
        const bfs::path root = bfs::path( base );
        if( ! bfs::exists( root ) )
            return result;

        bfs::recursive_directory_iterator end;
        for( bfs::recursive_directory_iterator it( root ); it != end; ++it )
        {
            const bfs::path child = *it;
            if( v( child ) )
            {
                QStringList entry;
                bfs::path p( child );
                for( int i = it.level(); i >= 0; --i )
                {
                    entry.push_front( p.filename().string().c_str() );
                    p = p.parent_path();
                }
                result.append( entry.join( "/" ) );
                it.no_push();
            }
        }
        return result;
    }

    inline bool IsPropagationDir( const bfs::path& dir )
    {
        return bfs::is_directory( dir )
            && bfs::exists( dir / "propagation.xml" );
    }

    inline std::string BuildPropagationDir( const std::string& root, const std::string& path )
    {
        return ( bfs::path( root ) / path ).string();
    }

}  // namespace gui

#endif  // CLIENT_GUI_TOOLS_H
