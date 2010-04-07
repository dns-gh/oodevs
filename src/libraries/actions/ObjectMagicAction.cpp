// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ObjectMagicAction.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Controller.h"

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::ObjectMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, bool registered /*= true*/ )
    : ActionWithTarget_ABC ( controller, magic, entity )
    , controller_( controller )
    , registered_( registered )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::ObjectMagicAction( xml::xistream& xis, kernel::Controller& controller, const kernel::MagicActionType& magic, const kernel::Entity_ABC& entity )
    : ActionWithTarget_ABC( xis, controller, magic, entity )
    , controller_         ( controller )
    , registered_         ( true )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::~ObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
ObjectMagicAction::~ObjectMagicAction()
{
    if( registered_ )
        controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Polish
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void ObjectMagicAction::Polish()
{
    if( registered_ )
        controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ObjectMagicAction::Serialize
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
void ObjectMagicAction::Serialize( xml::xostream& xos ) const
{
    xos << xml::attribute( "type", "magicunit" );
    ActionWithTarget_ABC::Serialize( xos );
}
