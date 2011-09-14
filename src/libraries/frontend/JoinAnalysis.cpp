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
#include <boost/lexical_cast.hpp>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: JoinAnalysis constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::JoinAnalysis( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& profile, unsigned port, bool attach )
    : SpawnCommand( config, "gaming_app.exe", attach, "" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    if( !session.isEmpty() )
        AddSessionArgument( session );
    if( profile.isEmpty() )
        AddArgument( "--login=\"anonymous\"" );
    else
        AddArgument( "--login=\"" + profile +"\"" );

    const std::string host = "--host=localhost:"  // $$$$ AGE 2008-01-07:
                           + boost::lexical_cast< std::string >( port );
    AddArgument( host.c_str() );
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysis destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::~JoinAnalysis()
{
    // NOTHING
}
