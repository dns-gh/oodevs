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
ActionTasker::ActionTasker( kernel::Controller& controller, const kernel::Entity_ABC* tasker, bool simulation )
    : tasker_( controller, tasker )
    , taskerId_( tasker ? tasker->GetId() : 0 )
    , taskerTypename_( tasker ? tasker->GetTypeName() : std::string() )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionTasker constructor
// Created: ABR 2014-01-14
// -----------------------------------------------------------------------------
ActionTasker::ActionTasker( kernel::Controller& controller, unsigned int id, const std::string& type, bool simulation /*= true*/ )
    : tasker_( controller, 0 )
    , taskerId_( id )
    , taskerTypename_( type )
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
// Name: ActionTasker::GetId
// Created: JSR 2013-01-31
// -----------------------------------------------------------------------------
unsigned int ActionTasker::GetId() const
{
    return tasker_ ? tasker_->GetId() : taskerId_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::SerializeAttributes
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void ActionTasker::SerializeAttributes( xml::xostream& xos ) const
{
    if( tasker_ || taskerId_ )
        xos << xml::attribute( "target", taskerId_ );
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::GetTasker
// Created: ABR 2014-01-13
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ActionTasker::GetTasker() const
{
    return tasker_;
}

// -----------------------------------------------------------------------------
// Name: ActionTasker::GetTypename
// Created: ABR 2014-01-13
// -----------------------------------------------------------------------------
const std::string& ActionTasker::GetTypename() const
{
    return tasker_ ? tasker_->GetTypeName() : taskerTypename_;
}
