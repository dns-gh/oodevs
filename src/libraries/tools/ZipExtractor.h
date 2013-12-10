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

#include <boost/scoped_ptr.hpp>

namespace zip
{
    class izipfile;
}

namespace tools
{
    class Path;

namespace zipextractor
{

class Archive
{
public:
    explicit Archive( const tools::Path& filename );

    boost::scoped_ptr< zip::izipfile > file_;
};

void ExtractArchive( const Path& archivePath, const Path& destination );

void ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& f );
void InstallPackageFile( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& f );
void ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& f );
void ReadPackageFile( Archive& archive, const tools::Path& name, const std::function< void( std::istream& ) >& f );

}
}

#endif // __ZipExtractor_h_
