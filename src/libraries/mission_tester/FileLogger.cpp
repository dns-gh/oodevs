// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "FileLogger.h"
#include <fstream>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: FileLogger constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
FileLogger::FileLogger( const std::string& filename )
    : T_Stream( new std::ofstream( filename.c_str() ) )
    , Logger( *T_Stream::member )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileLogger destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
FileLogger::~FileLogger()
{
    // NOTHING
}
