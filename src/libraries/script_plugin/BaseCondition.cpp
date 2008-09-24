// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "BaseCondition.h"
#include "MT/MT_Logger/MT_Logger_lib.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: BaseCondition constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
BaseCondition::BaseCondition()
    : active_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseCondition destructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
BaseCondition::~BaseCondition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BaseCondition::Activate
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void BaseCondition::Activate( bool active )
{
    active_ = active;
}

// -----------------------------------------------------------------------------
// Name: BaseCondition::SetTarget
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void BaseCondition::SetTarget( const directia::WrappedScriptFunction& function )
{
    function_.reset( new directia::WrappedScriptFunction( function ) );
}

// -----------------------------------------------------------------------------
// Name: BaseCondition::Trigger
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void BaseCondition::Trigger()
{
    try
    {
        if( active_ && function_.get() )
           (*function_)();
    }
    catch( std::exception& e )
    {
        LogError( e );
    }
}

// -----------------------------------------------------------------------------
// Name: BaseCondition::LogError
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void BaseCondition::LogError( const std::exception& e )
{
    MT_LOG_ERROR_MSG( "Error in script : " << e.what() )
}
