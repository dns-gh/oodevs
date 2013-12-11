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

void InputArchive::ReadFile( const tools::Path& name, const std::function< void( std::istream& ) >& reader )
{
    ::zip::izipstream s( *file_, name.ToUTF8().c_str() );
    if( s.bad() )
        throw MASA_EXCEPTION( "file '" + name.ToUTF8() + "' not found in zip archive" );
    reader( s );
}

void InputArchive::ExtractFiles( const tools::Path& destination, const std::function< bool( const tools::Path& ) >& filter )
{
    std::vector< char > buffer( 64 * 1024 );
    while( file_->isOk() && file_->browse() )
    {
        const auto file = tools::Path::FromUTF8( file_->getCurrentFileName() );
        if( ! filter( file ) )
            continue;
        const auto dest = ( destination / file ).SystemComplete();
        if( dest.FileName() == "." )
            continue;
        dest.Parent().CreateDirectories();
        tools::Ofstream output( dest, std::ios_base::out | std::ios_base::binary );
        for( ;; )
        {
            int read = file_->read( &buffer[0], static_cast< int >( buffer.size() ) );
            if( read <= 0 )
                break;
            output.write( &buffer[0], static_cast< std::streamsize >( read ) );
        }
    }
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

void OutputArchive::WriteFile( const tools::Path& name, const std::function< void( std::ostream& ) >& writer )
{
    ::zip::ozipstream s( *file_, name.ToUTF8().c_str(), std::ios_base::out | std::ios_base::binary );
    if( s.bad() )
        throw MASA_EXCEPTION( "file '" + name.ToUTF8() + "' cannot be added to zip archive" );
    writer( s );
}

namespace
{
    const tools::Path content = "content.xml"; // $$$$ SBO 2008-03-17: hard coded!
}

void tools::zip::ExtractArchive( const tools::Path& archivePath, const tools::Path& destination )
{
    InputArchive a( archivePath );
    a.ExtractFiles( destination, [&]( const tools::Path& ) { return true; } );
}

void tools::zip::ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& viewer )
{
    InputArchive a( filename );
    a.ExtractFiles( "", // $$$$ MCO 2013-12-11: 
        [&]( const tools::Path& file ) -> bool
        {
            if( file != content )
                viewer( file );
            return false;
        } );
}

void tools::zip::InstallPackageFiles( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& notifier )
{
    InputArchive a( filename );
    a.ExtractFiles( destination,
        [&]( const tools::Path& file ) -> bool
        {
            if( file == content )
                return false;
            notifier();
            return true;
        } );
}

void tools::zip::ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& reader )
{
    InputArchive a( filename );
    a.ReadFile( content, reader );
}
