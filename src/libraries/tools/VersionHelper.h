// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __VersionHelper_h_
#define __VersionHelper_h_

namespace tools
{
    const char* AppVersion();
    const char* AppProjectVersion();
    const char* AppMajorVersion();
    const char* AppModelVersion(); //$$$ Crap for thales, à gicler à terme
    const std::vector< int > SplitVersion( const std::string& version );

    // Return true if:
    // - expectedVersion represents an undefined version (not an X.Y.Z string)
    // - actualVersion represents an undefined version
    // - expectedVersion is a smaller or equal version than actualVersion.
    // Basically, we want ascending compatibility with tolerance for
    // non-version strings.
    bool CheckVersion( const std::string& expectedVersion, const std::string& actualVersion );
}

#endif // __VersionHelper_h_
