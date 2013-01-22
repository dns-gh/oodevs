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

namespace zip
{
    class izipfile;
}

namespace tools
{
    namespace zipextractor
    {
        // Extract a specific files from an archive
        void ExtractFile( zip::izipfile& archive, const char* inputName, const std::string& outputName, const std::string& destination );

        // Extract the whole archive
        void ExtractArchive( const std::string& archivePath, const std::string& destination );
    }
}

#endif // __ZipExtractor_h_
