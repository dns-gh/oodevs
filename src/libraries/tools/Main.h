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

namespace tools
{
    class WinArguments;

    int Main(
        const tools::WinArguments& winArgs,
        MT_Logger_ABC::E_Type type,
        bool silentCrash,
        int(*main)( const tools::WinArguments& ) );
}

#endif // Main_h
