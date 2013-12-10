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
#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

using namespace tools::zipextractor;

Archive::Archive( const tools::Path& filename )
    : file_( new zip::izipfile( filename.ToUnicode() ) )
{
    // NOTHING
}

namespace
{
    class ZipExtractor: private boost::noncopyable
    {
    public:
         ZipExtractor( zip::izipfile& archive )
            : archive_( archive )
            , buffer_( 64*1024 )
        {
            // Reset enumeration
            archive_.closeInZip();
        }

        bool Next()
        {
            return archive_.isOk() && archive_.browse();
        }

        tools::Path GetCurrentFileName() const
        {
            return tools::Path::FromUTF8( archive_.getCurrentFileName() );
        }

        void ExtractCurrentFile( tools::Path dest )
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

    private:
        zip::izipfile& archive_;
        std::vector< char > buffer_;
    };
}

void tools::zipextractor::ExtractArchive( const Path& archivePath, const Path& destination )
{
    zip::izipfile archive( archivePath.ToUnicode() );
    ZipExtractor extractor( archive );
    while( extractor.Next() )
    {
        auto output = destination / extractor.GetCurrentFileName();
        extractor.ExtractCurrentFile( output );
    }
}

void tools::zipextractor::ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& f )
{
    zip::izipfile archive( filename.ToUnicode() );
    if( ! archive.isOk() )
        return;
    while( archive.browse() )
    {
        const std::string name = archive.getCurrentFileName();
        if( name != "content.xml" ) // $$$$ SBO 2008-03-17: hard coded!
            f( tools::Path::FromUTF8( name ) );
    }
}

void tools::zipextractor::InstallPackageFile( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& f )
{
    zip::izipfile archive( filename.ToUnicode() );
    if( ! archive.isOk() )
        return;
    ZipExtractor ex( archive );
    while( ex.Next() )
    {
        const tools::Path name = ex.GetCurrentFileName();
        if( name == "content.xml" )
            continue;
        ex.ExtractCurrentFile( destination / name );
        f();
    }
}

void tools::zipextractor::ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& f )
{
    Archive a( filename );
    ReadPackageFile( a, "content.xml", f );
}

void tools::zipextractor::ReadPackageFile( Archive& archive, const tools::Path& name, const std::function< void( std::istream& ) >& f )
{
    if( !archive.file_->isOk() )
        return;
    zip::izipstream zipStream( *archive.file_, name.ToUTF8().c_str() );
    if( ! zipStream.bad() )
        f( zipStream );
}
