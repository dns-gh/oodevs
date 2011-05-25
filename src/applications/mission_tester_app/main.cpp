// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester/Config.h"
#include "mission_tester/Facade.h"
#include <iostream>

int main( int argc, char* argv[] )
{
    try
    {
        mission_tester::Config config( argc, argv );
        mission_tester::Facade facade( config, config );
        facade.Run();
        return EXIT_SUCCESS;
    }
    catch( std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "unknown exception" << std::endl;
    }
    return EXIT_FAILURE;
}
