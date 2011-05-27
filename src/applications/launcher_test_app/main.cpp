// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_test_app_pch.h"

#include <stdexcept>
#include <iostream>

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>

#include "App.h"
#include "PBTools.h"
#include "protocol/LauncherSenders.h"

#include <xeumeuleu/xml.hpp>

#include <qapplication.h>

int main( int argc, char* argv[] )
{   
    try
    {        
        launcher_test_app::App app( argc, argv );
        app.Connect();
        app.ProcessMessages();
        app.Wait(-1);

        return EXIT_SUCCESS;
    }
    catch( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "unknown exception" << std::endl;
    }
    return EXIT_FAILURE;
}



