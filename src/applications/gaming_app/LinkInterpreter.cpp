// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LinkInterpreter.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LinkInterpreter constructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::LinkInterpreter( QObject* parent, Controllers& controllers )
     : LinkInterpreter_ABC( parent, controllers.actions_ )
     , controllers_       ( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter destructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::~LinkInterpreter()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const kernel::Entity_ABC& entity )
{
    AddEntity( entity.GetTypeName(), entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    RemoveEntity( entity.GetTypeName(), entity );
}
