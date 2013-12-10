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
    class ozipfile;
}

namespace tools
{
    class Path;

namespace zipextractor
{

class InputArchive
{
public:
    explicit InputArchive( const tools::Path& filename );
    ~InputArchive();

    void ReadPackageFile( const tools::Path& name, const std::function< void( std::istream& ) >& f );

    boost::scoped_ptr< zip::izipfile > file_;
};

class OutputArchive
{
public:
    explicit OutputArchive( const tools::Path& filename );
    ~OutputArchive();

    void WritePackageFile( const tools::Path& name, const std::function< void( std::ostream& ) >& f );

    boost::scoped_ptr< zip::ozipfile > file_;
};

void ExtractArchive( const Path& archivePath, const Path& destination );

void ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& f );
void InstallPackageFiles( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& f );
void ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& f );

}
}

#endif // __ZipExtractor_h_
