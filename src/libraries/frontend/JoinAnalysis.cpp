// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "JoinAnalysis.h"
#pragma warning( push )
#pragma warning( disable: 4127 4511 4512 4702 )
#include <boost/lexical_cast.hpp>
#pragma warning( pop )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: JoinAnalysis constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::JoinAnalysis( const tools::GeneralConfig& config, const QString& exercise, unsigned port, bool attach )
    : SpawnCommand( config, "gaming_app.exe", attach )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    const std::string host = "--host=localhost:"  // $$$$ AGE 2008-01-07: 
                           + boost::lexical_cast< std::string >( port );
    addArgument( host.c_str() );
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysis destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::~JoinAnalysis()
{
    // NOTHING
}
