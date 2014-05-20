// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Pathfind.h"

Pathfind::Pathfind( kernel::Controller& controller, const sword::Pathfind& msg )
    : EntityImplementation< Pathfind_ABC >( controller, msg.id(), QString() /* no name yet */, true )
    , controller_( controller )
    , data_( new sword::Pathfind( msg ) )
{
    AddExtension( *this );
    controller_.Create( *this );
}

Pathfind::~Pathfind()
{
    Destroy();
    controller_.Delete( *this );
}