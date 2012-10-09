// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef MT_TOOLS_TEMPORARYDIRECTORY_H
#define MT_TOOLS_TEMPORARYDIRECTORY_H

#include <boost/filesystem.hpp>
#include <boost/noncopyable.hpp>
#include <string>

class TemporaryDirectory: private boost::noncopyable
{
public:
     TemporaryDirectory( const std::string& namePrefix, const std::string& basePath );
    ~TemporaryDirectory();

    const boost::filesystem::path& path() const;

private:
    boost::filesystem::path path_;
};

#endif // MT_TOOLS_TEMPORARYDIRECTORY_H