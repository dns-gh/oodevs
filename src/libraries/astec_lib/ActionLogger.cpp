// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "ActionLogger.h"
#include "Controllers.h"
#include "MT/MT_logger/MT_Logger_lib.h"
#include "Agent_ABC.h"
#include "Object_ABC.h"
#include "Team.h"
#include "Population.h"
#include "KnowledgeGroup.h"

// -----------------------------------------------------------------------------
// Name: ActionLogger constructor
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
ActionLogger::ActionLogger( Controllers& controllers )
    : controllers_( controllers )
    , selecting_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger destructor
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
ActionLogger::~ActionLogger()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::BeforeSelection
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::BeforeSelection()
{
    selecting_ = true;
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::AfterSelection
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::AfterSelection()
{
    if( selecting_ )
    {
        MT_LOG_INFO_MSG( "Selecting unknown object" );
        selecting_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::Select
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::Select( const Agent_ABC& element )
{
    LogSelection( element );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::Select
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::Select( const Object_ABC& element )
{
    LogSelection( element );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::Select
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::Select( const Team& element )
{
    LogSelection( element );
}
    
// -----------------------------------------------------------------------------
// Name: ActionLogger::Select
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::Select( const Population& element )
{
    LogSelection( element );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::Select
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
void ActionLogger::Select( const KnowledgeGroup& element )
{
     LogSelection( element );
}

// -----------------------------------------------------------------------------
// Name: ActionLogger::LogSelection
// Created: AGE 2006-06-19
// -----------------------------------------------------------------------------
template< typename T >
void ActionLogger::LogSelection( const T& element )
{
    selecting_ = false;
    MT_LOG_INFO_MSG( "Selecting " << typeid( T ).name() << " id " << element.GetId() << " (" << element.GetName() << ')' );
}
