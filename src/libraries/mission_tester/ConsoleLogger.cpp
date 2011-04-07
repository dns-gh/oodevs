// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "ConsoleLogger.h"
#include <iostream>

using namespace mission_tester;

// -----------------------------------------------------------------------------
// Name: ConsoleLogger constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ConsoleLogger::ConsoleLogger()
    : Logger( std::cout )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConsoleLogger destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
ConsoleLogger::~ConsoleLogger()
{
    // NOTHING
}
