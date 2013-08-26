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
JoinAnalysis::JoinAnalysis( const tools::GeneralConfig& config, const tools::Path& exercise, const tools::Path& session, const QString& profile, bool attach )
    : SpawnCommand( config, MakeBinaryName( "gaming_app" ), attach, "" )
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    if( !session.IsEmpty() )
        AddSessionArgument( session );
    if( profile.isEmpty() )
        AddArgument( "--login=\"anonymous\"" );
    else
        AddArgument( "--login=\"" + profile +"\"" );
}

// -----------------------------------------------------------------------------
// Name: JoinAnalysis destructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
JoinAnalysis::~JoinAnalysis()
{
    // NOTHING
}
