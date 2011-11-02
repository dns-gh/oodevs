// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "LoggerApplication.h"
#include "tools/ExerciseConfig.h"
#include <iostream>

int main( int argc, char** argv )
{
    std::cout << "Usage: loadtester hostname [logFile [verbose [login password]]]]" << std::endl;
    if( argc < 2 )
        return 1;
    std::string hostname( argv[1] );
    std::string logFile( argc > 2 ? argv[2] : "" );
    std::string verbose;
    if( argc > 3 )
        verbose = argv[3];
    std::string login;
    if( argc > 4 )
        login = argv[4];
    std::string password;
    if( argc > 5 )
        password = argv[5];
    LoggerApplication application( hostname, logFile, login, password, verbose == "verbose" );
    int rc = application.Run();
    std::cout << "RC = " << rc << std::endl;
    return rc;
}