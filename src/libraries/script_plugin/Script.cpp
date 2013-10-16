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
#include "MT_Tools/MT_Logger.h"
#include <directia/brain/Brain.h>
#include <tools/Path.h>

using namespace plugins::script;

namespace
{
    directia::brain::Brain* CreateBrain( tools::Path file )
    {
        file.Normalize();
        std::string workingDirectory = ( file.Parent().IsEmpty() ) ? "." : file.Parent().ToUTF8();
        std::string brainInit = std::string( "brain={file='" ) + file.ToUTF8() + "',type='brain'}plugins={} cwd='" + workingDirectory + "'";
        return new directia::brain::Brain( brainInit );
    }
}

// -----------------------------------------------------------------------------
// Name: Script constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
Script::Script( const tools::Path& file, dispatcher::Registrable_ABC& registrables )
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
