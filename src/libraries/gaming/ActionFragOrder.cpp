// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionFragOrder.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/FragOrder.h"

// -----------------------------------------------------------------------------
// Name: ActionFragOrder constructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::ActionFragOrder( const kernel::Entity_ABC& entity, const kernel::FragOrder& fragOrder, kernel::Controller& controller )
    : Action_ABC( controller )
    , controller_( controller )
    , entity_( entity )
    , fragOrder_( fragOrder )
{
    controller_.Create( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder destructor
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
ActionFragOrder::~ActionFragOrder()
{
    controller_.Delete( *(Action_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::GetName
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
QString ActionFragOrder::GetName() const
{
    return fragOrder_.GetName();
}

// -----------------------------------------------------------------------------
// Name: ActionFragOrder::GetEntity
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& ActionFragOrder::GetEntity() const
{
    return entity_;
}
