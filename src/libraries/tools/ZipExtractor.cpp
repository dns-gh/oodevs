// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "ZipExtractor.h"
#include <boost/filesystem/operations.hpp>
#pragma warning( push )
#pragma warning( disable: 4244 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

namespace
{
    bfs::path Normalize( bfs::path& p )
    {
        bfs::path result;
        for( bfs::path::iterator it = p.begin(); it != p.end(); ++it )
            if( *it == ".." ) // $$$$ SBO 2008-03-18: && !result.is_root()
                result.remove_leaf();
            else
                result /= *it;
        return result;
    }

    void Copy( std::istream& file, std::ostream& output )
    {
        std::istreambuf_iterator< char > it( file );
        std::istreambuf_iterator< char > end;
        std::ostreambuf_iterator< char > out( output );
        std::copy( it, end, out );
    }
}

namespace tools
{
    namespace zipextractor
    {

// -----------------------------------------------------------------------------
// Name: ExtractFile
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
void ExtractFile( zip::izipfile& archive, const char* inputName, const std::string& outputName, const std::string& destination )
{
    bfs::path p = bfs::path( destination ) / outputName;
    p = Normalize( p );
    if( p.filename() == "." )
        return;
    zip::izipstream file( archive, inputName, std::ios_base::in | std::ios_base::binary );
    if( file.good() )
    {
        if( !bfs::exists( p.branch_path() ) )
            bfs::create_directories( p.branch_path() );
        std::ofstream output( p.string().c_str(), std::ios_base::out | std::ios_base::binary );
        Copy( file, output );
    }
}

// -----------------------------------------------------------------------------
// Name: ExtractArchive
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
void ExtractArchive( const std::string& archivePath, const std::string& destination )
{
    zip::izipfile archive( archivePath );
    while( archive.isOk() && archive.browse() )
    {
        tools::zipextractor::ExtractFile( archive, 0, archive.getCurrentFileName(), destination );
    }
}

    } //! namespace zipextractor
} //! namespace tools
