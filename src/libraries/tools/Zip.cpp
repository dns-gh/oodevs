// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "Zip.h"
#include <tools/Path.h>
#include "FileWrapper.h"
#pragma warning( push, 0 )
#include <zipstream/zipstream.h>
#pragma warning( pop )

using namespace tools::zip;

InputArchive::InputArchive( const tools::Path& filename )
    : file_( new ::zip::izipfile( filename.ToUnicode() ) )
{
    if( !file_->isOk() )
        throw MASA_EXCEPTION( "failed to open zip archive '" + filename.ToUTF8() + "'" );
}

InputArchive::~InputArchive()
{
    // NOTHING
}

void InputArchive::ReadPackageFile( const tools::Path& name, const std::function< void( std::istream& ) >& f )
{
    ::zip::izipstream s( *file_, name.ToUTF8().c_str() );
    if( s.bad() )
        throw MASA_EXCEPTION( "file '" + name.ToUTF8() + "' not found in zip archive" );
    f( s );
}

OutputArchive::OutputArchive( const tools::Path& filename )
    : file_( new ::zip::ozipfile( filename.ToUnicode() ) )
{
    if( !file_->isOk() )
        throw MASA_EXCEPTION( "failed to create zip archive '" + filename.ToUTF8() + "'" );
}

OutputArchive::~OutputArchive()
{
    // NOTHING
}

void OutputArchive::WritePackageFile( const tools::Path& name, const std::function< void( std::ostream& ) >& f )
{
    ::zip::ozipstream s( *file_, name.ToUTF8().c_str(), std::ios_base::out | std::ios_base::binary );
    if( s.bad() )
        throw MASA_EXCEPTION( "file '" + name.ToUTF8() + "' cannot be added to zip archive" );
    f( s );
}

namespace
{
    class Extractor : private boost::noncopyable
    {
    public:
         Extractor( const tools::Path& archivePath )
            : archive_( archivePath.ToUnicode() )
            , buffer_( 64*1024 )
        {}

        bool Next()
        {
            return archive_.isOk() && archive_.browse();
        }

        tools::Path GetCurrentFileName() const
        {
            return tools::Path::FromUTF8( archive_.getCurrentFileName() );
        }

        void ExtractCurrentFile( const tools::Path& dest )
        {
            dest.SystemComplete();
            if( dest.FileName() == "." )
                return;

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
        ::zip::izipfile archive_;
        std::vector< char > buffer_;
    };
}

namespace
{
    const tools::Path content = "content.xml"; // $$$$ SBO 2008-03-17: hard coded!
}

void tools::zip::ExtractArchive( const Path& archivePath, const Path& destination )
{
    Extractor extractor( archivePath );
    while( extractor.Next() )
    {
        auto output = destination / extractor.GetCurrentFileName();
        extractor.ExtractCurrentFile( output );
    }
}

void tools::zip::ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& f )
{
    Extractor extractor( filename );
    while( extractor.Next() )
    {
        const tools::Path name = extractor.GetCurrentFileName();
        if( name != content )
            f( name );
    }
}

void tools::zip::InstallPackageFiles( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& f )
{
    Extractor extractor( filename );
    while( extractor.Next() )
    {
        const tools::Path name = extractor.GetCurrentFileName();
        if( name == content )
            continue;
        extractor.ExtractCurrentFile( destination / name );
        f();
    }
}

void tools::zip::ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& f )
{
    InputArchive a( filename );
    a.ReadPackageFile( content, f );
}
