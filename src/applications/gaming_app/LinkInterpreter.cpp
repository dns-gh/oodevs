// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LinkInterpreter.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/ProfileFilter.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LinkInterpreter constructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::LinkInterpreter( QObject* parent, Controllers& controllers, ProfileFilter& filter )
     : gui::LinkInterpreter( parent, controllers )
     , filter_( filter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter destructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::~LinkInterpreter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::InterpreteEntity
// Created: SBO 2009-03-04
// -----------------------------------------------------------------------------
bool LinkInterpreter::InterpreteEntity( const kernel::Entity_ABC& entity, const QString& action )
{
    if( action == "filter" )
    {
        filter_.SetFilter( entity );
        return true;
    }
    return LinkInterpreter_ABC::InterpreteEntity( entity, action );
}
