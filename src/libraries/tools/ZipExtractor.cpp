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
#include <tools/Path.h>
#include "FileWrapper.h"
#pragma warning( push )
#pragma warning( disable: 4244 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

namespace tools
{
namespace zipextractor
{

ZipExtractor::ZipExtractor( zip::izipfile& archive )
    : archive_( archive )
    , buffer_( 64*1024 )
{
    // Reset enumeration
    archive_.closeInZip();
}

ZipExtractor::~ZipExtractor()
{
}

bool ZipExtractor::Next()
{
    return archive_.isOk() && archive_.browse();
}

tools::Path ZipExtractor::GetCurrentFileName() const
{
    return Path::FromUTF8( archive_.getCurrentFileName() );
}

void ZipExtractor::ExtractCurrentFile( tools::Path dest )
{
    dest.SystemComplete();
    if( dest.FileName() == "." )
        return;

    if( !dest.Parent().Exists() )
        dest.Parent().CreateDirectories();
    tools::Ofstream output( dest, std::ios_base::out | std::ios_base::binary );

    for( ;; )
    {
        int read = archive_.read( &buffer_[0], static_cast< int >( buffer_.size() ));
        if( read <= 0 )
            break;
        output.write( &buffer_[0], static_cast< std::streamsize >( read ));
    }
}

// -----------------------------------------------------------------------------
// Name: ExtractArchive
// Created: ABR 2013-01-21
// -----------------------------------------------------------------------------
void ExtractArchive( const Path& archivePath, const Path& destination )
{
    zip::izipfile archive( archivePath.ToUnicode() );
    ZipExtractor extractor( archive );
    while( extractor.Next() )
    {
        auto output = destination / extractor.GetCurrentFileName();
        extractor.ExtractCurrentFile( output );
    }
}

    } //! namespace zipextractor
} //! namespace tools
