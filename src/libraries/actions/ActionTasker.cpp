// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionTasker.h"
#include "clients_kernel/Entity_ABC.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionTasker constructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::ActionTasker( const kernel::Entity_ABC* tasker, bool simulation )
    : taskerId_  ( tasker ? tasker->GetId() : 0 )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker destructor
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
ActionTasker::~ActionTasker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::IsSimulation
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
bool ActionTasker::IsSimulation() const
{
    return simulation_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::GetTaskerId
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
unsigned int ActionTasker::GetTaskerId() const
{
    return taskerId_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::SerializeAttributes
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void ActionTasker::SerializeAttributes( xml::xostream& xos ) const
{
    if( taskerId_ )
        xos << xml::attribute( "target", taskerId_ );
}
