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
    std::cout << "Usage: loadtester hostname logFile [login password]]" << std::endl;
    if( argc < 3 )
        return 1;
    std::string hostname( argv[1] );
    std::string logFile( argv[2] );
    std::string login;
    if( argc > 3 )
        login = argv[3];
    std::string password;
    if( argc > 4 )
        password = argv[4];
    LoggerApplication application( hostname, logFile, login, password );
    return application.Run();
}