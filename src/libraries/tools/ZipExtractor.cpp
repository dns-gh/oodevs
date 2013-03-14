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
#include "Path.h"
#include "FileWrapper.h"
#pragma warning( push )
#pragma warning( disable: 4244 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

namespace
{
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
void ExtractFile( zip::izipfile& archive, const char* inputName, const Path& outputName, const Path& destination )
{
    Path p = destination / outputName;
    p.SystemComplete();
    if( p.FileName() == "." )
        return;
    zip::izipstream file( archive, inputName, std::ios_base::in | std::ios_base::binary );
    if( file.good() )
    {
        if( !p.Parent().Exists() )
            p.Parent().CreateDirectories();
        tools::Ofstream output( p, std::ios_base::out | std::ios_base::binary );
        Copy( file, output );
    }
}

// -----------------------------------------------------------------------------
// Name: ExtractArchive
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
void ExtractArchive( const Path& archivePath, const Path& destination )
{
    zip::izipfile archive( archivePath.ToUnicode() );
    while( archive.isOk() && archive.browse() )
    {
        tools::zipextractor::ExtractFile( archive, 0, Path::FromUTF8( archive.getCurrentFileName() ), destination );
    }
}

    } //! namespace zipextractor
} //! namespace tools
