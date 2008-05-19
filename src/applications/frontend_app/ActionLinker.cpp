// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "ActionLinker.h"
#include "Panel_ABC.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ActionLinker constructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
ActionLinker::ActionLinker()
    : sub_( this )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionLinker destructor
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
ActionLinker::~ActionLinker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionLinker::Register
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void ActionLinker::Register( const QString& action, Panel_ABC& panel )
{
    panels_[ action ] = &panel;
}

// -----------------------------------------------------------------------------
// Name: ActionLinker::operator()
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
ActionLinker::SubLinker& ActionLinker::operator,( const QString& action )
{
    current_ = action;
    return sub_;
}

// -----------------------------------------------------------------------------
// Name: ActionLinker::Sublink
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void ActionLinker::Sublink( const QString& action )
{
    Panel_ABC* from = panels_[ current_ ];
    Panel_ABC* to   = panels_[ action ];
    if( from && to )
        from->LinkTo( *to );
    else 
        throw std::runtime_error( ( "No action " + action ).ascii() );
    current_ = action;
}

