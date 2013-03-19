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

#include <boost/noncopyable.hpp>

namespace zip
{
    class izipfile;
}

namespace tools
{
class Path;

namespace zipextractor
{

class ZipExtractor: private boost::noncopyable
{
public:
     ZipExtractor( zip::izipfile& archive );
    ~ZipExtractor();

    bool Next();
    tools::Path GetCurrentFileName() const;
    void ExtractCurrentFile( tools::Path destination );

private:
    zip::izipfile& archive_;
    std::vector< char > buffer_;
};

// Extract the whole archive
void ExtractArchive( const Path& archivePath, const Path& destination );

}  // namespace zipextractor
}  // namespace tools

#endif // __ZipExtractor_h_
