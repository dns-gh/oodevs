// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "Script.h"
#include "FiniteStateMachine.h"
#include "dispatcher/Registrable_ABC.h"
#include "directia/brain/Brain.h"
#include "MT_Tools/MT_Logger.h"

using namespace plugins::script;

namespace
{
    directia::brain::Brain* CreateBrain( const std::string& file )
    {
        std::string path( file );
        std::size_t lookHere = 0;
        std::size_t foundHere;
        while( ( foundHere = path.find( "\\", lookHere ) ) != std::string::npos )
        {
            path.replace( foundHere, 1, "/" );
            lookHere = foundHere + 1;
        }
        foundHere = path.find_last_of( "/" );
        std::string workingDirectory = ( foundHere == std::string::npos ) ? "." : path.substr( 0, foundHere );
        std::string brainInit = std::string( "brain={file='" ) + path + "',type='brain'}plugins={} cwd='" + workingDirectory + "'";
        return new directia::brain::Brain( brainInit );
    }
}

// -----------------------------------------------------------------------------
// Name: Script constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Script::Script( const std::string& file, dispatcher::Registrable_ABC& registrables )
    : brain_( CreateBrain( file ) )
    , fsm_  ( new FiniteStateMachine( *brain_ ) )
{
    registrables.RegisterIn( *brain_ );
    (*brain_)[ "Start" ]();
}

// -----------------------------------------------------------------------------
// Name: Script destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Script::~Script()
{
    // NOTHING
}
