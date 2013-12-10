// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ZipExtractor_h_
#define __ZipExtractor_h_

namespace tools
{
    class Path;

namespace zipextractor
{

void ExtractArchive( const Path& archivePath, const Path& destination );

void ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& f );
void InstallPackageFile( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& f );
void ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& f );

}
}

#endif // __ZipExtractor_h_
