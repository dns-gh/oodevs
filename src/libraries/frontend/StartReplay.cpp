// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartReplay.h"
#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartReplay constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::StartReplay( const tools::GeneralConfig& config, const QString& exercise, const QString& session, unsigned port, bool attach )
    : SpawnCommand( config, "replayer_app.exe", attach )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument ( session );
    const std::string portArg = "--port=" + boost::lexical_cast< std::string >( port );
    addArgument( portArg.c_str() );
}

// -----------------------------------------------------------------------------
// Name: StartReplay destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartReplay::~StartReplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartReplay::Wait
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
bool StartReplay::Wait()
{
    return true; 
}

