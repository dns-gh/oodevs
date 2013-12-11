// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef tools_Zip_h
#define tools_Zip_h

#include <memory>
#include <iosfwd>

namespace zip
{
    class izipfile;
    class ozipfile;
}

namespace tools
{
    class Path;

namespace zip
{

class InputArchive
{
public:
    explicit InputArchive( const tools::Path& filename );
    ~InputArchive();

    void ReadFile( const tools::Path& name, const std::function< void( std::istream& ) >& reader );
    // filter will be called for each file and must return true to actually have it extracted
    void ExtractFiles( const tools::Path& destination, const std::function< bool( const tools::Path& ) >& filter );

private:
    std::unique_ptr< ::zip::izipfile > file_;
};

class OutputArchive
{
public:
    explicit OutputArchive( const tools::Path& filename );
    ~OutputArchive();

    void WriteFile( const tools::Path& name, const std::function< void( std::ostream& ) >& writer );

private:
    std::unique_ptr< ::zip::ozipfile > file_;
};

void ExtractArchive( const tools::Path& archivePath, const tools::Path& destination );

void ListPackageFiles( const tools::Path& filename, const std::function< void( const tools::Path& ) >& viewer );
void InstallPackageFiles( const tools::Path& filename, const tools::Path& destination, const std::function< void() >& notifier );
void ReadPackageContentFile( const tools::Path& filename, const std::function< void( std::istream& ) >& reader );

}
}

#endif // tools_Zip_h
