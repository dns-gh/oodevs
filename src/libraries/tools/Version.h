// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Version_h_
#define __Version_h_

namespace tools
{
    const char* AppVersion();
    const char* AppProjectVersion();
    const char* AppMajorVersion();
    const char* AppModelVersion(); //$$$ Crap for thales, à gicler à terme
    const std::vector< int > SplitVersion( const std::string& version );
    bool CheckVersion( const std::string& expectedVersion, const std::string& actualVersion );
}

#endif // __Version_h_
