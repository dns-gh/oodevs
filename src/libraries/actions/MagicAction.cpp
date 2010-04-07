// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "MagicAction.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;


// -----------------------------------------------------------------------------
// Name: MagicAction constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::MagicAction( const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /*= true*/ )
    : Action_ABC ( controller, magic )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicAction constructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::MagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic )
    : Action_ABC( xis, controller, magic )
    , controller_         ( controller )
    , registered_         ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MagicAction destructor
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
MagicAction::~MagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void MagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Accept
// Created: JSR 2010-04-06
// -----------------------------------------------------------------------------
void MagicAction::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: MagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void MagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "magic" );
    Action_ABC::Serialize( xos );
}

