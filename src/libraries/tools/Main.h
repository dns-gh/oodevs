// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef Main_h
#define Main_h

#include "MT_Tools/MT_Logger_ABC.h"
#include <memory>
#include <string>

namespace tools
{
    class WinArguments;

    // Returns the union of all levels of same or greater importance than the
    // one defined by "name'. Returns 0 if name does not match anything.
    int GetLogLevel( const std::string& name );

    int Main(
        const tools::WinArguments& winArgs,
        MT_Logger_ABC::E_Type type,
        bool silentCrash,
        int(*main)( const tools::WinArguments& ) );
}

#endif // Main_h
